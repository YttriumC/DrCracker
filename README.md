# DrCracker
 解除学校DrCOM的限制  
 使用了[dogcom](https://github.com/mchome/dogcom)的心跳包代码.  
 本Repo使用GPLv3开源协议.  
 发现学校使用深信服检测共享, 没辙了, 除了代理流量.
 目前只用做DRCOM软件的代替使用,并不共享网络.
 **不建议使用本项目突破宽带共享限制**
 ### 本项目只限使用PPPoE拨号的校园网,并且账号前加回车符('\r\n').
## 使用
 ### [下载该项目](https://github.com/Lu7fer/DrCracker/releases)
 ### dial.conf
 ```
  username=你的宽带登录账号  
  password=你的宽带登录密码  
 ```
 ### dogcom.conf
 ```
  server = '填上你的Drcom显示的出口IP地址'
  pppoe_flag = '\x2f'
  keep_alive2_flag = '\xdc'
 ```
## Thanks:
- [mchome/dogcom](https://github.com/mchome/dogcom)
# 免责声明:
## 本项目仅供教学使用, 严禁用于商业用途.
## 特别指出禁止任何个人或者公司将 DrCracker 的代码投入商业使用，由此造成的后果和法律责任均与本人无关。
## 开源协议: GPLv3.
