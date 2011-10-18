/*
 * sendmail.c
 *
 *  Created on: 2011-10-17
 *      Author: liyajie
 */
#include "sm/simple_smtp.h"
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>

int main(int argv, char * args[]) {

	const char* sender = "365283170@qq.com";
	const char *receiver = "liyj2@wondershare.cn";
	const char *server = "smtp.wondershare.cn";
//	struct sockaddr_in address;
//	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
//	address.sin_family = AF_INET;
//	address.sin_addr.s_addr = inet_addr("192.168.10.8");
//	printf("the saddr %d\n",address.sin_addr.s_addr);
//	address.sin_port = htons(25);
//	int len = sizeof(address);
//	int result = connect(sockfd,  (struct sockaddr *)&address, len);
//	if(result == -1){
//	    perror("oops: client1");
//	    return 1;
//	} else {
//		printf("the socket is create ok!\n");
//	}
//	struct hostent *host = gethostbyname(server);
//	struct in_addr inaddress;
//	//将所在地址的值拷贝到in_addr数据中
//	memcpy(&inaddress.s_addr, host->h_addr, 4);
//	printf("%s", inet_ntoa(inaddress));
//	printf("%s",inet_ntoa(host->h_addr));
	Transport * transport = NULL;
	transport = malloc(sizeof(Transport));
	transport->sender = strdup(sender);
	transport->receiver = strdup(receiver);
	transport->smptserver = strdup(server);

	mail_init(transport);
	boolean isOk = (*(transport->sendMail))(transport,"美丽","人通过美丽");
	printf("is ok %d\n",isOk);

	mail_free(transport);
	return 0;
}
