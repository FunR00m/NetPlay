# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_SOURCE_DIR = /home/fedor/Documents/Projects/NetPlay/Client

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/fedor/Documents/Projects/NetPlay/Client/build

# Include any dependencies generated for this target.
include CMakeFiles/Client.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Client.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Client.dir/flags.make

CMakeFiles/Client.dir/main.cpp.o: CMakeFiles/Client.dir/flags.make
CMakeFiles/Client.dir/main.cpp.o: /home/fedor/Documents/Projects/NetPlay/Client/main.cpp
CMakeFiles/Client.dir/main.cpp.o: CMakeFiles/Client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fedor/Documents/Projects/NetPlay/Client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Client.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Client.dir/main.cpp.o -MF CMakeFiles/Client.dir/main.cpp.o.d -o CMakeFiles/Client.dir/main.cpp.o -c /home/fedor/Documents/Projects/NetPlay/Client/main.cpp

CMakeFiles/Client.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Client.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fedor/Documents/Projects/NetPlay/Client/main.cpp > CMakeFiles/Client.dir/main.cpp.i

CMakeFiles/Client.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Client.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fedor/Documents/Projects/NetPlay/Client/main.cpp -o CMakeFiles/Client.dir/main.cpp.s

CMakeFiles/Client.dir/src/GameManager.cpp.o: CMakeFiles/Client.dir/flags.make
CMakeFiles/Client.dir/src/GameManager.cpp.o: /home/fedor/Documents/Projects/NetPlay/Client/src/GameManager.cpp
CMakeFiles/Client.dir/src/GameManager.cpp.o: CMakeFiles/Client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fedor/Documents/Projects/NetPlay/Client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Client.dir/src/GameManager.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Client.dir/src/GameManager.cpp.o -MF CMakeFiles/Client.dir/src/GameManager.cpp.o.d -o CMakeFiles/Client.dir/src/GameManager.cpp.o -c /home/fedor/Documents/Projects/NetPlay/Client/src/GameManager.cpp

CMakeFiles/Client.dir/src/GameManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Client.dir/src/GameManager.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fedor/Documents/Projects/NetPlay/Client/src/GameManager.cpp > CMakeFiles/Client.dir/src/GameManager.cpp.i

CMakeFiles/Client.dir/src/GameManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Client.dir/src/GameManager.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fedor/Documents/Projects/NetPlay/Client/src/GameManager.cpp -o CMakeFiles/Client.dir/src/GameManager.cpp.s

CMakeFiles/Client.dir/src/Object.cpp.o: CMakeFiles/Client.dir/flags.make
CMakeFiles/Client.dir/src/Object.cpp.o: /home/fedor/Documents/Projects/NetPlay/Client/src/Object.cpp
CMakeFiles/Client.dir/src/Object.cpp.o: CMakeFiles/Client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fedor/Documents/Projects/NetPlay/Client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Client.dir/src/Object.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Client.dir/src/Object.cpp.o -MF CMakeFiles/Client.dir/src/Object.cpp.o.d -o CMakeFiles/Client.dir/src/Object.cpp.o -c /home/fedor/Documents/Projects/NetPlay/Client/src/Object.cpp

CMakeFiles/Client.dir/src/Object.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Client.dir/src/Object.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fedor/Documents/Projects/NetPlay/Client/src/Object.cpp > CMakeFiles/Client.dir/src/Object.cpp.i

CMakeFiles/Client.dir/src/Object.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Client.dir/src/Object.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fedor/Documents/Projects/NetPlay/Client/src/Object.cpp -o CMakeFiles/Client.dir/src/Object.cpp.s

CMakeFiles/Client.dir/src/Fields.cpp.o: CMakeFiles/Client.dir/flags.make
CMakeFiles/Client.dir/src/Fields.cpp.o: /home/fedor/Documents/Projects/NetPlay/Client/src/Fields.cpp
CMakeFiles/Client.dir/src/Fields.cpp.o: CMakeFiles/Client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fedor/Documents/Projects/NetPlay/Client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Client.dir/src/Fields.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Client.dir/src/Fields.cpp.o -MF CMakeFiles/Client.dir/src/Fields.cpp.o.d -o CMakeFiles/Client.dir/src/Fields.cpp.o -c /home/fedor/Documents/Projects/NetPlay/Client/src/Fields.cpp

CMakeFiles/Client.dir/src/Fields.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Client.dir/src/Fields.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fedor/Documents/Projects/NetPlay/Client/src/Fields.cpp > CMakeFiles/Client.dir/src/Fields.cpp.i

CMakeFiles/Client.dir/src/Fields.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Client.dir/src/Fields.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fedor/Documents/Projects/NetPlay/Client/src/Fields.cpp -o CMakeFiles/Client.dir/src/Fields.cpp.s

CMakeFiles/Client.dir/src/PackedData.cpp.o: CMakeFiles/Client.dir/flags.make
CMakeFiles/Client.dir/src/PackedData.cpp.o: /home/fedor/Documents/Projects/NetPlay/Client/src/PackedData.cpp
CMakeFiles/Client.dir/src/PackedData.cpp.o: CMakeFiles/Client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fedor/Documents/Projects/NetPlay/Client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/Client.dir/src/PackedData.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Client.dir/src/PackedData.cpp.o -MF CMakeFiles/Client.dir/src/PackedData.cpp.o.d -o CMakeFiles/Client.dir/src/PackedData.cpp.o -c /home/fedor/Documents/Projects/NetPlay/Client/src/PackedData.cpp

CMakeFiles/Client.dir/src/PackedData.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Client.dir/src/PackedData.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fedor/Documents/Projects/NetPlay/Client/src/PackedData.cpp > CMakeFiles/Client.dir/src/PackedData.cpp.i

CMakeFiles/Client.dir/src/PackedData.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Client.dir/src/PackedData.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fedor/Documents/Projects/NetPlay/Client/src/PackedData.cpp -o CMakeFiles/Client.dir/src/PackedData.cpp.s

CMakeFiles/Client.dir/src/utils/debug.cpp.o: CMakeFiles/Client.dir/flags.make
CMakeFiles/Client.dir/src/utils/debug.cpp.o: /home/fedor/Documents/Projects/NetPlay/Client/src/utils/debug.cpp
CMakeFiles/Client.dir/src/utils/debug.cpp.o: CMakeFiles/Client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fedor/Documents/Projects/NetPlay/Client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/Client.dir/src/utils/debug.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Client.dir/src/utils/debug.cpp.o -MF CMakeFiles/Client.dir/src/utils/debug.cpp.o.d -o CMakeFiles/Client.dir/src/utils/debug.cpp.o -c /home/fedor/Documents/Projects/NetPlay/Client/src/utils/debug.cpp

CMakeFiles/Client.dir/src/utils/debug.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Client.dir/src/utils/debug.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fedor/Documents/Projects/NetPlay/Client/src/utils/debug.cpp > CMakeFiles/Client.dir/src/utils/debug.cpp.i

CMakeFiles/Client.dir/src/utils/debug.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Client.dir/src/utils/debug.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fedor/Documents/Projects/NetPlay/Client/src/utils/debug.cpp -o CMakeFiles/Client.dir/src/utils/debug.cpp.s

CMakeFiles/Client.dir/src/sys/InetNetworker.cpp.o: CMakeFiles/Client.dir/flags.make
CMakeFiles/Client.dir/src/sys/InetNetworker.cpp.o: /home/fedor/Documents/Projects/NetPlay/Client/src/sys/InetNetworker.cpp
CMakeFiles/Client.dir/src/sys/InetNetworker.cpp.o: CMakeFiles/Client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fedor/Documents/Projects/NetPlay/Client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/Client.dir/src/sys/InetNetworker.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Client.dir/src/sys/InetNetworker.cpp.o -MF CMakeFiles/Client.dir/src/sys/InetNetworker.cpp.o.d -o CMakeFiles/Client.dir/src/sys/InetNetworker.cpp.o -c /home/fedor/Documents/Projects/NetPlay/Client/src/sys/InetNetworker.cpp

CMakeFiles/Client.dir/src/sys/InetNetworker.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Client.dir/src/sys/InetNetworker.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fedor/Documents/Projects/NetPlay/Client/src/sys/InetNetworker.cpp > CMakeFiles/Client.dir/src/sys/InetNetworker.cpp.i

CMakeFiles/Client.dir/src/sys/InetNetworker.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Client.dir/src/sys/InetNetworker.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fedor/Documents/Projects/NetPlay/Client/src/sys/InetNetworker.cpp -o CMakeFiles/Client.dir/src/sys/InetNetworker.cpp.s

# Object files for target Client
Client_OBJECTS = \
"CMakeFiles/Client.dir/main.cpp.o" \
"CMakeFiles/Client.dir/src/GameManager.cpp.o" \
"CMakeFiles/Client.dir/src/Object.cpp.o" \
"CMakeFiles/Client.dir/src/Fields.cpp.o" \
"CMakeFiles/Client.dir/src/PackedData.cpp.o" \
"CMakeFiles/Client.dir/src/utils/debug.cpp.o" \
"CMakeFiles/Client.dir/src/sys/InetNetworker.cpp.o"

# External object files for target Client
Client_EXTERNAL_OBJECTS =

Client: CMakeFiles/Client.dir/main.cpp.o
Client: CMakeFiles/Client.dir/src/GameManager.cpp.o
Client: CMakeFiles/Client.dir/src/Object.cpp.o
Client: CMakeFiles/Client.dir/src/Fields.cpp.o
Client: CMakeFiles/Client.dir/src/PackedData.cpp.o
Client: CMakeFiles/Client.dir/src/utils/debug.cpp.o
Client: CMakeFiles/Client.dir/src/sys/InetNetworker.cpp.o
Client: CMakeFiles/Client.dir/build.make
Client: CMakeFiles/Client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/fedor/Documents/Projects/NetPlay/Client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX executable Client"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Client.dir/build: Client
.PHONY : CMakeFiles/Client.dir/build

CMakeFiles/Client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Client.dir/clean

CMakeFiles/Client.dir/depend:
	cd /home/fedor/Documents/Projects/NetPlay/Client/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/fedor/Documents/Projects/NetPlay/Client /home/fedor/Documents/Projects/NetPlay/Client /home/fedor/Documents/Projects/NetPlay/Client/build /home/fedor/Documents/Projects/NetPlay/Client/build /home/fedor/Documents/Projects/NetPlay/Client/build/CMakeFiles/Client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Client.dir/depend

