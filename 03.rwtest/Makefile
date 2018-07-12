obj-m += third_drv.o

KERNEL = /home/baohua/develop/linux

all:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- -C $(KERNEL) M=`pwd` modules
	arm-linux-gnueabi-gcc third_app.c -o third_app
	@echo ">>>>>> make all successful <<<<<<"

clean:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- -C $(KERNEL) M=`pwd` modules clean
	rm -rf third_app
	@echo ">>>>>> make clean successful <<<<<<"

