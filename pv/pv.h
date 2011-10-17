/*
 * pv.h
 *
 *  Created on: 2011-10-17
 *      Author: liyajie
 */
#include <pthread.h>

#ifndef PV_H_
#define PV_H_

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 12
#endif

typedef struct ShareData PvData;
/**
 * 共享数据操作区域
 */
struct ShareData {
	/*the datas*/
	int container[BUFFER_SIZE];
	int reaLoc;//读位置
	int wriLoc;//写位置
	/*保证只有一个线程操作数据区*/
	pthread_mutex_t mutexlock;
	/*条件锁，读或写操作标识*/
	pthread_cond_t empty;//空标识开始写入
	pthread_cond_t full;//full标识开读
};

void initPv(PvData * container);
/**
 * 读取数据
 * @param container pvdatas
 * @return index of value
 */
int getPv(PvData *container);
/**
 * 写入数据
 * @param container集合
 * @param data
 */
void writePv(PvData *container,int data);

void destryPv(PvData * pv);
#endif /* PV_H_ */
