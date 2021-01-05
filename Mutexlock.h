#include<pthread.h>
#include<assert.h>

class Mutexlock
{
public:
	Mutexlock():holder_(-1)
	{
		pthread_mutex_init(&mutex_, NULL);
	}
	~Mutexlock()
	{
		assert(holder_ == 0);
		pthread_mutex_destroy(&mutex_);
	}
	void lock()
	{
		pthread_mutex_lock(&mutex_);
	}
	void unlock()
	{
		pthread_mutex_unlock(&mutex_);
	}
	pthread_mutex_t* getMutex()
	{
		return &mutex_;
	}

private:
	pthread_mutex_t mutex_;
	pid_t holder_;
};

class MutexlockGuard
{
public:
	explicit MutexlockGuard(Mutexlock& mutex):mutex_(mutex)
	{
		mutex_.lock();
	}
	~MutexlockGuard()
	{
		mutex_.unlock();
	}
private:
	Mutexlock& mutex_;
};

class Condition
{
public:
	Condition(Mutexlock& mutex):mutex_(mutex)
	{
		pthread_cond_init(&pcond_, NULL);
	}

	~Condition()
	{
		pthread_cond_destroy(&pcond_);
	}

	void wait()
	{
		pthread_cond_wait(&pcond_, mutex_.getMutex());
	}
	void notify()
	{
		pthread_cond_signal(&pcond_);
	}
	void notifyall()
	{
		pthread_cond_broadcast(&pcond_);
	}



private:
	Mutexlock& mutex_;
	pthread_cond_t pcond_;
};