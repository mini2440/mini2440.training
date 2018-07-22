#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/uaccess.h>        /* copy_to_user ... */
#include <linux/slab.h>         /* kmalloc ... */
#include <linux/proc_fs.h>

/* simple log */
#define log(fmt, arg...)\
        printk(KERN_INFO "[Paul][%s][%d] "fmt"\n", __func__, __LINE__, ##arg);

static struct proc_dir_entry* proc_dir = NULL;
static struct proc_dir_entry* proc_node = NULL;

#define MEM_SIZE (4*1024*1024)

struct share {
        unsigned char mem[MEM_SIZE];  // share mem
};

struct share *sh;

static int second_open(struct inode *my_indoe, struct file *my_file)
{
        log("second open ok");
        return 0;
}

static int second_release(struct inode *my_indoe, struct file *my_file)
{
        log("second release ok");
        return 0;
}

static loff_t proc_llseek(struct file *filp, loff_t offset, int whence)
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

static ssize_t proc_read(struct file *my_file, char __user *buff, size_t cnt, loff_t *loff)
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

        log("second write ok");
        return ret;
}

static ssize_t proc_write(struct file *my_file, const char __user *buff, size_t cnt, loff_t *loff)
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

        log("second write ok");
        return ret;
}

static struct file_operations second_fops =
{
        .open    = second_open,
        .release = second_release,
};

static const struct file_operations proc_fops = 
{
        .owner  = THIS_MODULE,
        .llseek = proc_llseek,
        .read   = proc_read,
        .write  = proc_write,
};

static int second_major;
static int second_minor;
static dev_t second_devno;

struct cdev *second_device;
static struct class * second_class;

static int __init mod_init(void)
{
        int ret;

        ret = alloc_chrdev_region(&second_devno, second_minor, 1, "second_device");
        second_major = MAJOR(second_devno);
        if (ret < 0)
        {
                log("cannot get second_major %d", second_major);
                return -1;
        }

        second_device = cdev_alloc();
        if (second_device != NULL)
        {
                cdev_init(second_device, &second_fops);
                second_device->owner = THIS_MODULE;
                if (cdev_add(second_device, second_devno, 1) != 0)
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

        second_class = class_create(THIS_MODULE, "second_class");
        if (IS_ERR(second_class))
        {
                log("create class error");
                return -1;
        }
        device_create(second_class, NULL, second_devno, NULL, "second_device");

        sh = (unsigned char *)kzalloc(MEM_SIZE, GFP_KERNEL);
        if (sh == NULL) {
                ret = -ENOBUFS;
                goto outfree;
        }

        proc_dir = proc_mkdir("share", NULL);
        if(!proc_dir)
        {
                remove_proc_entry("share", NULL);
                return -ENOMEM;
        }

        proc_node = proc_create("mem", 0666, proc_dir, &proc_fops);
        if(!proc_node)
        {
                remove_proc_entry("mem", proc_dir);
                return -ENOMEM;
        }

        log("mod_init ok");
        return 0;

outfree:
        kfree(sh);
error:
        unregister_chrdev_region(second_devno, 1);
        return ret;
}

static void __exit mod_exit(void)
{
        cdev_del(second_device);
        unregister_chrdev_region(second_devno, 1);
        device_destroy(second_class, second_devno);
        class_destroy(second_class);
        kfree(sh);
        remove_proc_entry("mem", proc_dir);
        remove_proc_entry("share", NULL);
        log("mod_exit ok");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");

