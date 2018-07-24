    # rmmod fourth_drv    
    [Paul][mod_exit][176] mod_exit ok
    # insmod fourth_drv.ko 
    [Paul][mod_init][173] mod_init ok
    # ./fourth_app 
    [fourth_app] creating thread 0
    [Paul][fourth_drv_open][27] fourth_drv open ok
    [Paul][fourth_drv_write][78] whois = 1
    [Paul][fourth_drv_write][81] write slow start
    [Paul][fourth_drv_write][85] write slow time = 0
    [fourth_app] creating thread 1
    [Paul][fourth_drv_open][27] fourth_drv open ok
    [Paul][fourth_drv_write][85] write slow time = 1
    [fourth_app] creating thread 2
    [Paul][fourth_drv_open][27] fourth_drv open ok
    [Paul][fourth_drv_write][85] write slow time = 2
    [fourth_app] main: exit!
    [Paul][fourth_drv_write][85] write slow time = 3
    [Paul][fourth_drv_write][85] write slow time = 4
    [Paul][fourth_drv_write][89] write slow end
    [Paul][fourth_drv_write][107] write size = 5
    [Paul][fourth_drv_write][109] fourth_drv write ok
    [Paul][fourth_drv_write][78] whois = 2
    [Paul][fourth_drv_write][93] write fast start
    [Paul][fourth_drv_write][97] write slow time = 0
    [Paul][fourth_drv_write][97] write slow time = 1
    [Paul][fourth_drv_write][97] write slow time = 2
    [Paul][fourth_drv_write][97] write slow time = 3
    [Paul][fourth_drv_write][97] write slow time = 4
    [Paul][fourth_drv_write][101] write fast end
    [Paul][fourth_drv_write][107] write size = 5
    [Paul][fourth_drv_write][109] fourth_drv write ok
    [Paul][fourth_drv_read][54] read size = 5
    [Paul][fourth_drv_read][58] fourth_drv read ok
     1:[Paul][fourth_drv_release][33] fourth_drv release ok
    [Paul][fourth_drv_release][33] fourth_drv release ok
    [Paul][fourth_drv_release][33] fourth_drv release ok
    B  B  B  B  B  

内核打印信息和应用打印信息重叠了，不方便分析，将内核打印过滤后多试几次(5次)看看：

    # echo "1 4 1 7" > /proc/sys/kernel/printk
    # ./fourth_app 
    [fourth_app] creating thread 0
    [fourth_app] creating thread 1
    [fourth_app] creating thread 2
    [fourth_app] main: exit!
     1:B  B  B  B  B  
    # ./fourth_app 
    [fourth_app] creating thread 0
    [fourth_app] creating thread 1
    [fourth_app] creating thread 2
    [fourth_app] main: exit!
     1:B  B  B  B  B  
    # ./fourth_app 
    [fourth_app] creating thread 0
    [fourth_app] creating thread 1
    [fourth_app] creating thread 2
    [fourth_app] main: exit!
     1:B  B  B  B  B  
    # ./fourth_app 
    [fourth_app] creating thread 0
    [fourth_app] creating thread 1
    [fourth_app] creating thread 2
    [fourth_app] main: exit!
     1:B  B  B  B  B  
    # ./fourth_app 
    [fourth_app] creating thread 0
    [fourth_app] creating thread 1
    [fourth_app] creating thread 2
    [fourth_app] main: exit!
     1:B  B  B  B  B 




