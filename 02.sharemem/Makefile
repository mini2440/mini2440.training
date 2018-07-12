obj-m += second_drv.o

KERNEL = /home/baohua/develop/linux

all:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- -C $(KERNEL) M=`pwd` modules
	arm-linux-gnueabi-gcc second_app.c -o second_app
	@echo ">>>>>> make all successful <<<<<<"

clean:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- -C $(KERNEL) M=`pwd` modules clean
	@echo ">>>>>> make clean successful <<<<<<"

