obj-m += first_drv.o

KERNEL = /home/user/mini2440/linux-2.6.22.6
SHARE = /home/user/board

all:
	make -C $(KERNEL) M=`pwd` modules
	/home/user/mini2440/gcc-3.4.5-glibc-2.3.6/bin/arm-linux-gcc first_app.c -o first_app
	rm -rf $(SHARE)/*
	cp -f *.ko $(SHARE)/
	cp -f first_app $(SHARE)/
	make clean
	@echo ">>>>>> make all successful <<<<<<"

clean:
	make -C $(KERNEL) M=`pwd` modules clean
	rm -rf first_app *.symvers
	@echo ">>>>>> make clean successful <<<<<<"
