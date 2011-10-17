/*
 * test.c
 *
 *  Created on: 2011-10-14
 *      Author: liyajie
 */
#include <unistd.h>
#include "pv/pv.h"
#include "src/threadpool.h"
/**
 * 客户定制的线程执行方法,
 * 取出一个线程进行执行
 * @param param
 * @return value
 */
void * client_thread_fun(void * param) {
	printf("threadid is 0x%x, working on task %d\n", pthread_self(),
			*(int *) param);
	sleep(1);/*休息一秒，延长任务的执行时间*/
	return NULL;
}

void * productor(PvData *data) {
	int i;
	for (i = 0; i < 100; i++) {
		writePv(data,i);
		printf("the productor is running %d\n",i);
	}
	return NULL;
}

void * consumer(PvData *container) {
	int i;
	int rev;
	while (1) {
		rev = getPv(container);
		printf("the consumer is running %d\n",rev);
	}
	return NULL;
}
int main(int arg, char *ags[]) {

	//	init(3);//初始化完成后，创建任务
	//	int i;
	//	for (i = 0; i < 5; i++) {
	//		add_worker(client_thread_fun, &i);
	//	}
	//
	//	sleep(3);
	//	destroy();


	/*now is the pv test
	 * */
	PvData data;
	pthread_t a,b;
	int retval;
	initPv(&data);
	/* 创建生产者和消费者线程*/
	pthread_create(&a, NULL, productor, &data);
	pthread_create(&b, NULL, consumer, &data);
	/* 等待两个线程结束*/
	pthread_join(a, &retval);
	pthread_join(b, &retval);

	destryPv(&data);
	return 0;
}
