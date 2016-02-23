/*************************************************************************
    > File Name: shttp_string.c
    > Author: 浅握双手
    > ---------------- 
    > Created Time: 2016年02月24日 01时07分38秒 CST
 ************************************************************************/

#include"common.h"
void string_print_help()
{
    printf("显示帮助\n");
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
