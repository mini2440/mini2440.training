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
#include <linux/poll.h>

/* simple log */
#define log(fmt, arg...)\
        printk(KERN_INFO "[Paul][%s][%d] "fmt"\n", __func__, __LINE__, ##arg);

#define MEM_SIZE (4*1024*1024)

struct share {
        unsigned char mem[MEM_SIZE];  // share mem
};

struct share *sh;
struct mutex share_mutex;
DECLARE_WAIT_QUEUE_HEAD(share_wait_queue_head);

/* 数据是否准备好 0: 没有准备好，不可读; 1: 准备好了，有数据可读 */
int data_ready_flag = 0;

static int sixth_drv_open(struct inode *indoe, struct file *filp)
{
        log("sixth_drv open ok");
        return 0;
}

static int sixth_drv_release(struct inode *indoe, struct file *filp)
{
        log("sixth_drv release ok");
        return 0;
}

static ssize_t sixth_drv_read(struct file *filp, char __user *buff, size_t cnt, loff_t *loff)
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

        log("sixth_drv read ok");
        return ret;
}

static ssize_t sixth_drv_write(struct file *filp, const char __user *buff, size_t cnt, loff_t *loff)
{
        unsigned long p = *loff;
        unsigned int count = cnt;
        int ret = 0;

        if(p >= MEM_SIZE)
                return 0;
        if(count > MEM_SIZE - p)
                count = MEM_SIZE - p;

        mutex_lock(&share_mutex);
        data_ready_flag = 1;
        wake_up_interruptible(&share_wait_queue_head);
        if(copy_from_user(sh->mem + p, buff, count))
                ret = -EFAULT;
        else
        {
                *loff += count;
                ret = count;
                log("write size = %d", count);
        }
        mutex_unlock(&share_mutex);

        log("sixth_drv write ok");
        return ret;
}


static loff_t sixth_drv_llseek(struct file *filp, loff_t offset, int whence)
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

static unsigned int sixth_drv_poll(struct file *filp, struct poll_table_struct *wait)
{
        poll_wait(filp, &share_wait_queue_head, wait);

        if(data_ready_flag != 1)
        {
                return 0;
        }

        data_ready_flag = 0;

        return POLLIN;
}

static struct file_operations sixth_drv_fops =
{
        .open    = sixth_drv_open,
        .llseek  = sixth_drv_llseek,
        .release = sixth_drv_release,
        .read    = sixth_drv_read,
        .write   = sixth_drv_write,
        .poll    = sixth_drv_poll,
};

struct cdev *sixth_drv;
static dev_t sixth_drv_devno;
static struct class *sixth_drv_class;

static int __init mod_init(void)
{
        alloc_chrdev_region(&sixth_drv_devno, 0, 1, "sixth_drv_name");
        sixth_drv = cdev_alloc();
        cdev_init(sixth_drv, &sixth_drv_fops); 
        sixth_drv->owner = THIS_MODULE;
        cdev_add(sixth_drv, sixth_drv_devno, 1);

        sixth_drv_class = class_create(THIS_MODULE, "sixth_drv_class");
        device_create(sixth_drv_class, NULL, sixth_drv_devno, "sixth_drv_device");

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
        cdev_del(sixth_drv);
        unregister_chrdev_region(sixth_drv_devno, 1);
        device_destroy(sixth_drv_class, sixth_drv_devno);
        class_destroy(sixth_drv_class);
        log("mod_exit ok");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");

