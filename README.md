Sml-http
====
<b>微型高并发http服务器</b>

####[简要介绍]
  这个项目主要是仿照nginx原理实现的，基于epoll & 多进程模型，支持静态网页，各项配置可调。

####[开发环境]
* 系统：Ubuntu 14.04
* 编译工具：gcc

####[主要功能]
  * 1.HTTP请求支持[只支持静态]
  * 2.基于epoll模型，高并发连接支持
  * 3.简单进程间均衡负载
  * 4.简单并发状况日志功能
  * 5.自定义配置监听端口，worker进程数
  * 6.自定义进程锁实现
  * 7.调试输出为彩色界面

####[使用方法]
* 首先进入目录make编译,生成的最终文件是 /bin 目录下SmlHttp
* 本机测试在浏览器中输入地址：http:/127.0.0.1:8888 访问，网页文件来自于网络[侵删]
* 输入 ./SmlHttp -h 查看帮助
* 输入 ./SmlHttp -r XX 开启进程并发状态日志记录，XX代表记录次数，如果是0代表一直记录

####[参考源码]
	darkhttpd-1.10
	tinyhttpd-0.1.0
	monkey-1.6.8
	nginx-1.8.1

  其他系统并未经过测试，不过在我的系统上运行是这样的：
![运行截图](https://github.com/daterlove/smlhttp/blob/master/tools/screen_shot.png)
