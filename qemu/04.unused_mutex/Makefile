obj-m += fourth_drv.o

KERNEL = /home/baohua/develop/linux

all:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- -C $(KERNEL) M=`pwd` modules
	arm-linux-gnueabi-gcc -pthread fourth_app.c -o fourth_app
	@echo ">>>>>> make all successful <<<<<<"

clean:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- -C $(KERNEL) M=`pwd` modules clean
	rm -rf fourth_app
	@echo ">>>>>> make clean successful <<<<<<"

