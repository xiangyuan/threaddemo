/*
 * simple_smtp.h
 *
 *  Created on: 2011-10-17
 *      Author: liyajie
 */
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>


#ifndef SIMPLE_SMTP_H_
#define SIMPLE_SMTP_H_

typedef int boolean;

#define SMPT_PORT 25

/**
 * 一个小对象
 */
typedef struct Transport Transport;

struct Transport {
	char * sender;
	char * receiver;
	char * smptserver;
	boolean (*sendMail)(Transport * email,char * subject,char * content);
};

/**
 * @param email
 */
void mail_init(Transport *email);
/**
 * free the memory of the transport
 */
void mail_free(Transport *email);


#endif /* SIMPLE_SMTP_H_ */
