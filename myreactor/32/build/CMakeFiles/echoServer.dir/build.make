# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/caizhao/Desktop/reactor/myreactor/32c

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/caizhao/Desktop/reactor/myreactor/32c/build

# Include any dependencies generated for this target.
include CMakeFiles/echoServer.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/echoServer.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/echoServer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/echoServer.dir/flags.make

CMakeFiles/echoServer.dir/echoserver.cpp.o: CMakeFiles/echoServer.dir/flags.make
CMakeFiles/echoServer.dir/echoserver.cpp.o: /home/caizhao/Desktop/reactor/myreactor/32c/echoserver.cpp
CMakeFiles/echoServer.dir/echoserver.cpp.o: CMakeFiles/echoServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/caizhao/Desktop/reactor/myreactor/32c/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/echoServer.dir/echoserver.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/echoServer.dir/echoserver.cpp.o -MF CMakeFiles/echoServer.dir/echoserver.cpp.o.d -o CMakeFiles/echoServer.dir/echoserver.cpp.o -c /home/caizhao/Desktop/reactor/myreactor/32c/echoserver.cpp

CMakeFiles/echoServer.dir/echoserver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/echoServer.dir/echoserver.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/caizhao/Desktop/reactor/myreactor/32c/echoserver.cpp > CMakeFiles/echoServer.dir/echoserver.cpp.i

CMakeFiles/echoServer.dir/echoserver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/echoServer.dir/echoserver.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/caizhao/Desktop/reactor/myreactor/32c/echoserver.cpp -o CMakeFiles/echoServer.dir/echoserver.cpp.s

CMakeFiles/echoServer.dir/server/src/Acceptor.cpp.o: CMakeFiles/echoServer.dir/flags.make
CMakeFiles/echoServer.dir/server/src/Acceptor.cpp.o: /home/caizhao/Desktop/reactor/myreactor/32c/server/src/Acceptor.cpp
CMakeFiles/echoServer.dir/server/src/Acceptor.cpp.o: CMakeFiles/echoServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/caizhao/Desktop/reactor/myreactor/32c/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/echoServer.dir/server/src/Acceptor.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/echoServer.dir/server/src/Acceptor.cpp.o -MF CMakeFiles/echoServer.dir/server/src/Acceptor.cpp.o.d -o CMakeFiles/echoServer.dir/server/src/Acceptor.cpp.o -c /home/caizhao/Desktop/reactor/myreactor/32c/server/src/Acceptor.cpp

CMakeFiles/echoServer.dir/server/src/Acceptor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/echoServer.dir/server/src/Acceptor.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/caizhao/Desktop/reactor/myreactor/32c/server/src/Acceptor.cpp > CMakeFiles/echoServer.dir/server/src/Acceptor.cpp.i

CMakeFiles/echoServer.dir/server/src/Acceptor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/echoServer.dir/server/src/Acceptor.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/caizhao/Desktop/reactor/myreactor/32c/server/src/Acceptor.cpp -o CMakeFiles/echoServer.dir/server/src/Acceptor.cpp.s

CMakeFiles/echoServer.dir/server/src/Buffer.cpp.o: CMakeFiles/echoServer.dir/flags.make
CMakeFiles/echoServer.dir/server/src/Buffer.cpp.o: /home/caizhao/Desktop/reactor/myreactor/32c/server/src/Buffer.cpp
CMakeFiles/echoServer.dir/server/src/Buffer.cpp.o: CMakeFiles/echoServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/caizhao/Desktop/reactor/myreactor/32c/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/echoServer.dir/server/src/Buffer.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/echoServer.dir/server/src/Buffer.cpp.o -MF CMakeFiles/echoServer.dir/server/src/Buffer.cpp.o.d -o CMakeFiles/echoServer.dir/server/src/Buffer.cpp.o -c /home/caizhao/Desktop/reactor/myreactor/32c/server/src/Buffer.cpp

CMakeFiles/echoServer.dir/server/src/Buffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/echoServer.dir/server/src/Buffer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/caizhao/Desktop/reactor/myreactor/32c/server/src/Buffer.cpp > CMakeFiles/echoServer.dir/server/src/Buffer.cpp.i

CMakeFiles/echoServer.dir/server/src/Buffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/echoServer.dir/server/src/Buffer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/caizhao/Desktop/reactor/myreactor/32c/server/src/Buffer.cpp -o CMakeFiles/echoServer.dir/server/src/Buffer.cpp.s

CMakeFiles/echoServer.dir/server/src/Channel.cpp.o: CMakeFiles/echoServer.dir/flags.make
CMakeFiles/echoServer.dir/server/src/Channel.cpp.o: /home/caizhao/Desktop/reactor/myreactor/32c/server/src/Channel.cpp
CMakeFiles/echoServer.dir/server/src/Channel.cpp.o: CMakeFiles/echoServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/caizhao/Desktop/reactor/myreactor/32c/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/echoServer.dir/server/src/Channel.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/echoServer.dir/server/src/Channel.cpp.o -MF CMakeFiles/echoServer.dir/server/src/Channel.cpp.o.d -o CMakeFiles/echoServer.dir/server/src/Channel.cpp.o -c /home/caizhao/Desktop/reactor/myreactor/32c/server/src/Channel.cpp

CMakeFiles/echoServer.dir/server/src/Channel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/echoServer.dir/server/src/Channel.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/caizhao/Desktop/reactor/myreactor/32c/server/src/Channel.cpp > CMakeFiles/echoServer.dir/server/src/Channel.cpp.i

CMakeFiles/echoServer.dir/server/src/Channel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/echoServer.dir/server/src/Channel.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/caizhao/Desktop/reactor/myreactor/32c/server/src/Channel.cpp -o CMakeFiles/echoServer.dir/server/src/Channel.cpp.s

CMakeFiles/echoServer.dir/server/src/Connection.cpp.o: CMakeFiles/echoServer.dir/flags.make
CMakeFiles/echoServer.dir/server/src/Connection.cpp.o: /home/caizhao/Desktop/reactor/myreactor/32c/server/src/Connection.cpp
CMakeFiles/echoServer.dir/server/src/Connection.cpp.o: CMakeFiles/echoServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/caizhao/Desktop/reactor/myreactor/32c/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/echoServer.dir/server/src/Connection.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/echoServer.dir/server/src/Connection.cpp.o -MF CMakeFiles/echoServer.dir/server/src/Connection.cpp.o.d -o CMakeFiles/echoServer.dir/server/src/Connection.cpp.o -c /home/caizhao/Desktop/reactor/myreactor/32c/server/src/Connection.cpp

CMakeFiles/echoServer.dir/server/src/Connection.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/echoServer.dir/server/src/Connection.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/caizhao/Desktop/reactor/myreactor/32c/server/src/Connection.cpp > CMakeFiles/echoServer.dir/server/src/Connection.cpp.i

CMakeFiles/echoServer.dir/server/src/Connection.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/echoServer.dir/server/src/Connection.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/caizhao/Desktop/reactor/myreactor/32c/server/src/Connection.cpp -o CMakeFiles/echoServer.dir/server/src/Connection.cpp.s

CMakeFiles/echoServer.dir/server/src/Epoll.cpp.o: CMakeFiles/echoServer.dir/flags.make
CMakeFiles/echoServer.dir/server/src/Epoll.cpp.o: /home/caizhao/Desktop/reactor/myreactor/32c/server/src/Epoll.cpp
CMakeFiles/echoServer.dir/server/src/Epoll.cpp.o: CMakeFiles/echoServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/caizhao/Desktop/reactor/myreactor/32c/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/echoServer.dir/server/src/Epoll.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/echoServer.dir/server/src/Epoll.cpp.o -MF CMakeFiles/echoServer.dir/server/src/Epoll.cpp.o.d -o CMakeFiles/echoServer.dir/server/src/Epoll.cpp.o -c /home/caizhao/Desktop/reactor/myreactor/32c/server/src/Epoll.cpp

CMakeFiles/echoServer.dir/server/src/Epoll.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/echoServer.dir/server/src/Epoll.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/caizhao/Desktop/reactor/myreactor/32c/server/src/Epoll.cpp > CMakeFiles/echoServer.dir/server/src/Epoll.cpp.i

CMakeFiles/echoServer.dir/server/src/Epoll.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/echoServer.dir/server/src/Epoll.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/caizhao/Desktop/reactor/myreactor/32c/server/src/Epoll.cpp -o CMakeFiles/echoServer.dir/server/src/Epoll.cpp.s

CMakeFiles/echoServer.dir/server/src/EventLoop.cpp.o: CMakeFiles/echoServer.dir/flags.make
CMakeFiles/echoServer.dir/server/src/EventLoop.cpp.o: /home/caizhao/Desktop/reactor/myreactor/32c/server/src/EventLoop.cpp
CMakeFiles/echoServer.dir/server/src/EventLoop.cpp.o: CMakeFiles/echoServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/caizhao/Desktop/reactor/myreactor/32c/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/echoServer.dir/server/src/EventLoop.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/echoServer.dir/server/src/EventLoop.cpp.o -MF CMakeFiles/echoServer.dir/server/src/EventLoop.cpp.o.d -o CMakeFiles/echoServer.dir/server/src/EventLoop.cpp.o -c /home/caizhao/Desktop/reactor/myreactor/32c/server/src/EventLoop.cpp

CMakeFiles/echoServer.dir/server/src/EventLoop.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/echoServer.dir/server/src/EventLoop.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/caizhao/Desktop/reactor/myreactor/32c/server/src/EventLoop.cpp > CMakeFiles/echoServer.dir/server/src/EventLoop.cpp.i

CMakeFiles/echoServer.dir/server/src/EventLoop.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/echoServer.dir/server/src/EventLoop.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/caizhao/Desktop/reactor/myreactor/32c/server/src/EventLoop.cpp -o CMakeFiles/echoServer.dir/server/src/EventLoop.cpp.s

CMakeFiles/echoServer.dir/server/src/InetAddress.cpp.o: CMakeFiles/echoServer.dir/flags.make
CMakeFiles/echoServer.dir/server/src/InetAddress.cpp.o: /home/caizhao/Desktop/reactor/myreactor/32c/server/src/InetAddress.cpp
CMakeFiles/echoServer.dir/server/src/InetAddress.cpp.o: CMakeFiles/echoServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/caizhao/Desktop/reactor/myreactor/32c/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/echoServer.dir/server/src/InetAddress.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/echoServer.dir/server/src/InetAddress.cpp.o -MF CMakeFiles/echoServer.dir/server/src/InetAddress.cpp.o.d -o CMakeFiles/echoServer.dir/server/src/InetAddress.cpp.o -c /home/caizhao/Desktop/reactor/myreactor/32c/server/src/InetAddress.cpp

CMakeFiles/echoServer.dir/server/src/InetAddress.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/echoServer.dir/server/src/InetAddress.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/caizhao/Desktop/reactor/myreactor/32c/server/src/InetAddress.cpp > CMakeFiles/echoServer.dir/server/src/InetAddress.cpp.i

CMakeFiles/echoServer.dir/server/src/InetAddress.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/echoServer.dir/server/src/InetAddress.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/caizhao/Desktop/reactor/myreactor/32c/server/src/InetAddress.cpp -o CMakeFiles/echoServer.dir/server/src/InetAddress.cpp.s

CMakeFiles/echoServer.dir/server/src/Socket.cpp.o: CMakeFiles/echoServer.dir/flags.make
CMakeFiles/echoServer.dir/server/src/Socket.cpp.o: /home/caizhao/Desktop/reactor/myreactor/32c/server/src/Socket.cpp
CMakeFiles/echoServer.dir/server/src/Socket.cpp.o: CMakeFiles/echoServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/caizhao/Desktop/reactor/myreactor/32c/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/echoServer.dir/server/src/Socket.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/echoServer.dir/server/src/Socket.cpp.o -MF CMakeFiles/echoServer.dir/server/src/Socket.cpp.o.d -o CMakeFiles/echoServer.dir/server/src/Socket.cpp.o -c /home/caizhao/Desktop/reactor/myreactor/32c/server/src/Socket.cpp

CMakeFiles/echoServer.dir/server/src/Socket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/echoServer.dir/server/src/Socket.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/caizhao/Desktop/reactor/myreactor/32c/server/src/Socket.cpp > CMakeFiles/echoServer.dir/server/src/Socket.cpp.i

CMakeFiles/echoServer.dir/server/src/Socket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/echoServer.dir/server/src/Socket.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/caizhao/Desktop/reactor/myreactor/32c/server/src/Socket.cpp -o CMakeFiles/echoServer.dir/server/src/Socket.cpp.s

CMakeFiles/echoServer.dir/server/src/TcpServer.cpp.o: CMakeFiles/echoServer.dir/flags.make
CMakeFiles/echoServer.dir/server/src/TcpServer.cpp.o: /home/caizhao/Desktop/reactor/myreactor/32c/server/src/TcpServer.cpp
CMakeFiles/echoServer.dir/server/src/TcpServer.cpp.o: CMakeFiles/echoServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/caizhao/Desktop/reactor/myreactor/32c/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/echoServer.dir/server/src/TcpServer.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/echoServer.dir/server/src/TcpServer.cpp.o -MF CMakeFiles/echoServer.dir/server/src/TcpServer.cpp.o.d -o CMakeFiles/echoServer.dir/server/src/TcpServer.cpp.o -c /home/caizhao/Desktop/reactor/myreactor/32c/server/src/TcpServer.cpp

CMakeFiles/echoServer.dir/server/src/TcpServer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/echoServer.dir/server/src/TcpServer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/caizhao/Desktop/reactor/myreactor/32c/server/src/TcpServer.cpp > CMakeFiles/echoServer.dir/server/src/TcpServer.cpp.i

CMakeFiles/echoServer.dir/server/src/TcpServer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/echoServer.dir/server/src/TcpServer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/caizhao/Desktop/reactor/myreactor/32c/server/src/TcpServer.cpp -o CMakeFiles/echoServer.dir/server/src/TcpServer.cpp.s

CMakeFiles/echoServer.dir/affairs/code/EchoServer.cpp.o: CMakeFiles/echoServer.dir/flags.make
CMakeFiles/echoServer.dir/affairs/code/EchoServer.cpp.o: /home/caizhao/Desktop/reactor/myreactor/32c/affairs/code/EchoServer.cpp
CMakeFiles/echoServer.dir/affairs/code/EchoServer.cpp.o: CMakeFiles/echoServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/caizhao/Desktop/reactor/myreactor/32c/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/echoServer.dir/affairs/code/EchoServer.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/echoServer.dir/affairs/code/EchoServer.cpp.o -MF CMakeFiles/echoServer.dir/affairs/code/EchoServer.cpp.o.d -o CMakeFiles/echoServer.dir/affairs/code/EchoServer.cpp.o -c /home/caizhao/Desktop/reactor/myreactor/32c/affairs/code/EchoServer.cpp

CMakeFiles/echoServer.dir/affairs/code/EchoServer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/echoServer.dir/affairs/code/EchoServer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/caizhao/Desktop/reactor/myreactor/32c/affairs/code/EchoServer.cpp > CMakeFiles/echoServer.dir/affairs/code/EchoServer.cpp.i

CMakeFiles/echoServer.dir/affairs/code/EchoServer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/echoServer.dir/affairs/code/EchoServer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/caizhao/Desktop/reactor/myreactor/32c/affairs/code/EchoServer.cpp -o CMakeFiles/echoServer.dir/affairs/code/EchoServer.cpp.s

CMakeFiles/echoServer.dir/thread/src/ThreadPool.cpp.o: CMakeFiles/echoServer.dir/flags.make
CMakeFiles/echoServer.dir/thread/src/ThreadPool.cpp.o: /home/caizhao/Desktop/reactor/myreactor/32c/thread/src/ThreadPool.cpp
CMakeFiles/echoServer.dir/thread/src/ThreadPool.cpp.o: CMakeFiles/echoServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/caizhao/Desktop/reactor/myreactor/32c/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/echoServer.dir/thread/src/ThreadPool.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/echoServer.dir/thread/src/ThreadPool.cpp.o -MF CMakeFiles/echoServer.dir/thread/src/ThreadPool.cpp.o.d -o CMakeFiles/echoServer.dir/thread/src/ThreadPool.cpp.o -c /home/caizhao/Desktop/reactor/myreactor/32c/thread/src/ThreadPool.cpp

CMakeFiles/echoServer.dir/thread/src/ThreadPool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/echoServer.dir/thread/src/ThreadPool.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/caizhao/Desktop/reactor/myreactor/32c/thread/src/ThreadPool.cpp > CMakeFiles/echoServer.dir/thread/src/ThreadPool.cpp.i

CMakeFiles/echoServer.dir/thread/src/ThreadPool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/echoServer.dir/thread/src/ThreadPool.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/caizhao/Desktop/reactor/myreactor/32c/thread/src/ThreadPool.cpp -o CMakeFiles/echoServer.dir/thread/src/ThreadPool.cpp.s

# Object files for target echoServer
echoServer_OBJECTS = \
"CMakeFiles/echoServer.dir/echoserver.cpp.o" \
"CMakeFiles/echoServer.dir/server/src/Acceptor.cpp.o" \
"CMakeFiles/echoServer.dir/server/src/Buffer.cpp.o" \
"CMakeFiles/echoServer.dir/server/src/Channel.cpp.o" \
"CMakeFiles/echoServer.dir/server/src/Connection.cpp.o" \
"CMakeFiles/echoServer.dir/server/src/Epoll.cpp.o" \
"CMakeFiles/echoServer.dir/server/src/EventLoop.cpp.o" \
"CMakeFiles/echoServer.dir/server/src/InetAddress.cpp.o" \
"CMakeFiles/echoServer.dir/server/src/Socket.cpp.o" \
"CMakeFiles/echoServer.dir/server/src/TcpServer.cpp.o" \
"CMakeFiles/echoServer.dir/affairs/code/EchoServer.cpp.o" \
"CMakeFiles/echoServer.dir/thread/src/ThreadPool.cpp.o"

# External object files for target echoServer
echoServer_EXTERNAL_OBJECTS =

echoServer: CMakeFiles/echoServer.dir/echoserver.cpp.o
echoServer: CMakeFiles/echoServer.dir/server/src/Acceptor.cpp.o
echoServer: CMakeFiles/echoServer.dir/server/src/Buffer.cpp.o
echoServer: CMakeFiles/echoServer.dir/server/src/Channel.cpp.o
echoServer: CMakeFiles/echoServer.dir/server/src/Connection.cpp.o
echoServer: CMakeFiles/echoServer.dir/server/src/Epoll.cpp.o
echoServer: CMakeFiles/echoServer.dir/server/src/EventLoop.cpp.o
echoServer: CMakeFiles/echoServer.dir/server/src/InetAddress.cpp.o
echoServer: CMakeFiles/echoServer.dir/server/src/Socket.cpp.o
echoServer: CMakeFiles/echoServer.dir/server/src/TcpServer.cpp.o
echoServer: CMakeFiles/echoServer.dir/affairs/code/EchoServer.cpp.o
echoServer: CMakeFiles/echoServer.dir/thread/src/ThreadPool.cpp.o
echoServer: CMakeFiles/echoServer.dir/build.make
echoServer: CMakeFiles/echoServer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/caizhao/Desktop/reactor/myreactor/32c/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Linking CXX executable echoServer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/echoServer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/echoServer.dir/build: echoServer
.PHONY : CMakeFiles/echoServer.dir/build

CMakeFiles/echoServer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/echoServer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/echoServer.dir/clean

CMakeFiles/echoServer.dir/depend:
	cd /home/caizhao/Desktop/reactor/myreactor/32c/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/caizhao/Desktop/reactor/myreactor/32c /home/caizhao/Desktop/reactor/myreactor/32c /home/caizhao/Desktop/reactor/myreactor/32c/build /home/caizhao/Desktop/reactor/myreactor/32c/build /home/caizhao/Desktop/reactor/myreactor/32c/build/CMakeFiles/echoServer.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/echoServer.dir/depend

