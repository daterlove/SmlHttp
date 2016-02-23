/*************************************************************************
    > File Name: shttp_file.c
    > Author: 浅握双手
    > ---------------- 
    > Created Time: 2016年02月24日 01时02分48秒 CST
 ************************************************************************/

#include"common.h"
int file_write(char *path,char *szbuf)
{
	FILE *fp=fopen(path,"a+");
	if(fp==NULL)
	{
		printf("文件打开出错\n");
		return -1;
	}
	//printf("文件打开成功\n");
	//写文件
	fputs(szbuf,fp);
	fclose(fp);
    return 0;
}