# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/hgfs/we/myweb_1.0

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/hgfs/we/myweb_1.0

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/local/bin/cmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /mnt/hgfs/we/myweb_1.0/CMakeFiles /mnt/hgfs/we/myweb_1.0/CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /mnt/hgfs/we/myweb_1.0/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named webserver

# Build rule for target.
webserver: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 webserver
.PHONY : webserver

# fast build rule for target.
webserver/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/webserver.dir/build.make CMakeFiles/webserver.dir/build
.PHONY : webserver/fast

RequestData.o: RequestData.cpp.o

.PHONY : RequestData.o

# target to build an object file
RequestData.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/webserver.dir/build.make CMakeFiles/webserver.dir/RequestData.cpp.o
.PHONY : RequestData.cpp.o

RequestData.i: RequestData.cpp.i

.PHONY : RequestData.i

# target to preprocess a source file
RequestData.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/webserver.dir/build.make CMakeFiles/webserver.dir/RequestData.cpp.i
.PHONY : RequestData.cpp.i

RequestData.s: RequestData.cpp.s

.PHONY : RequestData.s

# target to generate assembly for a file
RequestData.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/webserver.dir/build.make CMakeFiles/webserver.dir/RequestData.cpp.s
.PHONY : RequestData.cpp.s

ThreadPool.o: ThreadPool.cpp.o

.PHONY : ThreadPool.o

# target to build an object file
ThreadPool.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/webserver.dir/build.make CMakeFiles/webserver.dir/ThreadPool.cpp.o
.PHONY : ThreadPool.cpp.o

ThreadPool.i: ThreadPool.cpp.i

.PHONY : ThreadPool.i

# target to preprocess a source file
ThreadPool.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/webserver.dir/build.make CMakeFiles/webserver.dir/ThreadPool.cpp.i
.PHONY : ThreadPool.cpp.i

ThreadPool.s: ThreadPool.cpp.s

.PHONY : ThreadPool.s

# target to generate assembly for a file
ThreadPool.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/webserver.dir/build.make CMakeFiles/webserver.dir/ThreadPool.cpp.s
.PHONY : ThreadPool.cpp.s

TimerQueue.o: TimerQueue.cpp.o

.PHONY : TimerQueue.o

# target to build an object file
TimerQueue.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/webserver.dir/build.make CMakeFiles/webserver.dir/TimerQueue.cpp.o
.PHONY : TimerQueue.cpp.o

TimerQueue.i: TimerQueue.cpp.i

.PHONY : TimerQueue.i

# target to preprocess a source file
TimerQueue.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/webserver.dir/build.make CMakeFiles/webserver.dir/TimerQueue.cpp.i
.PHONY : TimerQueue.cpp.i

TimerQueue.s: TimerQueue.cpp.s

.PHONY : TimerQueue.s

# target to generate assembly for a file
TimerQueue.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/webserver.dir/build.make CMakeFiles/webserver.dir/TimerQueue.cpp.s
.PHONY : TimerQueue.cpp.s

main.o: main.cpp.o

.PHONY : main.o

# target to build an object file
main.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/webserver.dir/build.make CMakeFiles/webserver.dir/main.cpp.o
.PHONY : main.cpp.o

main.i: main.cpp.i

.PHONY : main.i

# target to preprocess a source file
main.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/webserver.dir/build.make CMakeFiles/webserver.dir/main.cpp.i
.PHONY : main.cpp.i

main.s: main.cpp.s

.PHONY : main.s

# target to generate assembly for a file
main.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/webserver.dir/build.make CMakeFiles/webserver.dir/main.cpp.s
.PHONY : main.cpp.s

my_epoll.o: my_epoll.cpp.o

.PHONY : my_epoll.o

# target to build an object file
my_epoll.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/webserver.dir/build.make CMakeFiles/webserver.dir/my_epoll.cpp.o
.PHONY : my_epoll.cpp.o

my_epoll.i: my_epoll.cpp.i

.PHONY : my_epoll.i

# target to preprocess a source file
my_epoll.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/webserver.dir/build.make CMakeFiles/webserver.dir/my_epoll.cpp.i
.PHONY : my_epoll.cpp.i

my_epoll.s: my_epoll.cpp.s

.PHONY : my_epoll.s

# target to generate assembly for a file
my_epoll.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/webserver.dir/build.make CMakeFiles/webserver.dir/my_epoll.cpp.s
.PHONY : my_epoll.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... edit_cache"
	@echo "... webserver"
	@echo "... RequestData.o"
	@echo "... RequestData.i"
	@echo "... RequestData.s"
	@echo "... ThreadPool.o"
	@echo "... ThreadPool.i"
	@echo "... ThreadPool.s"
	@echo "... TimerQueue.o"
	@echo "... TimerQueue.i"
	@echo "... TimerQueue.s"
	@echo "... main.o"
	@echo "... main.i"
	@echo "... main.s"
	@echo "... my_epoll.o"
	@echo "... my_epoll.i"
	@echo "... my_epoll.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

