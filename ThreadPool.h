#ifndef THREADPOOL
#define	THREADPOOL
#include<queue>
#include<assert.h>
#include<stdio.h>
#include"RequestData.h"

#include"Mutexlock.h"

#define MAX_QUEUE_SIZE 1000

template <typename T>
class Threadpool
{
public:
	static void* work(void*);
	Threadpool(int);
	T* dequeue();
	void pushback(T* quest);
	void run();
	~Threadpool();
private:

	void worker();

	bool m_stop;
	std::queue<T*> q;
	Mutexlock mutex;
	Condition cond;
	pthread_t* p_array;
};

template <typename T>
void* Threadpool<T>::work(void* arg)
{
	auto elem = static_cast<Threadpool<T>*>(arg);
	elem->run();
	return elem;
}



template <typename T>
T* Threadpool<T>::dequeue()
{
	MutexlockGuard lock(mutex);
	while(q.empty())
	{
		cond.wait();
	}
	assert(!q.empty());
	T* ret = q.front();
	q.pop();
	return ret;
}


template <typename T>
void Threadpool<T>::run()
{
	while(!m_stop)
	{
		T* quest = dequeue();
		if(!quest->work())
		{
			printf("fucking wrong!!!\n");
		}
	}
}



template <typename T>
Threadpool<T>::Threadpool(int num):cond(mutex), m_stop(false)
{
	p_array = new pthread_t[num];

	for(int i(0); i < num; ++i)
	{
		pthread_create(&p_array[i], NULL, work, this);
		assert(pthread_detach(p_array[i]) == 0);
	}
}



template <typename T>
Threadpool<T>::~Threadpool()
{
	delete [] p_array;
	m_stop = true;
}

template <typename T>
void Threadpool<T>::pushback(T* quest)
{
	MutexlockGuard lock(mutex);
	if(q.size() > MAX_QUEUE_SIZE)
	{
		printf("There is too much quest in queue!!\n");
		return;
	}
	q.push(quest);
	cond.notify();
}


#endif

