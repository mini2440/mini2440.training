    # insmod sixth_drv.ko 
    [Paul][mod_init][170] mod_init ok
    # ./sixth_app  &
    # [Paul][sixth_drv_open][32] sixth_drv open ok
    [sixth_app] cnt = 1
    [sixth_app] cnt = 2

    # echo "hello" > /dev/sixth_drv_device
    [Paul][sixth_drv_open][32] sixth_drv open ok
    [Paul][sixth_drv_write][87] write size = 6
    [Paul][sixth_drv_write][91] sixth_drv write ok
    [si[Paul][sixth_drv_read][59] read size = 5
    [Paul][sixth_drv_read][63] sixth_drv read ok
    [Paul][sixth_drv_release][38] sixth_drv release ok
    xth_app] data arrive
    #  1:h  e  l  l  o  
    status: 0
    [sixth_app] cnt = 3
    [sixth_app] cnt = 4
    [sixth_app] cnt = 5
    [sixth_app] cnt = 6

    # 
    # ps | grep "sixth"
      835 0          1804 S   ./sixth_app 
      837 0          3096 S   grep sixth 
    # 
    # [sixth_app] cnt = 7
    [sixth_app] cnt = 8
    [sixth_app] cnt = 9
    [sixth_app] cnt = 10

    # 
    # kill 835
    # [Paul][sixth_drv_release][38] sixth_drv release ok

    [1] + Terminated                 ./sixth_app