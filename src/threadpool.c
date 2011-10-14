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

void init(int maxnum, thread_routine fun_method) {
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
		pthread_create(&(pool->threads[i]), NULL, fun_method, NULL);
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
 * @param pool
 */
int destroy(Pool *pool) {
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
