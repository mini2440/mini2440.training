
    # cd mnt/
    # ls 
    third_app     third_drv.ko
    # insmod third_drv.ko 
    [Paul][mod_init][140] mod_init ok
    # ls -l /dev/third_drv_device 
    crw-rw----    1 0        0        252,   0 Jan  1 00:04 /dev/third_drv_device
    # ./third_app 
    [third_app] O_RDWR = 2
    [Paul][third_drv_open][25] third_drv open ok
    [Paul][third_drv_write][75] write size = 16
    [Paul][third_drv_write][78] third_drv write ok
    [Paul][third_drv_read][51] read size = 16
    [Paul][third_drv_read][54] third_drv read ok
    [Paul][third_drv_write][75] write size = 8
    [Paul][third_drv_write][78] third_drv write ok
    [Paul][third_drv_read][51] read size = 8
    [Paul][third_drv_read][54] third_drv read ok
    [Paul][third_drv_release][31] third_drv release ok
    the 0~15 is:
    0 1 2 3 4 5 6 7 8 9 A B C D E F 

    set rdbuf = 7
    7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 

    the 4M-8 ~ 4M+8 is:
    0 1 2 3 4 5 6 7 7 7 7 7 7 7 7 7 

