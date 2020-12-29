#ifndef MY_EPOLL
#define MY_EPOLL
#include<sys/epoll.h>
#include<cstdlib>
#include<fcntl.h>

#define MAX_EVENT_NUMBER

void setnonblocking(int);

class my_epoll
{
public:
	my_epoll(int);
	~my_epoll();
	epoll_event operator[](int i);

	int my_epoll_wait();
	int my_epoll_add(int fd);
	int my_epoll_del(int fd, __uint32_t events = EPOLLIN | EPOLLOUT);
	int my_epoll_mod(int fd, __uint32_t events);
private:
	int epollfd;
	int max_num;
	epoll_event* events;
};


#endif



