#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/uaccess.h>        /* copy_to_user ... */
#include <linux/ioctl.h>        

/* simple log */
#define log(fmt, arg...)\
        printk(KERN_INFO "[Paul][%s][%d] "fmt"\n", __func__, __LINE__, ##arg);

#define FIRST_IOC_MAGIC 'F'
#define FIRST_ONLY_EXECUTE _IO(FIRST_IOC_MAGIC, 0)
#define FIRST_ONLY_WRITE   _IOW(FIRST_IOC_MAGIC, 1, int)
#define FIRST_ONLY_READ    _IOR(FIRST_IOC_MAGIC, 2, int)
#define FIRST_RW           _IOWR(FIRST_IOC_MAGIC, 3, int)
#define FIRST_IOCTL_MAXNR  3

int first_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
        if (_IOC_TYPE(cmd) != FIRST_IOC_MAGIC) {
                log("error1");
                return -ENOTTY;
        }

        if (_IOC_NR(cmd) > FIRST_IOCTL_MAXNR) {
                log("error2");
                return -ENOTTY;
        }

        switch(cmd)
        {
                case FIRST_ONLY_EXECUTE:
                        log("FIRST_ONLY_EXECUTE");
                        break;
                case FIRST_ONLY_WRITE:
                        log("FIRST_ONLY_WRITE");
                        break;
                case FIRST_ONLY_READ:
                        log("FIRST_ONLY_READ");
                        break;
                case FIRST_RW:
                        log("FIRST_RW");
                        break;
                default:
                        log("default");
        }
}

static int first_open(struct inode *indoe, struct file *filp)
{
        log("first open ok");
        return 0;
}

static int first_release(struct inode *indoe, struct file *filp)
{
        log("first release ok");
        return 0;
}

static ssize_t first_read(struct file *filp, char __user *buff, size_t cnt, loff_t *loff)
{
        int tmp = 128;
        copy_to_user(buff, &tmp, 4);
        log("first read ok");
        return 0;
}

static ssize_t first_write(struct file *filp, const char __user *buff, size_t cnt, loff_t *loff)
{
        int tmp = 0;
        copy_from_user(&tmp, buff, 4);
        log("tmp = %d", tmp);
        log("first write ok");
        return 0;
}

static struct file_operations first_drv_fops =
{
        .open    = first_open,
        .release = first_release,
        .read    = first_read,
        .write   = first_write,
        .unlocked_ioctl = first_ioctl,
};

static int first_drv_major;
static int first_drv_minor;
struct cdev *first_drv;                                 /* cdev 数据结构 */
static dev_t first_drv_devno;                           /* 设备编号 */
static struct class *first_drv_class;

static int __init mod_init(void)
{
        int ret;

        /* 
         * 从系统获取主设备号，这里指定的驱动名称将会在 /proc/device 下面显示
         */
        ret = alloc_chrdev_region(&first_drv_devno, first_drv_minor, 1, "first_drv_name");
        if (ret < 0)
        {
                log("cannot get major %d \n", first_drv_major);
                return -1;
        }

        first_drv_major = MAJOR(first_drv_devno);

        /* 分配 led 结构 */
        first_drv = cdev_alloc();
        if (first_drv != NULL)
        {
                /* 初始化字符设备结构，并绑定操作函数 */
                cdev_init(first_drv, &first_drv_fops); 
                first_drv->owner = THIS_MODULE;
                
                /* 将字符设备添加到系统中 */
                if (cdev_add(first_drv, first_drv_devno, 1) != 0)
                {
                        log("add cdev error!\n");
                        goto error;
                }
        }
        else
        {
                log("cdev_alloc error!\n");
                return -1;
        }

        /* 在 /sys/class 目录下创建一个 first_drv_class */
        first_drv_class = class_create(THIS_MODULE, "first_drv_class");
        if (IS_ERR(first_drv_class)) {
                log("create class error\n");
                return -1;
        }

        /* 在 /sys/class/first_drv_class 目录下创建一个 first_drv_device */
        device_create(first_drv_class, NULL, first_drv_devno, "first_drv_device");

        log("mod_init ok");
        return 0;

error:
        unregister_chrdev_region(first_drv_devno, 1);
        return ret;
}

static void __exit mod_exit(void)
{
        cdev_del(first_drv);                                    /* 移除字符设备 */
        unregister_chrdev_region(first_drv_devno, 1);           /* 释放设备号 */
        device_destroy(first_drv_class, first_drv_devno);
        class_destroy(first_drv_class);
        log("mod_exit ok");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");

