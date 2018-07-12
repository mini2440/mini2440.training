#include <stdio.h>
#include <fcntl.h>      /* O_RDWR */
#include <linux/ioctl.h>

int main(int argc, char**argv)
{
        int fd;
        int i;
        char *rdbuf;
        char *zero = "0000000000";
        char *wrbuf = "1234567890";
        int offset = 4*1024*1024-8;
        rdbuf = malloc(10);

        printf("[second_app] O_RDWR = %d\n", O_RDWR);

        fd = open("/proc/share/mem", O_RDWR);
        if(fd < 0)
        {
                printf("[second_app] share mem node open fail\n");
                return -1;
        }
        lseek(fd, offset, 0);
        write(fd, zero, 10);

        lseek(fd, offset, 0);
        read(fd, rdbuf, 10);
        for(i = 0; i < 10; i++)
        {
                printf("[second_app] old mem[%d]:%c\n", i, *(rdbuf + i));
        }

        lseek(fd, offset + 5, 0);
        write(fd, wrbuf, 10);

        lseek(fd, offset, 0);
        read(fd,rdbuf,10);

        for(i = 0; i < 10; i++)
        {
                printf("[second_app] new mem[%d]:%c\n",i,*(rdbuf + i));
        }

        close(fd);

        return 0;
}
