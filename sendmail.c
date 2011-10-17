/*
 * sendmail.c
 *
 *  Created on: 2011-10-17
 *      Author: liyajie
 */
#include "sm/simple_smtp.h"
#include <stdlib.h>
#include <netdb.h>

int main(int argv, char * args[]) {

	const char* sender = "365283170@qq.com";
	const char *receiver = "liyj2@wondershare.cn";
	const char *server = "smtp.wondershare.cn";
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
	boolean isOk = (*(transport->sendMail))(transport,"test","hello are you ok?");
	printf("is ok %d\n",isOk);

	mail_free(transport);
	return 0;
}
