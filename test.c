/*
 * test.c
 *
 *  Created on: 2011-10-14
 *      Author: liyajie
 */
#include "src/threadpool.h"
/**
 * 客户定制的线程执行方法,
 * 取出一个线程进行执行
 * @param param
 * @return value
 */
void * client_thread_fun(void * param) {

	return NULL;
}

int main(int arg,char *ags[]) {

	init(3,client_thread_fun);
	return 0;
}
