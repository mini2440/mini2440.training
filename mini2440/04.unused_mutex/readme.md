# insmod fourth_drv.ko 
[Paul][mod_init][166] mod_init ok
# ./fourth_app 
    [fourth_app] creating thread 0
    [Paul][fourth_drv_open][26] fourth_drv open ok
    [Paul][fourth_drv_write][74] whois = 3
    [Paul][fourth_drv_write][77] write slow start
    [Paul][fourth_drv_write][81] write slow time = 0
    [fourth_app] creating thread 1
    [Paul][fourth_drv_open][26] fourth_drv open ok
    [Paul][fourth_drv_write][74] whois = 4
    [Paul][fourth_drv_write][89] write fast start
    [Paul][fourth_drv_write][93] write slow time = 0
    [fourth_app] creating thread 2
    [Paul][fourth_drv_write][93] write slow time = 1
    [fourth_app] main: exit!
    [Paul][fourth_drv_open][26] fourth_drv open ok
    [Paul][fourth_drv_read][52] read size = 5
    [Paul][fourth_drv_read][55] fourth_drv read ok
    [Paul][fourth_drv_release][32] fourth_drv release ok
    [Paul][fourth_drv_write][93] write slow time = 2
     1:B  B  A  A  A  
    [Paul][fourth_drv_write][93] write slow time = 3
    [Paul][fourth_drv_write][93] write slow time = 4
    [Paul][fourth_drv_write][97] write fast end
    [Paul][fourth_drv_write][102] write size = 5
    [Paul][fourth_drv_write][104] fourth_drv write ok
    [Paul][fourth_drv_release][32] fourth_drv release ok
    [Paul][fourth_drv_write][81] write slow time = 1
    [Paul][fourth_drv_write][81] write slow time = 2
    [Paul][fourth_drv_write][81] write slow time = 3
    [Paul][fourth_drv_write][81] write slow time = 4
    [Paul][fourth_drv_write][85] write slow end
    [Paul][fourth_drv_write][102] write size = 5
    [Paul][fourth_drv_write][104] fourth_drv write ok
    [Paul][fourth_drv_release][32] fourth_drv release ok

结果出现 B A 交错的情况

     1:B  B  A  A  A  
 
内核打印信息和应用打印信息重叠了，不方便分析，将内核打印过滤后多试几次(5次)看看：

    # cat /proc/sys/kernel/printk
    7	4	1	7
    # echo "1 4 1 7" > /proc/sys/kernel/printk
    # cat /proc/sys/kernel/printk
    1	4	1	7
    # ./fourth_app 
    [fourth_app] creating thread 0
    [fourth_app] creating thread 1
    [fourth_app] creating thread 2
    [fourth_app] main: exit!
     1:B  B  A  A  A  
    # ./fourth_app 
    [fourth_app] creating thread 0
    [fourth_app] creating thread 1
    [fourth_app] creating thread 2
     1:B  A  A  A  A  
    [fourth_app] main: exit!
    # ./fourth_app 
    [fourth_app] creating thread 0
    [fourth_app] creating thread 1
    [fourth_app] creating thread 2
     1:B  B  A  A  A  
    [fourth_app] main: exit!
    # ./fourth_app 
    [fourth_app] creating thread 0
    [fourth_app] creating thread 1
    [fourth_app] creating thread 2
    [fourth_app] main: exit!
     1:B  B  A  A  A  
    # ./fourth_app 
    [fourth_app] creating thread 0
    [fourth_app] creating thread 1
    [fourth_app] creating thread 2
     1:B  A  A  A  A  
    [fourth_app] main: exit!
