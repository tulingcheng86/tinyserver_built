# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

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
CMAKE_COMMAND = /home/sbim/.local/lib/python3.10/site-packages/cmake/data/bin/cmake

# The command to remove a file.
RM = /home/sbim/.local/lib/python3.10/site-packages/cmake/data/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/sbim/Music/webserver_built

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sbim/Music/webserver_built/build

# Include any dependencies generated for this target.
include CMakeFiles/WebServer.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/WebServer.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/WebServer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/WebServer.dir/flags.make

CMakeFiles/WebServer.dir/src/CGImysql/sql_connection_pool.cpp.o: CMakeFiles/WebServer.dir/flags.make
CMakeFiles/WebServer.dir/src/CGImysql/sql_connection_pool.cpp.o: /home/sbim/Music/webserver_built/src/CGImysql/sql_connection_pool.cpp
CMakeFiles/WebServer.dir/src/CGImysql/sql_connection_pool.cpp.o: CMakeFiles/WebServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/sbim/Music/webserver_built/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/WebServer.dir/src/CGImysql/sql_connection_pool.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/WebServer.dir/src/CGImysql/sql_connection_pool.cpp.o -MF CMakeFiles/WebServer.dir/src/CGImysql/sql_connection_pool.cpp.o.d -o CMakeFiles/WebServer.dir/src/CGImysql/sql_connection_pool.cpp.o -c /home/sbim/Music/webserver_built/src/CGImysql/sql_connection_pool.cpp

CMakeFiles/WebServer.dir/src/CGImysql/sql_connection_pool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/WebServer.dir/src/CGImysql/sql_connection_pool.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sbim/Music/webserver_built/src/CGImysql/sql_connection_pool.cpp > CMakeFiles/WebServer.dir/src/CGImysql/sql_connection_pool.cpp.i

CMakeFiles/WebServer.dir/src/CGImysql/sql_connection_pool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/WebServer.dir/src/CGImysql/sql_connection_pool.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sbim/Music/webserver_built/src/CGImysql/sql_connection_pool.cpp -o CMakeFiles/WebServer.dir/src/CGImysql/sql_connection_pool.cpp.s

CMakeFiles/WebServer.dir/src/config/config.cpp.o: CMakeFiles/WebServer.dir/flags.make
CMakeFiles/WebServer.dir/src/config/config.cpp.o: /home/sbim/Music/webserver_built/src/config/config.cpp
CMakeFiles/WebServer.dir/src/config/config.cpp.o: CMakeFiles/WebServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/sbim/Music/webserver_built/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/WebServer.dir/src/config/config.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/WebServer.dir/src/config/config.cpp.o -MF CMakeFiles/WebServer.dir/src/config/config.cpp.o.d -o CMakeFiles/WebServer.dir/src/config/config.cpp.o -c /home/sbim/Music/webserver_built/src/config/config.cpp

CMakeFiles/WebServer.dir/src/config/config.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/WebServer.dir/src/config/config.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sbim/Music/webserver_built/src/config/config.cpp > CMakeFiles/WebServer.dir/src/config/config.cpp.i

CMakeFiles/WebServer.dir/src/config/config.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/WebServer.dir/src/config/config.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sbim/Music/webserver_built/src/config/config.cpp -o CMakeFiles/WebServer.dir/src/config/config.cpp.s

CMakeFiles/WebServer.dir/src/http/http_conn.cpp.o: CMakeFiles/WebServer.dir/flags.make
CMakeFiles/WebServer.dir/src/http/http_conn.cpp.o: /home/sbim/Music/webserver_built/src/http/http_conn.cpp
CMakeFiles/WebServer.dir/src/http/http_conn.cpp.o: CMakeFiles/WebServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/sbim/Music/webserver_built/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/WebServer.dir/src/http/http_conn.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/WebServer.dir/src/http/http_conn.cpp.o -MF CMakeFiles/WebServer.dir/src/http/http_conn.cpp.o.d -o CMakeFiles/WebServer.dir/src/http/http_conn.cpp.o -c /home/sbim/Music/webserver_built/src/http/http_conn.cpp

CMakeFiles/WebServer.dir/src/http/http_conn.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/WebServer.dir/src/http/http_conn.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sbim/Music/webserver_built/src/http/http_conn.cpp > CMakeFiles/WebServer.dir/src/http/http_conn.cpp.i

CMakeFiles/WebServer.dir/src/http/http_conn.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/WebServer.dir/src/http/http_conn.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sbim/Music/webserver_built/src/http/http_conn.cpp -o CMakeFiles/WebServer.dir/src/http/http_conn.cpp.s

CMakeFiles/WebServer.dir/src/log/log.cpp.o: CMakeFiles/WebServer.dir/flags.make
CMakeFiles/WebServer.dir/src/log/log.cpp.o: /home/sbim/Music/webserver_built/src/log/log.cpp
CMakeFiles/WebServer.dir/src/log/log.cpp.o: CMakeFiles/WebServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/sbim/Music/webserver_built/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/WebServer.dir/src/log/log.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/WebServer.dir/src/log/log.cpp.o -MF CMakeFiles/WebServer.dir/src/log/log.cpp.o.d -o CMakeFiles/WebServer.dir/src/log/log.cpp.o -c /home/sbim/Music/webserver_built/src/log/log.cpp

CMakeFiles/WebServer.dir/src/log/log.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/WebServer.dir/src/log/log.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sbim/Music/webserver_built/src/log/log.cpp > CMakeFiles/WebServer.dir/src/log/log.cpp.i

CMakeFiles/WebServer.dir/src/log/log.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/WebServer.dir/src/log/log.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sbim/Music/webserver_built/src/log/log.cpp -o CMakeFiles/WebServer.dir/src/log/log.cpp.s

CMakeFiles/WebServer.dir/src/server/webserver.cpp.o: CMakeFiles/WebServer.dir/flags.make
CMakeFiles/WebServer.dir/src/server/webserver.cpp.o: /home/sbim/Music/webserver_built/src/server/webserver.cpp
CMakeFiles/WebServer.dir/src/server/webserver.cpp.o: CMakeFiles/WebServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/sbim/Music/webserver_built/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/WebServer.dir/src/server/webserver.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/WebServer.dir/src/server/webserver.cpp.o -MF CMakeFiles/WebServer.dir/src/server/webserver.cpp.o.d -o CMakeFiles/WebServer.dir/src/server/webserver.cpp.o -c /home/sbim/Music/webserver_built/src/server/webserver.cpp

CMakeFiles/WebServer.dir/src/server/webserver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/WebServer.dir/src/server/webserver.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sbim/Music/webserver_built/src/server/webserver.cpp > CMakeFiles/WebServer.dir/src/server/webserver.cpp.i

CMakeFiles/WebServer.dir/src/server/webserver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/WebServer.dir/src/server/webserver.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sbim/Music/webserver_built/src/server/webserver.cpp -o CMakeFiles/WebServer.dir/src/server/webserver.cpp.s

CMakeFiles/WebServer.dir/src/timer/lst_timer.cpp.o: CMakeFiles/WebServer.dir/flags.make
CMakeFiles/WebServer.dir/src/timer/lst_timer.cpp.o: /home/sbim/Music/webserver_built/src/timer/lst_timer.cpp
CMakeFiles/WebServer.dir/src/timer/lst_timer.cpp.o: CMakeFiles/WebServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/sbim/Music/webserver_built/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/WebServer.dir/src/timer/lst_timer.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/WebServer.dir/src/timer/lst_timer.cpp.o -MF CMakeFiles/WebServer.dir/src/timer/lst_timer.cpp.o.d -o CMakeFiles/WebServer.dir/src/timer/lst_timer.cpp.o -c /home/sbim/Music/webserver_built/src/timer/lst_timer.cpp

CMakeFiles/WebServer.dir/src/timer/lst_timer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/WebServer.dir/src/timer/lst_timer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sbim/Music/webserver_built/src/timer/lst_timer.cpp > CMakeFiles/WebServer.dir/src/timer/lst_timer.cpp.i

CMakeFiles/WebServer.dir/src/timer/lst_timer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/WebServer.dir/src/timer/lst_timer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sbim/Music/webserver_built/src/timer/lst_timer.cpp -o CMakeFiles/WebServer.dir/src/timer/lst_timer.cpp.s

CMakeFiles/WebServer.dir/main.cpp.o: CMakeFiles/WebServer.dir/flags.make
CMakeFiles/WebServer.dir/main.cpp.o: /home/sbim/Music/webserver_built/main.cpp
CMakeFiles/WebServer.dir/main.cpp.o: CMakeFiles/WebServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/sbim/Music/webserver_built/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/WebServer.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/WebServer.dir/main.cpp.o -MF CMakeFiles/WebServer.dir/main.cpp.o.d -o CMakeFiles/WebServer.dir/main.cpp.o -c /home/sbim/Music/webserver_built/main.cpp

CMakeFiles/WebServer.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/WebServer.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sbim/Music/webserver_built/main.cpp > CMakeFiles/WebServer.dir/main.cpp.i

CMakeFiles/WebServer.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/WebServer.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sbim/Music/webserver_built/main.cpp -o CMakeFiles/WebServer.dir/main.cpp.s

# Object files for target WebServer
WebServer_OBJECTS = \
"CMakeFiles/WebServer.dir/src/CGImysql/sql_connection_pool.cpp.o" \
"CMakeFiles/WebServer.dir/src/config/config.cpp.o" \
"CMakeFiles/WebServer.dir/src/http/http_conn.cpp.o" \
"CMakeFiles/WebServer.dir/src/log/log.cpp.o" \
"CMakeFiles/WebServer.dir/src/server/webserver.cpp.o" \
"CMakeFiles/WebServer.dir/src/timer/lst_timer.cpp.o" \
"CMakeFiles/WebServer.dir/main.cpp.o"

# External object files for target WebServer
WebServer_EXTERNAL_OBJECTS =

WebServer: CMakeFiles/WebServer.dir/src/CGImysql/sql_connection_pool.cpp.o
WebServer: CMakeFiles/WebServer.dir/src/config/config.cpp.o
WebServer: CMakeFiles/WebServer.dir/src/http/http_conn.cpp.o
WebServer: CMakeFiles/WebServer.dir/src/log/log.cpp.o
WebServer: CMakeFiles/WebServer.dir/src/server/webserver.cpp.o
WebServer: CMakeFiles/WebServer.dir/src/timer/lst_timer.cpp.o
WebServer: CMakeFiles/WebServer.dir/main.cpp.o
WebServer: CMakeFiles/WebServer.dir/build.make
WebServer: /usr/lib/x86_64-linux-gnu/libmysqlclient.so
WebServer: CMakeFiles/WebServer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/sbim/Music/webserver_built/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX executable WebServer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/WebServer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/WebServer.dir/build: WebServer
.PHONY : CMakeFiles/WebServer.dir/build

CMakeFiles/WebServer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/WebServer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/WebServer.dir/clean

CMakeFiles/WebServer.dir/depend:
	cd /home/sbim/Music/webserver_built/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sbim/Music/webserver_built /home/sbim/Music/webserver_built /home/sbim/Music/webserver_built/build /home/sbim/Music/webserver_built/build /home/sbim/Music/webserver_built/build/CMakeFiles/WebServer.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/WebServer.dir/depend

