#ifndef REQUEST
#define REQUEST

#define CLOSE 0
#define KEEP_ALIVE 1

#define BUFFERSIZE 2048


#include<memory>
#include<errno.h>
#include<sys/stat.h>
#include<unordered_map>
#include<string.h>
#include<string>
#include<iterator>




class Http_c
{
public:
	enum HTTP_CODE
	{
		NO_REQUEST,        BAD_REQUEST,
		GET_REQUEST,       NO_RESOURCE,
		FORBIDDEN_REQUEST, FILE_REQUEST,
		INTERMAL_ERROR,	   CLOSED_CONNECTION
	};

	enum METHOD
	{
		GET, HEAD, POST, PUT, DELETE
	};

	enum P_STATE
	{
		P_REQUEST, P_HEADER, P_CONTENT
	};

	enum LINE_RS
	{
		LINE_OK, LINE_ER, LINE_OP
	};

	const char* internal_error = "oop, internal error!!!";
	

	std::unordered_map<std::string, std::string> headers;


	Http_c(int fd);
	~Http_c();

	int work();
	bool need_write;
private:


	HTTP_CODE do_request();
	HTTP_CODE request_read();
	bool send_response();

	void setnonblocking(int);
	void reset();

	int ssend(int fd, void* buf, size_t len, int flags);

	

	bool keep_alive;
	int sockfd;
	char r_buf[BUFFERSIZE];
	char w_buf[BUFFERSIZE];
	
	int check_idx;
	int read_idx;
	int write_idx;
	char* url;
	struct stat filestat;
	METHOD method;
	P_STATE state;


	std::string filepath;
	std::string mime_type;


	LINE_RS parse_line();
	HTTP_CODE parse_request(char*);
	HTTP_CODE parse_head(char*);
	HTTP_CODE parse_body();

	HTTP_CODE response;
	void* m_file;


};




#endif

