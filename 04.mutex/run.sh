#!/bin/bash

sudo cp *_app /home/baohua/develop/linux/extra/img/ -av
sudo cp *_drv.ko /home/baohua/develop/linux/extra/img/ -av
sync
make clean
cd /home/baohua/develop/linux/extra/
./run-lcd.sh
