#include"ThreadPool.h"

int create_fd__()
{
	int ret = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
	if(ret < 0)
	{
		perror("eventfd failed\n");
		exit(EXIT_FAILURE);
	}
	else return ret;
}


class IgnoreSigPipe
{
public:
	IgnoreSigPipe()
    {
    	signal(SIGPIPE, SIG_IGN);
    }
};

IgnoreSigPipe initobj;


Httpserver::Httpserver(int num):Threadnums(num), threads(thread(Threadnums)), p_thread(new pthread_t[num]), robin(0)
{
	for(int i(0); i < Threadnums; ++i)
	{	
		//threads[i].set_read(std::bind(Httpserver::handle_read, this, std::placeholders::_1));
		threads[i].set_remove(std::bind(&Httpserver::removeconn, this, std::placeholders::_1));
		threads[i].set_run_in_main(std::bind(&Httpserver::addToIoloop, this, std::placeholders::_1));
		threads[i].set_set_time(std::bind(&Httpserver::set_time, this, std::placeholders::_1));
		//threads[i].set_write()
		threads[i].run(&p_thread[i]);
	}
}

void Httpserver::append(int fd)
{
	assert(http_map[fd] == nullptr);
	http_map[fd] = connection_ptr(new Http_c(fd));
	threads[robin].append(fd, std::weak_ptr<Http_c>(http_map[fd]));
	wakeup(robin++);
	robin %= Threadnums;
}


int Httpserver::conn_num()
{
	return http_map.size();
}



void Httpserver::removeconn(int fd)
{
	if(http_map.find(fd) != http_map.end())
	{
		http_map.erase(fd);
	}
}

void Httpserver::addToIoloop(const Func& foo)
{
	MutexlockGuard lock(mainlooplock);
	mainqueue.push_back(foo);
}

void Httpserver::wakeup(int idx)
{
	uint64_t one = 1;
	ssize_t n = write(threads[idx].wakeupfd, &one, sizeof(one));
	if(n != sizeof(one))
	{

		printf("%d thread wakeup() writes %d byte",n, idx);
	}
}

void Httpserver::IoLoop()
{
	MutexlockGuard lock(mainlooplock);
	for(auto foo: mainqueue)
	{
		foo();
	}
	mainqueue.clear();
}

Loop::Loop():epoll(my_epoll(200)), stop(false), wakeupfd(create_fd__()){}

Loop::~Loop()
{
	stop = true;
}

/*void Loop::set_write(cbfunction cb)
{
	handle_write = cb;
}
*/

void* start(void* item)
{
	Loop* loop = static_cast<Loop*>(item);
	loop->work();
	return NULL;
}


void Loop::append(int fd, Loop::connection_ptr ptr)
{
	MutexlockGuard  lock(cloop_lock);
	http_map[fd] = ptr;
	if(epoll.my_epoll_add(fd) < 0)
	{
		perror("epoll failed\n");
	}

}

void Loop::set_remove(const fdcbfunction &cb)
{
	remove = cb;
}

void Loop::run(pthread_t* t_id)
{
	td = t_id;
	pthread_create(td, NULL, start, (void*)this);
	//线程分离
}

void Loop::handle_event(int fd, __uint32_t event)
{
	MutexlockGuard  lock(cloop_lock);
	assert(http_map.find(fd) != http_map.end());
	auto connection = http_map[fd].lock();
	if(!connection)
	{
		//MutexlockGuard  lock(cloop_lock);
		http_map.erase(fd);
		epoll.my_epoll_del(fd);
	}
	else if(event & EPOLLIN)
	{
		//handle_read(connection);
		if(connection->work())
		{
			//添加30s的定时处理
			//printf(" set timer to %d\n", fd);
			runInmain(std::bind(set_time, fd));
			return;
		}
		else if(connection->need_write)
		{
		    //暂不考虑
		    return;
		}
		else
		{
			epoll.my_epoll_del(fd);
			http_map.erase(fd);
			runInmain(std::bind(remove, fd));
		}
	}
	else if(event & EPOLLOUT)
	{
		//handle_write(connection);
		return;
	}
}

void Loop::work()
{
	while(!stop)
	{
		int n = epoll.my_epoll_wait();
		if(n < 0)
		{
			if(errno != EAGAIN)
			{
				perror("epoll failed\n");
			}
		}
		for(int i(0); i < n; ++i)
		{
			int sockfd = epoll[i].data.fd;
			if(sockfd == wakeupfd)
			{
				continue;
			}
			else handle_event(sockfd, epoll[i].events);
		}
	}
}

void Httpserver::set_time(int fd)
{
	connection_ptr ptr = nullptr;
	if(http_map[fd] != nullptr)
	{
		ptr = std::move(http_map[fd]);
		http_map_w[fd] = ptr;
		http_map.erase(fd);
	}
	else if(http_map_w.find(fd) != http_map_w.end())
	{
		ptr = http_map_w[fd].lock();
	}
	if(ptr == nullptr)
	{
		http_map_w.erase(fd);
		return ;
	}
	MutexlockGuard lock(tqlock);
	tq.insert(30, std::move(ptr));
}

void Httpserver::handle_expired()
{
	MutexlockGuard lock(tqlock);
	tq.handle_expired();
}

