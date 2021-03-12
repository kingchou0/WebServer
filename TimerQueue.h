#ifndef TIMEQUEUE
#define TIMEQUEUE


#include<utility>
#include<memory>
#include<queue>

//#include"Mutexlock.h"
#include<sys/time.h>
#include"RequestData.h"
#include<vector>

using con_ptr = std::shared_ptr<Http_c>;
using q_elem = std::pair<int, con_ptr>;

struct compair{
	bool operator()(const q_elem& a, const q_elem& b)
	{
		return a.first < b.first;
	}
};

class TimerQueue
{
public:
	TimerQueue();
	~TimerQueue();

	void insert(int, con_ptr&&);
	void handle_expired();
	void update_time();

private:
	std::priority_queue<q_elem, std::vector<q_elem>, compair> tmq;
//	Mutexlock q_lock;

	int now;
};



#endif