#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/uaccess.h>        /* copy_to_user ... */
#include <linux/slab.h>         /* kmalloc ... */
#include <linux/delay.h>

/* simple log */
#define log(fmt, arg...)\
        printk(KERN_INFO "[Paul][%s][%d] "fmt"\n", __func__, __LINE__, ##arg);

#define MEM_SIZE (4*1024*1024)

struct share {
        unsigned char mem[MEM_SIZE];  // share mem
};

struct share *sh;

static loff_t fourth_llseek(struct file *filp, loff_t offset, int whence)
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

static int fourth_open(struct inode *my_indoe, struct file *my_file)
{
        // log("fourth open ok");
        return 0;
}

static int fourth_release(struct inode *my_indoe, struct file *my_file)
{
        // log("fourth release ok");
        return 0;
}

static ssize_t fourth_read(struct file *my_file, char __user *buff, size_t cnt, loff_t *loff)
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
                // log("read size = %d", count);
        }

        // log("fourth read ok");
        return ret;
}

static ssize_t fourth_write(struct file *my_file, const char __user *buff, size_t cnt, loff_t *loff)
{
        unsigned long p = *loff;
        unsigned int count = cnt;
        int ret = 0;
        int i = 0;

        if(p >= MEM_SIZE)
                return 0;
        if(count > MEM_SIZE - p)
                count = MEM_SIZE - p;
        
        if(copy_from_user(sh->mem + p, buff, count))
                ret = -EFAULT;
        else
        {
                for(i = 0; i < count; i++)
                {
                        *(sh->mem + p + i) = *(sh->mem + p + i) + 10;
                        mdelay(10);
                }
                *loff += count;
                ret = count;
                // log("write size = %d", count);
        }

        // log("fourth write ok");
        return ret;
}

static struct file_operations fourth_fops =
{
        .open    = fourth_open,
        .llseek  = fourth_llseek,
        .release = fourth_release,
        .read    = fourth_read,
        .write   = fourth_write,
};

static int fourth_major;
static int fourth_minor;
static dev_t fourth_devno;

struct cdev *fourth_device;
static struct class * fourth_class;

static int __init mod_init(void)
{
        int ret;

        ret = alloc_chrdev_region(&fourth_devno, fourth_minor, 1, "fourth_device");
        fourth_major = MAJOR(fourth_devno);
        if (ret < 0)
        {
                log("cannot get fourth_major %d", fourth_major);
                return -1;
        }

        fourth_device = cdev_alloc();
        if (fourth_device != NULL)
        {
                cdev_init(fourth_device, &fourth_fops);
                fourth_device->owner = THIS_MODULE;
                if (cdev_add(fourth_device, fourth_devno, 1) != 0)
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

        fourth_class = class_create(THIS_MODULE, "fourth_class");
        if (IS_ERR(fourth_class))
        {
                log("create class error");
                return -1;
        }
        device_create(fourth_class, NULL, fourth_devno, NULL, "fourth_device");

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
        unregister_chrdev_region(fourth_devno, 1);
        return ret;
}

static void __exit mod_exit(void)
{
        cdev_del(fourth_device);
        unregister_chrdev_region(fourth_devno, 1);
        device_destroy(fourth_class, fourth_devno);
        class_destroy(fourth_class);
        kfree(sh);
        log("mod_exit ok");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");

