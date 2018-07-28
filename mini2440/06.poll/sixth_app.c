#include <stdio.h>
#include <fcntl.h>      /* O_RDWR */
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define RW_BUFF_SIZE (5)

int main(int argc, char *argv[])
{
        unsigned char *wrbuf;
        unsigned char *rdbuf;
        int row_count = 32;
        int i;
        int fd;

        
        fd_set fds;

        int cnt = 0;

        /* 定义时间结构体 */
        struct timeval time;

        wrbuf = malloc(RW_BUFF_SIZE);
        rdbuf = malloc(RW_BUFF_SIZE);

        fd = open("/dev/sixth_drv_device", O_RDWR);
        if(fd < 0)
        {
                printf("[sixth_app] share mem node open fail\n");
                pthread_exit(NULL);
        }

        while(1)
        {
                /* 清空文件描述符 */
                FD_ZERO(&fds);

                /* 添加文件描述符，可以设置多个 */
                FD_SET(fd, &fds);

                /* 填充时间结构 */
                time.tv_sec  = 5;
                time.tv_usec = 0;

                /* 检测事件 */
                if(select(fd+1, &fds, &fds, &fds, &time) <= 0) //判断是否有事件产生
                {
                        cnt++;
                        printf("[sixth_app] cnt = %d\n", cnt);
                        continue;
                }
                else
                {
                        printf("[sixth_app] data arrive\n");
                        lseek(fd, 0, 0);
                        read(fd, rdbuf, RW_BUFF_SIZE);
                        for(i = 0; i < RW_BUFF_SIZE; i++)
                        {
                                if(i == 0)
                                        printf("%*d:", 2, 1);
                                printf("%c  ", *(rdbuf + i));
                                if(i % row_count == (row_count - 1))
                                        printf("\n%*d:", 2, i / row_count + 2);
                        }
                        printf("\n");
                }

                /* 测试文件是否可读写 */
                printf("status: %d\n", FD_ISSET(fd, &fds));
        }

        close(fd);
        return 0;
}
