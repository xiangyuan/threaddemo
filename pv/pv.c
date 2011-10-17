/*
 * pv.c
 *
 *  Created on: 2011-10-17
 *      Author: liyajie
 */
#include <stdio.h>
#include "pv.h"

void initPv(PvData * container) {
	printf("the container %p\n",container);
	pthread_mutex_init(&container->mutexlock,NULL);
	pthread_cond_init(&container->empty,NULL);
	pthread_cond_init(&container->full,NULL);
	container->reaLoc = container->wriLoc = 0;
}
/**
 * 读取数据
 * @param container pvdatas
 * @return index of value
 */
int getPv(PvData *container) {
	int value = 0;
	pthread_mutex_lock(&container->mutexlock);
	if (container->wriLoc == container->reaLoc) {
		pthread_cond_wait(&container->empty,&container->mutexlock);
	}
	value = container->container[container->reaLoc];
	container->reaLoc ++;
	if (container->reaLoc >= BUFFER_SIZE) {
		container->reaLoc = 0;
	}
	pthread_cond_signal(&container->empty);//唤醒消费者
	pthread_mutex_unlock(&container->mutexlock);
	return (value);
}
/**
 * 写入数据
 * @param container集合
 * @param data
 */
void writePv(PvData *container,int data) {
	pthread_mutex_lock(&container->mutexlock);
	if ((container->wriLoc + 1) % BUFFER_SIZE == container->reaLoc) {
		pthread_cond_wait(&container->full,&container->mutexlock);
	}
	container->wriLoc ++;
	container->container[container->wriLoc] = data;

	if (container->wriLoc >= BUFFER_SIZE) {
		container->wriLoc = 0;
	}
	pthread_cond_signal(&container->full);//唤醒消费者
	pthread_mutex_unlock(&container->mutexlock);
}
/**
 *
 */
void destryPv(PvData * pv) {
	if (pv != NULL) {
		pthread_cond_destroy(&pv->empty);
		pthread_cond_destroy(&pv->full);
		pthread_mutex_destroy(&pv->mutexlock);
//		pthread_detach(pthread_self());
	}
}
