#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/mman.h>
#include "RequestData.h"

	static std::unordered_map<std::string, std::string> mime_map = 
	{
		{".html", "text/html"},
        {".xml", "text/xml"},
        {".xhtml", "application/xhtml+xml"},
        {".txt", "text/plain"},
        {".rtf", "application/rtf"},
        {".pdf", "application/pdf"},
        {".word", "application/msword"},
        {".png", "image/png"},
        {".gif", "image/gif"},
        {".jpg", "image/jpeg"},
        {".jpeg", "image/jpeg"},
        {".au", "audio/basic"},
        {".mpeg", "video/mpeg"},
        {".mpg", "video/mpeg"},
        {".avi", "video/x-msvideo"},
        {".gz", "application/x-gzip"},
        {".tar", "application/x-tar"},
        {".css", "text/css"},
        {"", "text/plain"},
        {"default","text/plain"}
	};


static const char* basepath = "/home/re/codes/myweb_1.0";

void Http_c::setnonblocking(int fd)
{
	int old_option = fcntl(fd, F_GETFL);
	old_option |= O_NONBLOCK;
	fcntl(fd, F_SETFL, old_option);
} 

Http_c::Http_c(int fd)
{
	sockfd = fd;
	setnonblocking(fd);
	keep_alive = CLOSE;
	check_idx = read_idx = write_idx = 0;
	state = P_REQUEST;	
	mime_type = "default";
}

Http_c::~Http_c(){}




Http_c::LINE_RS Http_c::parse_line()
{
	char temp;
	for(; check_idx < read_idx; ++check_idx)
	{
		temp = r_buf[check_idx];
		if(temp == '\r')
		{
			if(check_idx + 1 == read_idx)
			{
				return LINE_OP;
			}
			else if(r_buf[check_idx+1] == '\n')
			{
				r_buf[check_idx++] = '\0';
				r_buf[check_idx++] = '\0';
				return LINE_OK;
			}
			return LINE_ER;
		}
		/* 没有写是\n的情况，可能会有bug，mark一下*/
	}
	return LINE_ER;
}

Http_c::HTTP_CODE Http_c::parse_request(char* text)
{
	url = strpbrk(text, " \t");
	if(!url)
	{
		return BAD_REQUEST;
	}
	*url++ = '\0';
	if(strcasecmp(text, "GET") == 0)
	{
		method = GET;
	}
	else
	{
		return BAD_REQUEST;
	}
	url += strspn(url, " \t");
	char* version = strpbrk(url, " \t");
	if(!version)
	{
		return BAD_REQUEST;
	}
	*version++ = '\0';
	version += strspn(version, " \t");
	if(strcasecmp(version, "HTTP/1.1") != 0)
	{
		return BAD_REQUEST;
	}
	if(strncasecmp(url, "http://", 7) == 0)
	{
		url += 7;
		url = strchr(url, '/');
	}
	else if(strncasecmp(url, "/", 1) == 0)
	{
	}
	else
	{
		return BAD_REQUEST;
	}
	if(!url || *url != '/')
	{
		return BAD_REQUEST;
	}
	state = P_HEADER;
	return NO_REQUEST;
}



Http_c::HTTP_CODE Http_c::parse_head(char* text)
{
	if(*text == '\0')
	{
		if(method == GET)
		{
			return GET_REQUEST;
		}
		return NO_REQUEST;
	}
	char key[100];
	char val[100];
	sscanf(text, "%[^:]: %s", key, val);
	headers[key] = val;
	return NO_REQUEST;
}



Http_c::HTTP_CODE Http_c::request_read()
{
	int ret = recv(sockfd, r_buf, BUFFERSIZE, 0);

	if(ret <= 0)
	{
		if(ret < 0 )
		{
			if(errno == EAGAIN)
			{
				return NO_REQUEST;
			}
			printf("read error\n");
		}
		state = P_CONTENT;
		return NO_REQUEST;
	}
	read_idx += ret;
	LINE_RS line_state;
	HTTP_CODE parse_ret;
	int start = check_idx;
	while((line_state = parse_line()) == LINE_OK)
	{
		char* temp = r_buf + start;
		start = check_idx;

		switch(state)
		{
			case P_REQUEST:
			{
				parse_ret = parse_request(temp);
				if(parse_ret == BAD_REQUEST)
				{
					return BAD_REQUEST;
				}
				state = P_HEADER;
				break;
			}
			case P_HEADER:
			{
				parse_ret = parse_head(temp);
				if(parse_ret == BAD_REQUEST)
				{
					return BAD_REQUEST;
				}
				else if(parse_ret == GET_REQUEST)
				{
					return GET_REQUEST;
				}
				break;
			}
			case P_CONTENT:
			{
				parse_ret = parse_body();
				if(parse_ret == GET_REQUEST)
				{
					return GET_REQUEST;
				}
				return BAD_REQUEST;
				break;
			}
		}
	}
	if(line_state == LINE_OP)
	{
		return NO_REQUEST;
	}
	return BAD_REQUEST;
}


int Http_c::work()
{
	HTTP_CODE ret = request_read();
	if(ret == BAD_REQUEST)
	{
		return CLOSE;
	}
	else if(ret == GET_REQUEST)
	{
		response = do_request();
		if(response == BAD_REQUEST)
		{
			return CLOSE;
		}
	}
	else return CLOSE;
	if(send_response())
	{
		reset();
		return keep_alive;
	}
	else return CLOSE;
}

Http_c::HTTP_CODE Http_c::do_request()
{

	sprintf(w_buf, "%s\t", "HTTP/1.1");
	write_idx = 9;

	
	char file[strlen(url) + strlen(basepath) + 1];

	strcpy(file, basepath);
	strcat(file, url);
	if((headers.find("Connection") != headers.end())&&(headers["Connection"] == "keep-alive"))
	{
		keep_alive = KEEP_ALIVE;
	}

	if(stat(file, &filestat) < 0)
	{
		sprintf(w_buf+write_idx, "%s", "404 Not Found\r\n");
		write_idx += 17;
		filepath = std::string(basepath) + "/page/404.html";
		return NO_RESOURCE;
	}

	if(!(filestat.st_mode & S_IROTH))
	{
		sprintf(w_buf+write_idx, "%s", "403 ForBidden\r\n");
		write_idx += 15;
		filepath = std::string(basepath) + "/page/403.html";
		return FORBIDDEN_REQUEST;
	}
	sprintf(w_buf + write_idx, "%s", "200 OK\r\n");
	write_idx += 8;
	filepath = std::string(file);
	/*get mime type*/
	int sear;
	if((sear = filepath.find('.')) == std::string::npos)
	{
		mime_type = "default";
	}
	else
	{
		mime_type = filepath.substr(sear);
		if(mime_map.find(mime_type) == mime_map.end())
		{
			mime_type = "default";
		}
	}

	return FILE_REQUEST;
}



bool Http_c::send_response()
{
	int fd = open(filepath.c_str(), O_RDONLY);
	if( fd < 0 )
	{
		sprintf(w_buf+write_idx, "Content-length: %d\r\n\r\n%s", strlen(internal_error), internal_error);
		return false;
	}
	void* m_file = mmap(NULL, filestat.st_size, PROT_READ, MAP_PRIVATE, fd, 0); /*忘记咋用了，看书*/
	close(fd);
	sprintf(w_buf + write_idx, "Server: KINGCHOU's WebServer\r\n");
	write_idx += 30;
	sprintf(w_buf + write_idx, "Content-type: %s\r\n", mime_map[mime_type].c_str());
	write_idx += (16+(mime_map[mime_type]).size());
	sprintf(w_buf + write_idx, "Content-length: %d\r\n\r\n", filestat.st_size);

	if(send(sockfd, w_buf, BUFFERSIZE, 0) < 0)
	{
		return false;
	}
	if(send(sockfd, m_file, filestat.st_size, 0)< 0)
	{
		return false;
	}
	return true;
}

Http_c::HTTP_CODE Http_c::parse_body()
{
	return BAD_REQUEST;
}

void Http_c::reset()
{
	read_idx = write_idx = check_idx = 0;
}
