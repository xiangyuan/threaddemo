/*
 * threadpool.c
 *
 *  Created on: 2011-10-14
 *      Author: liyajie
 */
#include "threadpool.h"
#include <stdlib.h>
#include <assert.h>

static Pool *pool = NULL;

/**
 * the thread will invoke this method
 * @return
 */
extern void * thread_loop() {
	/*get the current thread's id*/
	printf("start the thread 0x%x\n", pthread_self());
	//get into a loop
	while (1) {
		while (pool->taskNum == 0 && !pool->isShutdown) {//have no task
			printf("thread 0x%x is waiting\n", pthread_self());
			//wait to add task
			pthread_cond_wait(&(pool->condLock), &(pool->isShutdown));
		}
		if (pool->isShutdown) {//关闭
			pthread_mutex_unlock(&(pool->lock));
			printf("thread 0x%x will exit\n", pthread_self());
			pthread_exit(NULL);
		}
		/*now is normal mode*/
		printf("thread 0x%x is starting to work\n", pthread_self());
		/*assert是调试的好帮手*/
		assert (pool->taskNum != 0);
		assert (pool->workers != NULL);
		//get a worker to run
		Worker *task = pool->workers;
		pool->taskNum--;
		pool->workers = task->next;
		pthread_mutex_unlock(&(pool->lock));

		//执行任务
		(*(task->proccess))(task->param);
		free(task);
		task = NULL;
	}
	pthread_exit(NULL);
}

void init(int maxnum) {
	if (pool == NULL) {
		pool = malloc(sizeof(Pool));
	}
	pthread_mutex_init(&pool->lock, NULL);
	pthread_cond_init(&pool->condLock, NULL);
	pool->maxSize = maxnum;
	pool->workers = NULL;
	pool->taskNum = 0;
	pool->isShutdown = FALSE;
	pool->threads = (pthread_t *) malloc(maxnum * sizeof(pthread_t));
	//创建线程
	int i;
	for (i = 0; i < maxnum; i++) {
		/*the third parameter is the thread will invoked method*/
		pthread_create(&(pool->threads[i]), NULL, thread_loop, NULL);
	}
}

/**
 * add a worker to the pool
 */
int add_worker(void * (*proccess)(void *), void *param) {
	Worker *worker = malloc(sizeof(Worker));
	worker->proccess = proccess;
	worker->param = param;
	worker->next = NULL;
	pthread_mutex_lock(&pool->lock);
	//头插法，添加一个新的
	worker->next = pool->workers;
	pool->workers = worker;
	assert(pool->workers);
	//完成后
	pool->taskNum++;//工作者加上１
	pthread_mutex_unlock(&pool->lock);
	//完成后，唤醒其它等待线程
	pthread_cond_signal(&pool->condLock);
	return TRUE;
}
/**
 * destroy the thread pool
 */
int destroy() {
	if (pool->isShutdown)
		return -1;
	pool->isShutdown = TRUE;//关闭，然后清除资源
	pthread_cond_broadcast(&pool->condLock);
	//然后清除所有的线程
	int i;
	for (i = 0; i < pool->maxSize; i++) {
		pthread_join(pool->threads[i], NULL);//等待线程执行完成
	}
	free(pool->threads);//清除所有的线程
	Worker * worker = NULL;
	while (worker != NULL) {
		worker = pool->workers;
		pool->workers = pool->workers->next;
		free(worker);
	}

	pthread_mutex_destroy(&pool->lock);
	pthread_cond_destroy(&pool->condLock);

	free(pool);
	pool = NULL;
	return TRUE;
}
