/*====================================================
*   Copyright (C) 2018  All rights reserved.
*
*   文件名称：client.c
*   创 建 者：LH 863302850@qq.com
*   创建日期：2018年08月11日
*   描    述：
================================================================*/

#include <stdio.h>
#include "head.h"

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		printf("请输入端口号和IP\n");
		return -1;
	}

	int ret = 0;
	int ret2 = 0;
	char name[128] = {0};
	char values[512] = {0};
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0)
	{
		perror("sockfd");
		return -2;
	}

	struct sockaddr_in addr;
	addr.sin_family		= AF_INET;
	addr.sin_port 		= htons(atoi(argv[1]));
	addr.sin_addr.s_addr= inet_addr(argv[2]);
	socklen_t len = sizeof(addr);

	ret = connect(sockfd,(struct sockaddr *)&addr,len);
	if(ret < 0)
	{
		perror("connect");
		return -3;
	}

	while(1)
	{
		bzero(name,sizeof(name));
		printf("请输入要查的单词退出输入q-u-i-t:");
		scanf("%s",name);
		send(sockfd,name,sizeof(name),0);
		if(!strcmp(name,"q-u-i-t"))
		{
			printf("已和服务器断开链接\n");
			break;
		}
		recv(sockfd,values,sizeof(values),0);
		printf("values = %s\n",values);
	}

	close(sockfd);
    return 0;
}
