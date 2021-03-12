#include"TimerQueue.h"




TimerQueue::TimerQueue(){}

TimerQueue::~TimerQueue(){}

void TimerQueue::insert(int time_, con_ptr&& ptr)
{
	//加锁
//	MutexlockGuard lock(q_lock);
	tmq.push({time_, ptr});
}
void TimerQueue::handle_expired()
{
	update_time();
//	MutexlockGuard lock(q_lock);
	while(!tmq.empty())
	{
		auto& tp = tmq.top();
		if(tp.first <= now)
		{
			//printf("delete timenode %d\n", (tp.second)->sockfd);
			tmq.pop();
		}
		else
		{
			break;
		}
	}
}

void TimerQueue::update_time()
{
	struct timeval tm;
	gettimeofday(&tm, NULL);
	now = tm.tv_sec;
}


