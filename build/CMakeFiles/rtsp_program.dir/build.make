# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/Gene/git/rtsp_project

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/Gene/git/rtsp_project/build

# Include any dependencies generated for this target.
include CMakeFiles/rtsp_program.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/rtsp_program.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/rtsp_program.dir/flags.make

CMakeFiles/rtsp_program.dir/src/main.o: CMakeFiles/rtsp_program.dir/flags.make
CMakeFiles/rtsp_program.dir/src/main.o: ../src/main.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/Gene/git/rtsp_project/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/rtsp_program.dir/src/main.o"
	/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/rtsp_program.dir/src/main.o   -c /home/Gene/git/rtsp_project/src/main.c

CMakeFiles/rtsp_program.dir/src/main.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/rtsp_program.dir/src/main.i"
	/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/Gene/git/rtsp_project/src/main.c > CMakeFiles/rtsp_program.dir/src/main.i

CMakeFiles/rtsp_program.dir/src/main.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/rtsp_program.dir/src/main.s"
	/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/Gene/git/rtsp_project/src/main.c -o CMakeFiles/rtsp_program.dir/src/main.s

CMakeFiles/rtsp_program.dir/src/main.o.requires:
.PHONY : CMakeFiles/rtsp_program.dir/src/main.o.requires

CMakeFiles/rtsp_program.dir/src/main.o.provides: CMakeFiles/rtsp_program.dir/src/main.o.requires
	$(MAKE) -f CMakeFiles/rtsp_program.dir/build.make CMakeFiles/rtsp_program.dir/src/main.o.provides.build
.PHONY : CMakeFiles/rtsp_program.dir/src/main.o.provides

CMakeFiles/rtsp_program.dir/src/main.o.provides.build: CMakeFiles/rtsp_program.dir/src/main.o

# Object files for target rtsp_program
rtsp_program_OBJECTS = \
"CMakeFiles/rtsp_program.dir/src/main.o"

# External object files for target rtsp_program
rtsp_program_EXTERNAL_OBJECTS =

rtsp_program: CMakeFiles/rtsp_program.dir/src/main.o
rtsp_program: CMakeFiles/rtsp_program.dir/build.make
rtsp_program: CMakeFiles/rtsp_program.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable rtsp_program"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/rtsp_program.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/rtsp_program.dir/build: rtsp_program
.PHONY : CMakeFiles/rtsp_program.dir/build

CMakeFiles/rtsp_program.dir/requires: CMakeFiles/rtsp_program.dir/src/main.o.requires
.PHONY : CMakeFiles/rtsp_program.dir/requires

CMakeFiles/rtsp_program.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/rtsp_program.dir/cmake_clean.cmake
.PHONY : CMakeFiles/rtsp_program.dir/clean

CMakeFiles/rtsp_program.dir/depend:
	cd /home/Gene/git/rtsp_project/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/Gene/git/rtsp_project /home/Gene/git/rtsp_project /home/Gene/git/rtsp_project/build /home/Gene/git/rtsp_project/build /home/Gene/git/rtsp_project/build/CMakeFiles/rtsp_program.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/rtsp_program.dir/depend

