## flash分布

![flash](doc\flash.png)

### bootloader
sector0 ~ sector1 总大小控制在32kb，主要为解密和烧写APP

### APP
sector4~sector5 总打小为192kb

### config
sector3

### key
sector2 由随机数和真秘钥组成，共16kb
