
    # ls -l /proc/share/mem 
    -rw-rw-rw-    1 0        0               0 Jan  1 01:02 /proc/share/mem

    # ./second_app 
    [second_app] O_RDWR = 2
    [Paul][second_drv_proc_write][111] write size = 16
    [Paul][second_drv_proc_write][114] second_drv proc_write write ok
    [Paul][second_drv_proc_read][87] read size = 16
    [Paul][second_drv_proc_read][90] second_drv proc_read write ok
    [Paul][second_drv_proc_write][111] write size = 8
    [Paul][second_drv_proc_write][114] second_drv proc_write write ok
    [Paul][second_drv_proc_read][87] read size = 8
    [Paul][second_drv_proc_read][90] second_drv proc_read write ok
    the 0~15 is:
    0 1 2 3 4 5 6 7 8 9 A B C D E F 

    set rdbuf = 7
    7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 

    the 4M-8 ~ 4M+8 is:
    0 1 2 3 4 5 6 7 7 7 7 7 7 7 7 7e
