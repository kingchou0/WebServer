#include "my_epoll.h"
#include<assert.h>
#include<stdio.h>


void setnonblocking(int fd)
{
	int old_option = fcntl(fd, F_GETFL);
	old_option |= O_NONBLOCK;
	fcntl(fd, F_SETFL, old_option);
} 

my_epoll::my_epoll(int max_n):max_num(max_n), events(new epoll_event[max_num])
{
	epollfd = epoll_create(max_num);
	assert(epollfd != -1);
}


my_epoll::~my_epoll(){delete [] events;}


int my_epoll::my_epoll_wait()
{
	int ret = epoll_wait(epollfd, events, max_num, -1);
	if(ret == -1)
	{
		if(errno != EINTR)
		{
			perror("epoll_wait\n");
		}
	}
	return ret;
}

epoll_event my_epoll:: operator[](int i)
{
	return events[i];
}

int my_epoll::my_epoll_add(int fd)
{
	epoll_event event;
	event.data.fd = fd;
	event.events = (EPOLLIN | EPOLLOUT | EPOLLET);
	int ret = epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
	assert(ret >= 0);
	setnonblocking(fd);
	return ret;
}

int my_epoll::my_epoll_del(int fd, __uint32_t events)
{
	epoll_event event;
	event.data.fd = fd;
	event.events = events;
	int ret = epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &event);
	if(ret < 0)
	{
		if(errno != ENOENT)
		{
			perror("epoll_delete");
		}
	}
	return ret;
}



int my_epoll::my_epoll_mod(int fd, __uint32_t events)
{
	epoll_event event;
	event.data.fd = fd;
	event.events |= events;
	int ret = epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
	assert(ret >= 0);
	return ret;
}
