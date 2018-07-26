    # insmod fifth_drv.ko 
    [Paul][mod_init][162] mod_init ok
    # ./fifth_app 
    [fifth_app] creating thread 0
    [Paul][fifth_drv_open][31] fifth_drv open ok
    [fifth_app] creating thread 1
    [Paul][fifth_drv_open][31] fifth_drv open ok
    [Paul][fifth_drv_write][94] write size = 5
    [Paul][fifth_drv_write][98] fifth_drv write ok
    [Paul][fifth_drv_release][37] fifth_drv release ok
    [Paul][fifth_drv_read][65] read size = 5
    [Paul][fifth_drv_read][70] fifth_drv read ok
    [Paul][fifth_drv_release][37] fifth_drv release ok
     1:B  B  B  B  B  
    [fifth_app] main: exit!
    # ./fifth_app 
    [fifth_app] creating thread 0
    [Paul][fifth_drv_open][31] fifth_drv open ok
    [fifth_app] creating thread 1
    [Paul][fifth_drv_open][31] fifth_drv open ok
    [Paul][fifth_drv_write][94] write size = 5
    [Paul][fifth_drv_write][98] fifth_drv write ok
    [Paul][fifth_drv_release][37] fifth_drv release ok
    [Paul][fifth_drv_read][65] read size = 5
    [Paul][fifth_drv_read][70] fifth_drv read ok
    [Paul][fifth_drv_release][37] fifth_drv release ok
     1:B  B  B  B  B  
    [fifth_app] main: exit!
