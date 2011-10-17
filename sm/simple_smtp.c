/*
 * simple_smtp.c
 *
 *  Created on: 2011-10-17
 *      Author: liyajie
 */
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include "simple_smtp.h"

void diemsg(const char * msg) {
	perror(msg);
	exit(1);
}
/**
 * 发送邮件
 */
boolean send_mail(Transport * email,const char *subject,const char *content) {
	boolean isOk = 0;
	int sockb;
	struct sockaddr_in server;
	sockb = socket(AF_INET,SOCK_STREAM,0);
	if (sockb < 0) {
		diemsg("socket create error!\n");
	}
	//清0
	memset(&server,0,sizeof(struct sockaddr_in));
	struct hostent *host;
	printf("the sever ip %s\n",email->smptserver);
	host = gethostbyname(email->smptserver);
	struct in_addr inaddress;
	//将所在地址的值拷贝到in_addr数据中
	memcpy(&(inaddress.s_addr), host->h_addr, 4);
	printf("%s", inet_ntoa(inaddress));
	server.sin_family = AF_INET;
	server.sin_port = htonl(SMPT_PORT);
	printf("%s",inet_ntoa(inaddress));
	server.sin_addr.s_addr = inet_addr(inet_ntoa(inaddress));
	int re = bind(sockb,&server,sizeof(struct sockaddr));
	if (re < 0) {
		perror("socket bind error!\n");
	}
	return (isOk);
}

/**
 * @param email
 */
void mail_init(Transport *email) {
	if (email == NULL) {
		perror("can't initial it \n");
	}
	email->sendMail = send_mail;
}
/**
 * free the memory of the transport
 */
void mail_free(Transport *email) {
	if (email != NULL) {
		if (email->sender) {
			free(email->sender);
		}
		if (email->receiver) {
			free(email->receiver);
		}
		if (email->smptserver) {
			free(email->smptserver);
		}
		free(email);
		email = NULL;
	}
}
