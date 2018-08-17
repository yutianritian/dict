/*====================================================
*   Copyright (C) 2018  All rights reserved.
*
*   文件名称：01-1s.c
*   创 建 者：LH 863302850@qq.com
*   创建日期：2018年08月01日
*   描    述：
================================================================*/

#include <stdio.h>
#include "head.h"

pthread_t tid = 0;

void *fun(void *arg)
{
	sqlite3 *db = NULL;
	int ret = sqlite3_open("./dict.db",&db);
	if(ret < 0)
	{
		perror("sqlite3 open");
		exit(0);
	}
	int ret2 = 0;
	int ret3 = 0;
	int row = 0,col = 0;
	char sql[128] = {0};
	char **rest = NULL;
	char name[128] = {0};
	char values[512] = {0};

	long connfd = (long)arg;
	while(1)
	{	
		bzero(values,sizeof(values));
		ret2 = recv(connfd,name,sizeof(name),0);
		if(ret2 <= 0)
		{
			break;
		}
		if(!strcmp(name,"q-u-i-t"))
		{
			printf("客户端已退出\n");
			sqlite3_close(db);
			close(connfd);
			pthread_exit(NULL);
			break;
		}
		sprintf(sql,"select * from word where name = '%s'",name);
		ret3 = sqlite3_get_table(db,sql,&rest,&row,&col,NULL);
		if(ret3 < 0)
		{
			perror("ret2");
			sqlite3_close(db);
			exit(0);
		}
		if(row >= 1)
		{
			strcpy(values,rest[5]);
			send(connfd,values,sizeof(values),0);
		}
		else
		{
			
			char buff[64] = {"单词错误,请输入正确的单词\n"};
			send(connfd,buff,sizeof(buff),0);
		}

	}

}

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		printf("请输入端口号和IP:\n");
		return -1;
	}
	int ret = 0;
	long connfd = 0;
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0)
	{
		perror("socket");
		return -1;
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port   = htons(atoi(argv[1]));
	addr.sin_addr.s_addr = inet_addr(argv[2]);
	socklen_t len = sizeof(addr);

	ret = bind(sockfd,(struct sockaddr *)&addr,len);
	if(ret < 0)
	{
		perror("bind");
		return -2;
	}	
	listen(sockfd,10000);
	
	struct sockaddr_in cliaddr;
	socklen_t clilen = sizeof(cliaddr);


	while(1)
	{
		connfd = accept(sockfd,(struct sockaddr *)&cliaddr,&clilen);
		if(connfd < 0)
		{
			perror("connfd");
			return -3;
		}

		if(connfd > 0)
		{
			pthread_create(&tid,NULL,fun,(void *)connfd);
			printf("IP = %s ",inet_ntoa(cliaddr.sin_addr));
			printf("port = %d 已接入\n",ntohs(cliaddr.sin_port));
		}
	}

	close(sockfd);

    return 0;
}
