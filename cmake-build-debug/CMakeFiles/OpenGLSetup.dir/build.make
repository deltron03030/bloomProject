# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/yuyan95/Downloads/ClionOpenGLSetUp-Mac-master

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/yuyan95/Downloads/ClionOpenGLSetUp-Mac-master/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/OpenGLSetup.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/OpenGLSetup.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/OpenGLSetup.dir/flags.make

CMakeFiles/OpenGLSetup.dir/main.cpp.o: CMakeFiles/OpenGLSetup.dir/flags.make
CMakeFiles/OpenGLSetup.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yuyan95/Downloads/ClionOpenGLSetUp-Mac-master/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/OpenGLSetup.dir/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/OpenGLSetup.dir/main.cpp.o -c /Users/yuyan95/Downloads/ClionOpenGLSetUp-Mac-master/main.cpp

CMakeFiles/OpenGLSetup.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/OpenGLSetup.dir/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/yuyan95/Downloads/ClionOpenGLSetUp-Mac-master/main.cpp > CMakeFiles/OpenGLSetup.dir/main.cpp.i

CMakeFiles/OpenGLSetup.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/OpenGLSetup.dir/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/yuyan95/Downloads/ClionOpenGLSetUp-Mac-master/main.cpp -o CMakeFiles/OpenGLSetup.dir/main.cpp.s

CMakeFiles/OpenGLSetup.dir/GLAD/src/glad.c.o: CMakeFiles/OpenGLSetup.dir/flags.make
CMakeFiles/OpenGLSetup.dir/GLAD/src/glad.c.o: ../GLAD/src/glad.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yuyan95/Downloads/ClionOpenGLSetUp-Mac-master/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/OpenGLSetup.dir/GLAD/src/glad.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/OpenGLSetup.dir/GLAD/src/glad.c.o -c /Users/yuyan95/Downloads/ClionOpenGLSetUp-Mac-master/GLAD/src/glad.c

CMakeFiles/OpenGLSetup.dir/GLAD/src/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/OpenGLSetup.dir/GLAD/src/glad.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/yuyan95/Downloads/ClionOpenGLSetUp-Mac-master/GLAD/src/glad.c > CMakeFiles/OpenGLSetup.dir/GLAD/src/glad.c.i

CMakeFiles/OpenGLSetup.dir/GLAD/src/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/OpenGLSetup.dir/GLAD/src/glad.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/yuyan95/Downloads/ClionOpenGLSetUp-Mac-master/GLAD/src/glad.c -o CMakeFiles/OpenGLSetup.dir/GLAD/src/glad.c.s

# Object files for target OpenGLSetup
OpenGLSetup_OBJECTS = \
"CMakeFiles/OpenGLSetup.dir/main.cpp.o" \
"CMakeFiles/OpenGLSetup.dir/GLAD/src/glad.c.o"

# External object files for target OpenGLSetup
OpenGLSetup_EXTERNAL_OBJECTS =

../build/OpenGLSetup: CMakeFiles/OpenGLSetup.dir/main.cpp.o
../build/OpenGLSetup: CMakeFiles/OpenGLSetup.dir/GLAD/src/glad.c.o
../build/OpenGLSetup: CMakeFiles/OpenGLSetup.dir/build.make
../build/OpenGLSetup: dependencies/GLFW/src/libglfw3.a
../build/OpenGLSetup: CMakeFiles/OpenGLSetup.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/yuyan95/Downloads/ClionOpenGLSetUp-Mac-master/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ../build/OpenGLSetup"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/OpenGLSetup.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/OpenGLSetup.dir/build: ../build/OpenGLSetup

.PHONY : CMakeFiles/OpenGLSetup.dir/build

CMakeFiles/OpenGLSetup.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/OpenGLSetup.dir/cmake_clean.cmake
.PHONY : CMakeFiles/OpenGLSetup.dir/clean

CMakeFiles/OpenGLSetup.dir/depend:
	cd /Users/yuyan95/Downloads/ClionOpenGLSetUp-Mac-master/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/yuyan95/Downloads/ClionOpenGLSetUp-Mac-master /Users/yuyan95/Downloads/ClionOpenGLSetUp-Mac-master /Users/yuyan95/Downloads/ClionOpenGLSetUp-Mac-master/cmake-build-debug /Users/yuyan95/Downloads/ClionOpenGLSetUp-Mac-master/cmake-build-debug /Users/yuyan95/Downloads/ClionOpenGLSetUp-Mac-master/cmake-build-debug/CMakeFiles/OpenGLSetup.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/OpenGLSetup.dir/depend

