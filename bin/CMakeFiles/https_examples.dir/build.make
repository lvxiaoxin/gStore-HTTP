# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.5.1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.5.1/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/lvxin/Desktop/Simple-Web-Server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/lvxin/Desktop/Simple-Web-Server/bin

# Include any dependencies generated for this target.
include CMakeFiles/https_examples.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/https_examples.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/https_examples.dir/flags.make

CMakeFiles/https_examples.dir/https_examples.cpp.o: CMakeFiles/https_examples.dir/flags.make
CMakeFiles/https_examples.dir/https_examples.cpp.o: ../https_examples.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/lvxin/Desktop/Simple-Web-Server/bin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/https_examples.dir/https_examples.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/https_examples.dir/https_examples.cpp.o -c /Users/lvxin/Desktop/Simple-Web-Server/https_examples.cpp

CMakeFiles/https_examples.dir/https_examples.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/https_examples.dir/https_examples.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/lvxin/Desktop/Simple-Web-Server/https_examples.cpp > CMakeFiles/https_examples.dir/https_examples.cpp.i

CMakeFiles/https_examples.dir/https_examples.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/https_examples.dir/https_examples.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/lvxin/Desktop/Simple-Web-Server/https_examples.cpp -o CMakeFiles/https_examples.dir/https_examples.cpp.s

CMakeFiles/https_examples.dir/https_examples.cpp.o.requires:

.PHONY : CMakeFiles/https_examples.dir/https_examples.cpp.o.requires

CMakeFiles/https_examples.dir/https_examples.cpp.o.provides: CMakeFiles/https_examples.dir/https_examples.cpp.o.requires
	$(MAKE) -f CMakeFiles/https_examples.dir/build.make CMakeFiles/https_examples.dir/https_examples.cpp.o.provides.build
.PHONY : CMakeFiles/https_examples.dir/https_examples.cpp.o.provides

CMakeFiles/https_examples.dir/https_examples.cpp.o.provides.build: CMakeFiles/https_examples.dir/https_examples.cpp.o


# Object files for target https_examples
https_examples_OBJECTS = \
"CMakeFiles/https_examples.dir/https_examples.cpp.o"

# External object files for target https_examples
https_examples_EXTERNAL_OBJECTS =

https_examples: CMakeFiles/https_examples.dir/https_examples.cpp.o
https_examples: CMakeFiles/https_examples.dir/build.make
https_examples: /usr/local/lib/libboost_system-mt.dylib
https_examples: /usr/local/lib/libboost_thread-mt.dylib
https_examples: /usr/local/lib/libboost_filesystem-mt.dylib
https_examples: /usr/local/lib/libboost_date_time-mt.dylib
https_examples: /usr/local/opt/openssl/lib/libssl.dylib
https_examples: /usr/local/opt/openssl/lib/libcrypto.dylib
https_examples: CMakeFiles/https_examples.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/lvxin/Desktop/Simple-Web-Server/bin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable https_examples"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/https_examples.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/https_examples.dir/build: https_examples

.PHONY : CMakeFiles/https_examples.dir/build

CMakeFiles/https_examples.dir/requires: CMakeFiles/https_examples.dir/https_examples.cpp.o.requires

.PHONY : CMakeFiles/https_examples.dir/requires

CMakeFiles/https_examples.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/https_examples.dir/cmake_clean.cmake
.PHONY : CMakeFiles/https_examples.dir/clean

CMakeFiles/https_examples.dir/depend:
	cd /Users/lvxin/Desktop/Simple-Web-Server/bin && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/lvxin/Desktop/Simple-Web-Server /Users/lvxin/Desktop/Simple-Web-Server /Users/lvxin/Desktop/Simple-Web-Server/bin /Users/lvxin/Desktop/Simple-Web-Server/bin /Users/lvxin/Desktop/Simple-Web-Server/bin/CMakeFiles/https_examples.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/https_examples.dir/depend

