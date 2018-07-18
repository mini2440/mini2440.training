#include <stdio.h>
#include <fcntl.h>              /* O_RDWR */
#include <linux/ioctl.h>        /* ioctl */

#define FIRST_IOC_MAGIC 'F'
#define FIRST_ONLY_EXECUTE _IO(FIRST_IOC_MAGIC, 0)
#define FIRST_ONLY_WRITE   _IOW(FIRST_IOC_MAGIC, 1, int)
#define FIRST_ONLY_READ    _IOR(FIRST_IOC_MAGIC, 2, int)
#define FIRST_RW           _IOWR(FIRST_IOC_MAGIC, 3, int)
#define FIRST_IOCTL_MAXNR  3

int main(int argc, char**argv)
{
        int fd;
        printf("[app] O_RDWR = %d\n", O_RDWR);

        fd = open("/dev/first_drv_device", O_RDWR);
        if(fd < 0)
        {
                printf("[app] driver open fail\n");
                return -1;
        }

        int write_buf = 100;
        write(fd, &write_buf, 4);

        int read_buf = 0;
        read(fd, &read_buf, 4);

        ioctl(fd, FIRST_ONLY_EXECUTE);
        ioctl(fd, FIRST_ONLY_WRITE, 100);
        ioctl(fd, FIRST_ONLY_READ, 100);
        ioctl(fd, FIRST_RW, 100);

        close(fd);
        printf("[app] read_buf = %d\n", read_buf);

        return 0;
}

