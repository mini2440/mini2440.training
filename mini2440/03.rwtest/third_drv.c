#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/uaccess.h>        /* copy_to_user ... */
#include <linux/ioctl.h>        /* ioctl ... */
#include <linux/slab.h>         /* kmalloc ... */
#include <linux/proc_fs.h>      /* procfs ... */

/* simple log */
#define log(fmt, arg...)\
        printk(KERN_INFO "[Paul][%s][%d] "fmt"\n", __func__, __LINE__, ##arg);

#define MEM_SIZE (4*1024*1024)

struct share {
        unsigned char mem[MEM_SIZE];  // share mem
};

struct share *sh;

static int third_drv_open(struct inode *indoe, struct file *filp)
{
        log("third_drv open ok");
        return 0;
}

static int third_drv_release(struct inode *indoe, struct file *filp)
{
        log("third_drv release ok");
        return 0;
}

static ssize_t third_drv_read(struct file *filp, char __user *buff, size_t cnt, loff_t *loff)
{
        unsigned long p = *loff;
        unsigned int count = cnt;
        int ret = 0;
        if(p >= MEM_SIZE)
                return 0;
        if(count > MEM_SIZE - p)
                count = MEM_SIZE - p;

        if(copy_to_user(buff, sh->mem + p, count))
                ret = -EFAULT;
        else
        {
                *loff += count;
                ret = count;
                log("read size = %d", count);
        }

        log("third_drv read ok");
        return ret;
}

static ssize_t third_drv_write(struct file *filp, const char __user *buff, size_t cnt, loff_t *loff)
{
        unsigned long p = *loff;
        unsigned int count = cnt;
        int ret = 0;

        if(p >= MEM_SIZE)
                return 0;
        if(count > MEM_SIZE - p)
                count = MEM_SIZE - p;
        
        if(copy_from_user(sh->mem + p, buff, count))
                ret = -EFAULT;
        else
        {
                *loff += count;
                ret = count;
                log("write size = %d", count);
        }

        log("third_drv write ok");
        return ret;
}


static loff_t third_drv_llseek(struct file *filp, loff_t offset, int whence)
{
        loff_t new_pos = 0;
        switch(whence)
        {
                case 0: /* SEEK_SET */
                        new_pos = offset;
                        break;
 
                case 1: /* SEEK_CUR */
                        new_pos = filp->f_pos + offset;
                        break;
 
                case 2: /* SEEK_END */
                        new_pos = MEM_SIZE -1 + offset;
                        break;
 
                default: /* can't happen */
                        return -EINVAL;
        }

        if ((new_pos < 0) || (new_pos > MEM_SIZE))
                return -EINVAL; 
        filp->f_pos = new_pos;
        return new_pos;
}

static struct file_operations third_drv_fops =
{
        .open    = third_drv_open,
        .llseek  = third_drv_llseek,
        .release = third_drv_release,
        .read    = third_drv_read,
        .write   = third_drv_write,
};

struct cdev *third_drv;
static dev_t third_drv_devno;
static struct class *third_drv_class;

static int __init mod_init(void)
{
        alloc_chrdev_region(&third_drv_devno, 0, 1, "third_drv_name");
        third_drv = cdev_alloc();
        cdev_init(third_drv, &third_drv_fops); 
        third_drv->owner = THIS_MODULE;
        cdev_add(third_drv, third_drv_devno, 1);

        third_drv_class = class_create(THIS_MODULE, "third_drv_class");
        device_create(third_drv_class, NULL, third_drv_devno, "third_drv_device");

        sh = (unsigned char *)kzalloc(MEM_SIZE, GFP_KERNEL);
        if (sh == NULL) {
                kfree(sh);
                return -ENOBUFS;
        }

        log("mod_init ok");
        return 0;
}

static void __exit mod_exit(void)
{
        cdev_del(third_drv);
        unregister_chrdev_region(third_drv_devno, 1);
        device_destroy(third_drv_class, third_drv_devno);
        class_destroy(third_drv_class);
        log("mod_exit ok");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");

