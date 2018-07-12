#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/uaccess.h>        /* copy_to_user ... */
#include <linux/slab.h>         /* kmalloc ... */

/* simple log */
#define log(fmt, arg...)\
        printk(KERN_INFO "[Paul][%s][%d] "fmt"\n", __func__, __LINE__, ##arg);

#define MEM_SIZE (4*1024*1024)

struct share {
        unsigned char mem[MEM_SIZE];  // share mem
};

struct share *sh;

static loff_t third_llseek(struct file *filp, loff_t offset, int whence)
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

static int third_open(struct inode *my_indoe, struct file *my_file)
{
        log("third open ok");
        return 0;
}

static int third_release(struct inode *my_indoe, struct file *my_file)
{
        log("third release ok");
        return 0;
}

static ssize_t third_read(struct file *my_file, char __user *buff, size_t cnt, loff_t *loff)
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

        log("third write ok");
        return ret;
}

static ssize_t third_write(struct file *my_file, const char __user *buff, size_t cnt, loff_t *loff)
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

        log("third write ok");
        return ret;
}

static struct file_operations third_fops =
{
        .open    = third_open,
        .llseek  = third_llseek,
        .release = third_release,
        .read    = third_read,
        .write   = third_write,
};

static int third_major;
static int third_minor;
static dev_t third_devno;

struct cdev *third_device;
static struct class * third_class;

static int __init mod_init(void)
{
        int ret;

        ret = alloc_chrdev_region(&third_devno, third_minor, 1, "third_device");
        third_major = MAJOR(third_devno);
        if (ret < 0)
        {
                log("cannot get third_major %d", third_major);
                return -1;
        }

        third_device = cdev_alloc();
        if (third_device != NULL)
        {
                cdev_init(third_device, &third_fops);
                third_device->owner = THIS_MODULE;
                if (cdev_add(third_device, third_devno, 1) != 0)
                {
                        log("add cdev error!");
                        goto error;
                }
        }
        else
        {
                log("cdev_alloc error!");
                return -1;
        }

        third_class = class_create(THIS_MODULE, "third_class");
        if (IS_ERR(third_class))
        {
                log("create class error");
                return -1;
        }
        device_create(third_class, NULL, third_devno, NULL, "third_device");

        sh = (unsigned char *)kzalloc(MEM_SIZE, GFP_KERNEL);
        if (sh == NULL) {
                ret = -ENOBUFS;
                goto outfree;
        }

        log("mod_init ok");
        return 0;

outfree:
        kfree(sh);
error:
        unregister_chrdev_region(third_devno, 1);
        return ret;
}

static void __exit mod_exit(void)
{
        cdev_del(third_device);
        unregister_chrdev_region(third_devno, 1);
        device_destroy(third_class, third_devno);
        class_destroy(third_class);
        kfree(sh);
        log("mod_exit ok");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");

