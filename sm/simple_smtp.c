/*
 * simple_smtp.c
 *
 *  Created on: 2011-10-17
 *      Author: liyajie
 */
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "simple_smtp.h"

void diemsg(const char * msg) {
	perror(msg);
	exit(1);
}
/**
 *
 */
char *handle_mail_command(const char *command, const char *append) {
	return NULL;
}
/**
 * 发送邮件
 */
boolean send_mail(Transport * email, const char *subject, const char *content) {
	boolean isOk = 0;
	int sockb;
	struct sockaddr_in server;
	sockb = socket(AF_INET, SOCK_STREAM, 0);
	if (sockb < 0) {
		diemsg("socket create error!\n");
	}
	//清0
	memset(&server, 0, sizeof(struct sockaddr_in));
	struct hostent *host;
	printf("the sever ip %s\n", email->smptserver);
	host = gethostbyname(email->smptserver);
	struct in_addr inaddress;
	//将所在地址的值拷贝到in_addr数据中
	memcpy(&(inaddress.s_addr), host->h_addr, 4);
	printf("%s", inet_ntoa(inaddress));
	server.sin_family = AF_INET;
	printf("the port is %d\n", SMPT_PORT);
	server.sin_port = htons(SMPT_PORT);
	printf("%s", inet_ntoa(inaddress));
	server.sin_addr.s_addr = inet_addr(inet_ntoa(inaddress));
	printf("the saddr %d\n", server.sin_addr.s_addr);
	//	int re = bind(sockb,&server,sizeof(struct sockaddr_in));
	int re = connect(sockb, (struct sockaddr *) &server,
			sizeof(struct sockaddr_in));
	if (re < 0) {
		perror("socket bind error!\n");
	}
	char * buff = malloc(sizeof(char *));
	int len = recv(sockb, buff, sizeof(char *), 0);
	printf("the recv msg%s\n", buff);
	len = send(sockb, "helo", strlen("helo"), 0);
	send(sockb, "\r\n", strlen("\r\n"), 0);
	printf("the send len %d\n", len);
	printf("the buff %s\n", buff);
	len = recv(sockb, buff, 2048, 0);
	printf("the msg %s\n", buff);

	const char * sender = "mail from: <365283170@qq.com>";
	len = send(sockb, sender, strlen(sender), 0);
	send(sockb, "\r\n", strlen("\r\n"), 0);
	char senbuf[2048];
	len = recv(sockb, senbuf, 2048, 0);
	printf("the sender msg %s\n", senbuf);

	char rebuf[2048];
	const char * receiver = "rcpt to: <liyj2@wondershare.cn>";
	len = send(sockb, receiver, strlen(receiver), 0);

	send(sockb, "\r\n", strlen("\r\n"), 0);
	len = recv(sockb, rebuf, 2048, 0);
	printf("the receiver msg %s\n", rebuf);

//	const char * two = "rcp to: <liyj2@wondershare.cn>";
//	len = send(sockb, two, strlen(two), 0);
//
//	send(sockb, "\r\n", strlen("\r\n"), 0);
//	len = recv(sockb, rebuf, 2048, 0);
//	printf("the receiver msg %s\n", rebuf);

	len = send(sockb, "data", strlen("data"), 0);
	send(sockb, "\r\n", strlen("\r\n"), 0);
	printf("the send len %d\n", len);

	len = send(sockb, "suject:你好", strlen("suject:你好"), 0);
	send(sockb, "\r\n", strlen("\r\n"), 0);
	printf("the send len %d\n", len);

	send(sockb, subject, strlen(subject), 0);
	send(sockb, "\r\n", strlen("\r\n"), 0);
	send(sockb, content, strlen(content), 0);
	send(sockb, "\r\n", strlen("\r\n"), 0);
	send(sockb, ".", strlen("."), 0);
	send(sockb, "\r\n", strlen("\r\n"), 0);
	send(sockb, "rset", strlen("rset"), 0);
	send(sockb, "\r\n", strlen("\r\n"), 0);
	send(sockb, "quit", strlen("quit"), 0);
	send(sockb, "\r\n", strlen("\r\n"), 0);
	isOk = 1;

	printf("the isOk is %d\n", isOk);

	close(sockb);
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
