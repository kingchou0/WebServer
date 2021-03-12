cmake_minimum_required(VERSION 3.6)

project(WebServer)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FALGS -Wall -std=c++14 -lpthread)

link_libraries(pthread)

set(CMAKE_CXX_COMPLIER "g++")
aux_source_directory(. DIR_SRCS)

set(SOURCE_FILES
	main.cpp
	ThreadPool.cpp
	RequestData.cpp
	my_epoll.cpp
	TimerQueue.cpp
)

add_executable(webserver ${SOURCE_FILES})



