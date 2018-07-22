#include <stdio.h>
#include <fcntl.h>      /* O_RDWR */
#include <pthread.h>    /* pthread_create */
#include <stdlib.h>

#define RW_BUFF_SIZE (512)
#define NUM_THREADS 16

/* 线程函数 */
void *wirite_share_mem(void *threadid)
{
        long tid;
        tid = (long)threadid;

        unsigned char *wrbuf;
        unsigned char *rdbuf;
        int row_count = 32;
        int i;
        int fd;

        wrbuf = malloc(RW_BUFF_SIZE);
        rdbuf = malloc(RW_BUFF_SIZE);

        fd = open("/dev/fourth_device", O_RDWR);
        if(fd < 0)
        {
                printf("[fourth_app] share mem node open fail\n");
                return -1;
        }

        if(tid == NUM_THREADS - 1)
        {
                lseek(fd, 0, 0);
                read(fd, rdbuf, RW_BUFF_SIZE);
                for(i = 0; i < RW_BUFF_SIZE; i++)
                {
                        if(i == 0)
                                printf("%*d:", 2, 1);
                        printf("%*x", 3, *(rdbuf + i));
                        if(i % row_count == (row_count - 1))
                                printf("\n%*d:", 2, i / row_count + 2);
                }
                printf("\n");
        }
        else
        {
                for(i = 0; i < RW_BUFF_SIZE; i++)
                        *(wrbuf + i) = tid;
                lseek(fd, 0, 0);
                write(fd, wrbuf, RW_BUFF_SIZE);
        }

        close(fd);
        pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
        pthread_t threads[NUM_THREADS];
        int rc;
        long t;

        for(t = 0 ; t < NUM_THREADS; t++){ /* 循环创建 5 个线程 */
                printf("In main: creating thread %ld\n", t);
                rc = pthread_create(&threads[t], NULL, wirite_share_mem, (void *)t); /* 创建线程 */
                if (rc){
                        printf("ERROR; return code from pthread_create() is %d\n", rc);
                        exit(-1);
                }
                }
        printf("In main: exit!\n");
        pthread_exit(NULL); /* 主线程退出 */
        return 0;
}
