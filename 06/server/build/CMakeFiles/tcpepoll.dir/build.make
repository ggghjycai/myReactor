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
CMAKE_SOURCE_DIR = /home/caizhao/Desktop/reactor/myreactor/06/server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/caizhao/Desktop/reactor/myreactor/06/server/build

# Include any dependencies generated for this target.
include CMakeFiles/tcpepoll.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/tcpepoll.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/tcpepoll.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tcpepoll.dir/flags.make

CMakeFiles/tcpepoll.dir/tcpepoll.cpp.o: CMakeFiles/tcpepoll.dir/flags.make
CMakeFiles/tcpepoll.dir/tcpepoll.cpp.o: /home/caizhao/Desktop/reactor/myreactor/06/server/tcpepoll.cpp
CMakeFiles/tcpepoll.dir/tcpepoll.cpp.o: CMakeFiles/tcpepoll.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/caizhao/Desktop/reactor/myreactor/06/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/tcpepoll.dir/tcpepoll.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/tcpepoll.dir/tcpepoll.cpp.o -MF CMakeFiles/tcpepoll.dir/tcpepoll.cpp.o.d -o CMakeFiles/tcpepoll.dir/tcpepoll.cpp.o -c /home/caizhao/Desktop/reactor/myreactor/06/server/tcpepoll.cpp

CMakeFiles/tcpepoll.dir/tcpepoll.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/tcpepoll.dir/tcpepoll.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/caizhao/Desktop/reactor/myreactor/06/server/tcpepoll.cpp > CMakeFiles/tcpepoll.dir/tcpepoll.cpp.i

CMakeFiles/tcpepoll.dir/tcpepoll.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/tcpepoll.dir/tcpepoll.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/caizhao/Desktop/reactor/myreactor/06/server/tcpepoll.cpp -o CMakeFiles/tcpepoll.dir/tcpepoll.cpp.s

CMakeFiles/tcpepoll.dir/src/Channel.cpp.o: CMakeFiles/tcpepoll.dir/flags.make
CMakeFiles/tcpepoll.dir/src/Channel.cpp.o: /home/caizhao/Desktop/reactor/myreactor/06/server/src/Channel.cpp
CMakeFiles/tcpepoll.dir/src/Channel.cpp.o: CMakeFiles/tcpepoll.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/caizhao/Desktop/reactor/myreactor/06/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/tcpepoll.dir/src/Channel.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/tcpepoll.dir/src/Channel.cpp.o -MF CMakeFiles/tcpepoll.dir/src/Channel.cpp.o.d -o CMakeFiles/tcpepoll.dir/src/Channel.cpp.o -c /home/caizhao/Desktop/reactor/myreactor/06/server/src/Channel.cpp

CMakeFiles/tcpepoll.dir/src/Channel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/tcpepoll.dir/src/Channel.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/caizhao/Desktop/reactor/myreactor/06/server/src/Channel.cpp > CMakeFiles/tcpepoll.dir/src/Channel.cpp.i

CMakeFiles/tcpepoll.dir/src/Channel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/tcpepoll.dir/src/Channel.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/caizhao/Desktop/reactor/myreactor/06/server/src/Channel.cpp -o CMakeFiles/tcpepoll.dir/src/Channel.cpp.s

CMakeFiles/tcpepoll.dir/src/Epoll.cpp.o: CMakeFiles/tcpepoll.dir/flags.make
CMakeFiles/tcpepoll.dir/src/Epoll.cpp.o: /home/caizhao/Desktop/reactor/myreactor/06/server/src/Epoll.cpp
CMakeFiles/tcpepoll.dir/src/Epoll.cpp.o: CMakeFiles/tcpepoll.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/caizhao/Desktop/reactor/myreactor/06/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/tcpepoll.dir/src/Epoll.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/tcpepoll.dir/src/Epoll.cpp.o -MF CMakeFiles/tcpepoll.dir/src/Epoll.cpp.o.d -o CMakeFiles/tcpepoll.dir/src/Epoll.cpp.o -c /home/caizhao/Desktop/reactor/myreactor/06/server/src/Epoll.cpp

CMakeFiles/tcpepoll.dir/src/Epoll.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/tcpepoll.dir/src/Epoll.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/caizhao/Desktop/reactor/myreactor/06/server/src/Epoll.cpp > CMakeFiles/tcpepoll.dir/src/Epoll.cpp.i

CMakeFiles/tcpepoll.dir/src/Epoll.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/tcpepoll.dir/src/Epoll.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/caizhao/Desktop/reactor/myreactor/06/server/src/Epoll.cpp -o CMakeFiles/tcpepoll.dir/src/Epoll.cpp.s

CMakeFiles/tcpepoll.dir/src/InetAddress.cpp.o: CMakeFiles/tcpepoll.dir/flags.make
CMakeFiles/tcpepoll.dir/src/InetAddress.cpp.o: /home/caizhao/Desktop/reactor/myreactor/06/server/src/InetAddress.cpp
CMakeFiles/tcpepoll.dir/src/InetAddress.cpp.o: CMakeFiles/tcpepoll.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/caizhao/Desktop/reactor/myreactor/06/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/tcpepoll.dir/src/InetAddress.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/tcpepoll.dir/src/InetAddress.cpp.o -MF CMakeFiles/tcpepoll.dir/src/InetAddress.cpp.o.d -o CMakeFiles/tcpepoll.dir/src/InetAddress.cpp.o -c /home/caizhao/Desktop/reactor/myreactor/06/server/src/InetAddress.cpp

CMakeFiles/tcpepoll.dir/src/InetAddress.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/tcpepoll.dir/src/InetAddress.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/caizhao/Desktop/reactor/myreactor/06/server/src/InetAddress.cpp > CMakeFiles/tcpepoll.dir/src/InetAddress.cpp.i

CMakeFiles/tcpepoll.dir/src/InetAddress.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/tcpepoll.dir/src/InetAddress.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/caizhao/Desktop/reactor/myreactor/06/server/src/InetAddress.cpp -o CMakeFiles/tcpepoll.dir/src/InetAddress.cpp.s

CMakeFiles/tcpepoll.dir/src/Socket.cpp.o: CMakeFiles/tcpepoll.dir/flags.make
CMakeFiles/tcpepoll.dir/src/Socket.cpp.o: /home/caizhao/Desktop/reactor/myreactor/06/server/src/Socket.cpp
CMakeFiles/tcpepoll.dir/src/Socket.cpp.o: CMakeFiles/tcpepoll.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/caizhao/Desktop/reactor/myreactor/06/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/tcpepoll.dir/src/Socket.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/tcpepoll.dir/src/Socket.cpp.o -MF CMakeFiles/tcpepoll.dir/src/Socket.cpp.o.d -o CMakeFiles/tcpepoll.dir/src/Socket.cpp.o -c /home/caizhao/Desktop/reactor/myreactor/06/server/src/Socket.cpp

CMakeFiles/tcpepoll.dir/src/Socket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/tcpepoll.dir/src/Socket.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/caizhao/Desktop/reactor/myreactor/06/server/src/Socket.cpp > CMakeFiles/tcpepoll.dir/src/Socket.cpp.i

CMakeFiles/tcpepoll.dir/src/Socket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/tcpepoll.dir/src/Socket.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/caizhao/Desktop/reactor/myreactor/06/server/src/Socket.cpp -o CMakeFiles/tcpepoll.dir/src/Socket.cpp.s

# Object files for target tcpepoll
tcpepoll_OBJECTS = \
"CMakeFiles/tcpepoll.dir/tcpepoll.cpp.o" \
"CMakeFiles/tcpepoll.dir/src/Channel.cpp.o" \
"CMakeFiles/tcpepoll.dir/src/Epoll.cpp.o" \
"CMakeFiles/tcpepoll.dir/src/InetAddress.cpp.o" \
"CMakeFiles/tcpepoll.dir/src/Socket.cpp.o"

# External object files for target tcpepoll
tcpepoll_EXTERNAL_OBJECTS =

tcpepoll: CMakeFiles/tcpepoll.dir/tcpepoll.cpp.o
tcpepoll: CMakeFiles/tcpepoll.dir/src/Channel.cpp.o
tcpepoll: CMakeFiles/tcpepoll.dir/src/Epoll.cpp.o
tcpepoll: CMakeFiles/tcpepoll.dir/src/InetAddress.cpp.o
tcpepoll: CMakeFiles/tcpepoll.dir/src/Socket.cpp.o
tcpepoll: CMakeFiles/tcpepoll.dir/build.make
tcpepoll: CMakeFiles/tcpepoll.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/caizhao/Desktop/reactor/myreactor/06/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable tcpepoll"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tcpepoll.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/tcpepoll.dir/build: tcpepoll
.PHONY : CMakeFiles/tcpepoll.dir/build

CMakeFiles/tcpepoll.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/tcpepoll.dir/cmake_clean.cmake
.PHONY : CMakeFiles/tcpepoll.dir/clean

CMakeFiles/tcpepoll.dir/depend:
	cd /home/caizhao/Desktop/reactor/myreactor/06/server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/caizhao/Desktop/reactor/myreactor/06/server /home/caizhao/Desktop/reactor/myreactor/06/server /home/caizhao/Desktop/reactor/myreactor/06/server/build /home/caizhao/Desktop/reactor/myreactor/06/server/build /home/caizhao/Desktop/reactor/myreactor/06/server/build/CMakeFiles/tcpepoll.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/tcpepoll.dir/depend

