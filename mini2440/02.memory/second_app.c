#include <stdio.h>
#include <fcntl.h>              /* O_RDWR */
#include <linux/ioctl.h>        /* ioctl */

int main(int argc, char**argv)
{
        int fd;
        int i;
        char *rdbuf;
        char *wrbuf = "0123456789ABCDEF";
        int offset = 4*1024*1024-8;
        rdbuf = malloc(16);

        printf("[second_app] O_RDWR = %d\n", O_RDWR);

        fd = open("/proc/share/mem", O_RDWR);
        if(fd < 0)
        {
                printf("[second_app] share mem node open fail\n");
                return -1;
        }

        /* 向 0~15 号地址中写 0~F */
        lseek(fd, 0, 0);
        write(fd, wrbuf, 16);

        lseek(fd, 0, 0);
        read(fd, rdbuf, 16);

        printf("the 0~15 is:\n");
        for(i = 0; i < 16; i++)
        {
                printf("%c ", *(rdbuf + i));
        }
        printf("\n\n");

        /* 将 rdbuf 填充为 16     个 7 */
        for(i = 0; i < 16; i++)
        {
                *(rdbuf + i) = '7';
        }

        printf("set rdbuf = 7\n");
        for(i = 0; i < 16; i++)
        {
                printf("%c ", *(rdbuf + i));
        }
        printf("\n\n");

        /* 向 4M-8 ~ 4M+8 号地址中写 0~F */
        lseek(fd, offset, 0);
        write(fd, wrbuf, 16);

        lseek(fd, offset, 0);
        read(fd, rdbuf, 16);

        printf("the 4M-8 ~ 4M+8 is:\n");
        for(i = 0; i < 16; i++)
        {
                printf("%c ", *(rdbuf + i));
        }
        printf("\n\n");

        close(fd);

        return 0;
}


 
