#!/bin/bash

### 判断参数个数是否合法
if [ $# -ne 1 ]; then
    echo "USAGE: $0 num"
    echo "eg: $0 1"
    echo "eg: $0 2"
    echo "eg: $0 3"
    echo "eg: $0 41"
    echo "eg: $0 42"
    echo "eg: $0 5"
    echo "eg: $0 6"
    echo ""
    exit 1; 
fi

### 从 windows 共享文件夹中拷贝源码到 ubuntu 虚拟机中
### 并编译测试程序
if [ $1 -eq 1 ]; then
    cp -av /media/sf_share/01.char ./
    cd 01.char
    arm-linux-gnueabi-gcc first_app.c -o app_first

elif [ $1 -eq 2 ]; then
    cp -av /media/sf_share/02.memory ./
    cd 02.memory
    arm-linux-gnueabi-gcc second_app.c -o app_second

elif [ $1 -eq 3 ]; then
    cp -av /media/sf_share/03.rwtest ./
    cd 03.rwtest
    arm-linux-gnueabi-gcc third_app.c -o app_third

elif [ $1 -eq 41 ]; then
    cp -av /media/sf_share/04.unused_mutex ./
    cd 04.unused_mutex
    arm-linux-gnueabi-gcc fourth_app.c -o app_fourth

elif [ $1 -eq 42 ]; then
    cp -av /media/sf_share/04.used_mutex ./
    cd 04.used_mutex
    arm-linux-gnueabi-gcc fourth_app.c -o app_fourth

elif [ $1 -eq 5 ]; then
    cp -av /media/sf_share/05.wait ./
    cd 05.wait
    arm-linux-gnueabi-gcc fifth_app.c -o app_fifth

elif [ $1 -eq 6 ]; then
    cp -av /media/sf_share/06.poll ./
    cd 06.poll
    arm-linux-gnueabi-gcc sixth_app.c -o app_sixth

fi

if [ $? -ne 0 ]; then
    echo "build test app fail"
    exit 1;
fi

### 编译驱动模块
make

if [ $? -ne 0 ]; then
    echo "build driver fail"
    exit 1;
fi

### 删除开发板行原来的驱动和测试程序
sudo rm -rf /home/baohua/develop/linux/extra/img/*.ko
sudo rm -rf /home/baohua/develop/linux/extra/img/app_*

### 将当前编译得到的驱动和测试程序拷贝到开发板上
sudo cp -av app_* /home/baohua/develop/linux/extra/img/
sudo cp -av *.ko /home/baohua/develop/linux/extra/img/
sync

### 进入开发板路径, 运行开发板
cd /home/baohua/develop/linux/extra/
./run-lcd.sh

