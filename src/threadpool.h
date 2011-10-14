/*
 * threadpool.h
 *
 *  Created on: 2011-10-14
 *      Author: liyajie
 */

#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <pthread.h>

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

typedef struct Thread_Worker Worker;
typedef struct Thread_Pool Pool;//线程pool
/*
 * every thread will run this function
 * @param parameters to use
 * @return the value to return
 */
typedef void * (*thread_routine)(void *);

/*thread object -> next -> next all
 *like this
 */
struct Thread_Worker {
	/*handle function param*/
	void * (*proccess)(void *param);
	void * param;
	Worker *next;
};

struct Thread_Pool {
	pthread_mutex_t lock;//lock of the mutex
	pthread_t *threads;//所有的线程
	Worker * workers;//工作者,所有等待的线程
	int isShutdown;
	pthread_mutex_t mutexLock;
	pthread_cond_t condLock;
	int maxSize;
	int taskNum;//任务数
};
/**
 * 初始化线程pool
 * @param maxnum the max size of the pool
 * @param client implements the function method
 */
void init(int maxnum,thread_routine spefunction);
/**
 * add a worker to the pool
 */
int add_worker(void * (*proccess)(void *),void *param);
/**
 * destroy the thread pool
 * @param pool
 */
int destroy(Pool *pool);

#endif /* THREADPOOL_H_ */
