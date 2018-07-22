# ./fourth_app 
[fourth_app] creating thread 0
[Paul][fourth_drv_open][26] fourth_drv open ok
[Paul][fourth_drv_write][74] whois = 11
[Paul][fourth_drv_write][77] write slow start
[Paul][fourth_drv_write][81] write slow time = 0
[fourth_app] creating thread 1
[Paul][fourth_drv_open][26] fourth_drv open ok
[Paul][fourth_drv_write][74] whois = 12
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
