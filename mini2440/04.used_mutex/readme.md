# rmmod fourth_drv
[Paul][mod_exit][181] mod_exit ok
# insmod fourth_drv.ko 
[Paul][mod_init][167] mod_init ok
# ./fourth_app 
[fourth_app] creating thread 0
[Paul][fourth_drv_open][27] fourth_drv open ok
[Paul][fourth_drv_write][76] whois = 1
[Paul][fourth_drv_write][79] write slow start
[Paul][fourth_drv_write][83] write slow time = 0
[Paul][fourth_drv_write][83] write slow time = 1
[Paul][fourth_drv_write][83] write slow time = 2
[Paul][fourth_drv_write][83] write slow time = 3
[Paul][fourth_drv_write][83] write slow time = 4
[Paul][fourth_drv_write][85] write slow end
[Paul][fourth_drv_write][101] write size = 5
[Paul][fourth_drv_write][103] fourth_drv write ok
[f[Paul][fourth_drv_release][33] fourth_drv release ok
[Paul][fourth_drv_open][27] fourth_drv open ok
[Paul][fourth_drv_open][27] fourth_drv open ok
[Paul][fourth_drv_read][53] read size = 5
[Paul][fourth_drv_read][56] fourth_drv read ok
[Paul][fourth_drv_write][76] whois = 2
[Paul][fourth_drv_write][89] write fast start
[Paul][fourth_drv_write][93] write slow time = 0
[Paul][fourth_drv_write][93] write slow time = 1
[Paul][fourth_drv_write][93] write slow time = 2
[Paul][fourth_drv_write][93] write slow time = 3
[Paul][fourth_drv_write][93] write slow time = 4
[Paul][fourth_drv_write][95] write fast end
[Paul][fourth_drv_write][101] write size = 5
[Paul][fourth_drv_write][103] fourth_drv write ok
[Paul][fourth_drv_release][33] fourth_drv release ok
[Paul][fourth_drv_release][33] fourth_drv release ok
ourth_app] creating thread 1
[fourth_app] creating thread 2
[fourth_app] main: exit!
 1:A  A  A  A  A  

