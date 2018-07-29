#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/uaccess.h>        /* copy_to_user ... */
#include <linux/ioctl.h>        /* ioctl ... */
#include <linux/slab.h>         /* kmalloc ... */
#include <linux/proc_fs.h>      /* procfs ... */
#include <linux/delay.h>
#include <linux/sched.h>

/* simple log */
#define log(fmt, arg...)\
        printk(KERN_INFO "[Paul][%s][%d] "fmt"\n", __func__, __LINE__, ##arg);

#define MEM_SIZE (4*1024*1024)
struct share {
        unsigned char mem[MEM_SIZE];  // share mem
};

struct share *sh;
struct mutex share_mutex;

static int fourth_drv_open(struct inode *indoe, struct file *filp)
{
        log("fourth_drv open ok");
        return 0;
}

static int fourth_drv_release(struct inode *indoe, struct file *filp)
{
        log("fourth_drv release ok");
        return 0;
}

static ssize_t fourth_drv_read(struct file *filp, char __user *buff, size_t cnt, loff_t *loff)
{
        unsigned long p = *loff;
        unsigned int count = cnt;
        int ret = 0;
        if(p >= MEM_SIZE)
                return 0;
        if(count > MEM_SIZE - p)
                count = MEM_SIZE - p;

        mutex_lock(&share_mutex);
        if(copy_to_user(buff, sh->mem + p, count))
                ret = -EFAULT;
        else
        {
                *loff += count;
                ret = count;
                log("read size = %d", count);
        }
        mutex_unlock(&share_mutex);

        log("fourth_drv read ok");
        return ret;
}

static ssize_t fourth_drv_write(struct file *filp, const char __user *buff, size_t cnt, loff_t *loff)
{
        unsigned long p = *loff;
        unsigned int count = cnt;
        int ret = 0;
        int i = 0;

        static int whois = 0;
        whois++;

        if(p >= MEM_SIZE)
                return 0;
        if(count > MEM_SIZE - p)
                count = MEM_SIZE - p;

        mutex_lock(&share_mutex);
        log("whois = %d", whois);
        if(whois % 2 == 1)
        {
                log("write slow start");
                for(i = 0; i < count; i++)
                {
                        *(sh->mem + p + i) = *(buff + i);
                        log("write slow time = %d", i);
                        mdelay(1000);
                        schedule();
                }
                log("write slow end");
        }
        else
        {
                log("write fast start");
                for(i = 0; i < count; i++)
                {
                        *(sh->mem + p + i) = *(buff + i);
                        log("write slow time = %d", i);
                        mdelay(50);
                        schedule();
                }
                log("write fast end");
        }
        mutex_unlock(&share_mutex);

        *loff += count;
        ret = count;
        log("write size = %d", count);

        log("fourth_drv write ok");
        return ret;
}


static loff_t fourth_drv_llseek(struct file *filp, loff_t offset, int whence)
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

static struct file_operations fourth_drv_fops =
{
        .open    = fourth_drv_open,
        .llseek  = fourth_drv_llseek,
        .release = fourth_drv_release,
        .read    = fourth_drv_read,
        .write   = fourth_drv_write,
};

struct cdev *fourth_drv;
static dev_t fourth_drv_devno;
static struct class *fourth_drv_class;

static int __init mod_init(void)
{
        alloc_chrdev_region(&fourth_drv_devno, 0, 1, "fourth_drv_name");
        fourth_drv = cdev_alloc();
        cdev_init(fourth_drv, &fourth_drv_fops); 
        fourth_drv->owner = THIS_MODULE;
        cdev_add(fourth_drv, fourth_drv_devno, 1);

        fourth_drv_class = class_create(THIS_MODULE, "fourth_drv_class");
        device_create(fourth_drv_class, NULL, fourth_drv_devno, "fourth_drv_device");

        sh = (unsigned char *)kzalloc(MEM_SIZE, GFP_KERNEL);
        if (sh == NULL) {
                kfree(sh);
                return -ENOBUFS;
        }

        mutex_init(&share_mutex);

        log("mod_init ok");
        return 0;
}

static void __exit mod_exit(void)
{
        cdev_del(fourth_drv);
        unregister_chrdev_region(fourth_drv_devno, 1);
        device_destroy(fourth_drv_class, fourth_drv_devno);
        class_destroy(fourth_drv_class);
        log("mod_exit ok");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");

