# DrCracker
 解除学校CrCOM的限制
 使用了[dogcom](https://github.com/mchome/dogcom)的心跳包代码.
 本Repo使用GPLv3开源协议.
#使用
 dial.conf
  username=你的宽带登录账号
  password=你的宽带登录密码
 dogcom.conf
  server = '填上你的Drcom显示的出口IP地址'
  pppoe_flag = '\x2f'
  keep_alive2_flag = '\xdc'
##本项目只限使用PPPoE拨号的校园网,并且账号前加回车符.
