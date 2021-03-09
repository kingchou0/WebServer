#ifndef Httpserver__
#define	Httpserver__

#include<signal.h>
#include<map>
#include<vector>
#include<assert.h>
#include<errno.h>
#include<utility>
#include<memory>
#include<stdio.h>
#include<sys/eventfd.h>
#include<functional>
#include<unistd.h>
#include"RequestData.h"
#include"my_epoll.h"
#include"Mutexlock.h"


//typedef std::vector<struct epoll_event> event_list;
//typedef std::vector<int> fd_list;

typedef std::shared_ptr<Http_c> connection_ptr;
typedef std::function<void(connection_ptr)> cbfunction;
typedef std::function<void(int)> fdcbfunction;
typedef std::function<void()> Func;

class Loop
{
public:
	typedef std::weak_ptr<Http_c> connection_ptr;
	typedef std::map<int, connection_ptr> conn_map;
	typedef std::function<void(Func)> FFunc;
	Loop();
	~Loop();
	void run(pthread_t*);
	void work();
	void append(int, connection_ptr);
	void set_remove(const fdcbfunction&);

	int wakeupfd;
	void set_run_in_main(const FFunc& foo){runInmain = foo;}
	//void focus(connection_ptr);
	//void set_read(cbfunction);
	//void set_write(cbfunction);
private:

	

	pthread_t* td;

	my_epoll epoll;
	
	FFunc runInmain;
	
	bool stop;
	
	conn_map http_map;
	
	void handle_event(int, __uint32_t);
	
	fdcbfunction remove;

	Mutexlock cloop_lock;



	//cbfunction handle_write;
};


class Httpserver
{
public:
	typedef std::vector<Loop> thread;
	typedef std::shared_ptr<Http_c> connection_ptr;
	typedef std::map<int, connection_ptr> conn_map;
	
public:
	Httpserver(int);
	~Httpserver(){}
	void append(int);
	int conn_num();
	std::vector<Func> mainqueue;
	void IoLoop();
private:
	Mutexlock mainlooplock;
	int Threadnums;
	int robin;
	thread threads;
	pthread_t* p_thread;
	conn_map http_map; 

	void wakeup(int);
	void addToIoloop(const Func&);
	void removeconn(int);
	void handle_read(connection_ptr);


};



#endif

//还得写连接分配
