#include "RequestData.h"
#include "my_epoll.h"
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
#include<assert.h>
#include<stdio.h>
#include<unordered_map>
#include<unistd.h>




#define MAX_CONECT_SIZE 100

int socket_bind_listen(struct sockaddr_in* address, char* arg_1, char* arg_2)
{
	int port = atoi(arg_2);
	int listenfd = socket(PF_INET, SOCK_STREAM, 0);
	assert(listenfd >= 0);
	bzero(address, sizeof(*address));

	address->sin_family = AF_INET;
	inet_pton(AF_INET, arg_1, &address->sin_addr);
	address->sin_port = htons(port);

	int ret = bind(listenfd, (struct sockaddr*)address, sizeof(*address));
	assert(ret >= 0);

	ret = listen(listenfd, 5);
	assert(ret != -1);

	return listenfd;
}





int main(int argc, char** arg)
{
	if(argc < 3)
	{
		printf("need ip and port!!");
		return 0;
	}


	struct sockaddr_in address;
	int listenfd = socket_bind_listen(&address, arg[1], arg[2]);

	std::unordered_map<int, Http_c*> http_map;

	my_epoll server(30);

	if(server.my_epoll_add(listenfd) < 0)
	{
		perror("epoll_add fd error\n");
		return 1;
	}

	while(true)
	{
		int ret = server.my_epoll_wait();
		for(int i(0); i < ret; ++i)
		{
			if(server[i].data.fd == listenfd)
			{
				struct sockaddr_in cilent_address;
				socklen_t len = sizeof(cilent_address);
				int confd = accept(listenfd, (struct sockaddr*)&cilent_address, &len);

				if(confd < 0)
				{
					perror("accept error\n");
					continue;
				}

				if(http_map.size() >= MAX_CONECT_SIZE)
				{
					std::string info = "server busy!!\n";
					send(confd, info.c_str(), sizeof(info.c_str()), 0);
					close(confd);
					continue;  
				}
				else
				{
					http_map[confd] = new Http_c(confd);
					server.my_epoll_add(confd);
					printf("here comes a new connection\n");
				}
			}
			else if(server[i].events & EPOLLIN)
			{
				int sockfd = server[i].data.fd;
				printf("one ask\n");
				if(!http_map[sockfd]->work())
				{

					printf("bad ask\n");
					delete http_map[sockfd];
					http_map.erase(sockfd);
					server.my_epoll_del(sockfd);
					close(sockfd);
				}
				printf("success answear\n");

			}
		}
	}
	for(auto it = http_map.begin(); it != http_map.end(); ++it)
	{
		delete it->second;
	}

}
