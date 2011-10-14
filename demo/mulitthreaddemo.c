/*
 * mulitthreaddemo.c
 *
 *  Created on: 2011-10-14
 *      Author: liyajie
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

pthread_t t[2];

pthread_mutex_t lock;

int i;
int number;

int *retval;

void die(int re, const char * msg) {
	if (re != 0) {
		perror(msg);
	}
}
void *func1(void * arg) {
	printf("the func1 thread %s\n", (char *) arg);
	for (i = 0; i < 10; i++) {
		printf("the number = %d\n", number);
		pthread_mutex_lock(&lock);
		number++;
		pthread_mutex_unlock(&lock);
		sleep(2);
	}
	pthread_exit(retval);
	return NULL;
}

void *func2(void * arg) {
	printf("the func2 thread %s\n", (char *) arg);
	for (i = 0; i < 10; i++) {
		printf("the number = %d\n", number);
		pthread_mutex_lock(&lock);
		number++;
		pthread_mutex_unlock(&lock);
		sleep(3);
	}
	pthread_exit(retval);
	return NULL;
}

void thread_create() {
	memset(&t, 0, sizeof(t));
	int re = pthread_create(&t[0], NULL, func1, "first thread");
	die(re, "the first thread create error\n");
	re = pthread_create(&t[1], NULL, func2, "second thread");
	die(re, "the second thread create error\n");
}
/**
 * 线程等待
 */
void thread_wait() {
	if (t[0] != 0) {
		//printf("the thread1 is started %d\n", *retval);
		pthread_join(t[0], NULL);
	}
	if (t[1] != 0) {
		//printf("the thread2 is started %d\n", *retval);
		pthread_join(t[1], NULL);
	}
}
//int main(int arg, char *agvs[]) {
//	pthread_mutex_init(&lock, NULL);
//	printf("the main thread is running\n");
//	thread_create();
//	thread_wait();
//
//	printf("the main thread is ok\n");
//	return 0;
//}
