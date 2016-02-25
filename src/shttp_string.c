/*************************************************************************
    > File Name: shttp_string.c
    > Author: 浅握双手
    > ---------------- 
    > Created Time: 2016年02月24日 01时07分38秒 CST
 ************************************************************************/

#include"common.h"
void string_print_help()
{
  printf("\e[31m\e[1m%s\e[0m\n","\tSml-Http 帮助");
  printf("\e[33m\e[1m%s\e[0m\n","首先在这里欢迎你体验这小小的 HTTP Server");
  printf("\e[33m\e[1m%s\e[0m\n","这个小服务端主要仿照nginx，基于epoll & 多进程模型");
  printf("\e[33m\e[1m%s\e[0m\n","其中这个服务器端的主要配置如下[可在源码内更改]：");
  
  printf("\e[36m\e[1m  [%s%s%s]\e[0m\n","WEB目录：程序目录下",DIR_HTDOCS,"文件夹");
  printf("\e[36m\e[1m  [%s%d]\e[0m\n","监听端口:",LISTEN_PORT);
  printf("\e[36m\e[1m  [%s%d]\e[0m\n","Worker子进程数量:",PROCESS_NUM);
  printf("\e[36m\e[1m  [%s%d]\e[0m\n","单进程最大连接数:",MAX_CONNECTIONS);
  printf("\e[36m\e[1m  [%s%s]\e[0m\n","日志文件名:",LOG_NAME);
  
  printf("\e[38m\e[1m%s\e[0m\n","\n其中需要说明的是日志记录的功能，这个主要是用来简单查看并发量用的。");
  printf("\e[38m\e[1m%s\e[0m\n","不过多进程并发写同一个日志文件会造成性能的损耗，所以默认是关闭的,\n如果需要开启需要在程序启动的时候加上-r XX 参数。\n");
  printf("\e[36m\e[1m%s\e[0m\n","例如：./SmlHttp -r 20");
  printf("\e[33m\e[1m%s\e[0m\n","其中20代表记录20次，程序启动后每隔两秒记录一次。\n如果这个数是0，则会一直记录下去，间隔也是两秒。\n");
  
  printf("\e[38m\e[1m%s\e[0m\n","关于最大支持并发量：并没有做极限测试，因为本机只能模拟3000不到的并发量。");
  printf("\e[38m\e[1m%s\e[0m\n","这个微型Http Sever也只支持静态网页，请求方法只支持GET,因为本意是写个高\n并发服务器端，所以这里并没有继续完善。");
  
  printf("\e[35m\e[1m%s\e[0m\n","\n如果你发现了其中bug，欢迎联系我，DaterLove");
}

/*分析判断程序参数*/
void string_get_argments(int argc,char **argv,int *recordnum)
{
	/*如果没有提供参数*/
	if(argc==1)
	{
        //printf("没有参数\n");
		return;
	}
	/*开始判断参数*/
	int i;
	for(i=1;i<argc;i++)
	{
		if(argv[i][0]=='-')
		{
			switch(tolower(argv[i][1]))
			{
				case 'h':
					string_print_help();
					exit(0);
					break;				
				case 'r':
                        if(++i < argc)
    						*recordnum=atoi(argv[i]);
					break;
			}
	   }
    }
}
