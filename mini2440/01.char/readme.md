    # cd mnt/
    # ls
    first_app     first_drv.ko
    # insmod first_drv.ko 
    [Paul][mod_init][109] mod_init ok
    # cat /proc/devices | grep "first" 
    252 first_drv_name
    # ls -l /sys/class/first_drv_class/first_drv_device/
    -r--r--r--    1 0        0            4096 Jan  1 00:36 dev
    drwxr-xr-x    2 0        0               0 Jan  1 00:36 power
    lrwxrwxrwx    1 0        0               0 Jan  1 00:39 subsystem -> ../../../class/first_drv_class
    -rw-r--r--    1 0        0            4096 Jan  1 00:39 uevent
    # ls -l /dev/first_drv_device 
    crw-rw----    1 0        0        252,   0 Jan  1 00:36 /dev/first_drv_device
