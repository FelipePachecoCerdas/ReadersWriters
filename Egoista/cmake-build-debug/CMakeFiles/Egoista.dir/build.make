# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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
CMAKE_COMMAND = /home/jdtm23/Downloads/CLion-2020.2.4/clion-2020.2.4/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/jdtm23/Downloads/CLion-2020.2.4/clion-2020.2.4/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jdtm23/Documents/ReadersWriters/Egoista

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jdtm23/Documents/ReadersWriters/Egoista/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Egoista.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Egoista.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Egoista.dir/flags.make

CMakeFiles/Egoista.dir/main.c.o: CMakeFiles/Egoista.dir/flags.make
CMakeFiles/Egoista.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jdtm23/Documents/ReadersWriters/Egoista/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Egoista.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Egoista.dir/main.c.o   -c /home/jdtm23/Documents/ReadersWriters/Egoista/main.c

CMakeFiles/Egoista.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Egoista.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jdtm23/Documents/ReadersWriters/Egoista/main.c > CMakeFiles/Egoista.dir/main.c.i

CMakeFiles/Egoista.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Egoista.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jdtm23/Documents/ReadersWriters/Egoista/main.c -o CMakeFiles/Egoista.dir/main.c.s

# Object files for target Egoista
Egoista_OBJECTS = \
"CMakeFiles/Egoista.dir/main.c.o"

# External object files for target Egoista
Egoista_EXTERNAL_OBJECTS =

Egoista: CMakeFiles/Egoista.dir/main.c.o
Egoista: CMakeFiles/Egoista.dir/build.make
Egoista: CMakeFiles/Egoista.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jdtm23/Documents/ReadersWriters/Egoista/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable Egoista"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Egoista.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Egoista.dir/build: Egoista

.PHONY : CMakeFiles/Egoista.dir/build

CMakeFiles/Egoista.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Egoista.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Egoista.dir/clean

CMakeFiles/Egoista.dir/depend:
	cd /home/jdtm23/Documents/ReadersWriters/Egoista/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jdtm23/Documents/ReadersWriters/Egoista /home/jdtm23/Documents/ReadersWriters/Egoista /home/jdtm23/Documents/ReadersWriters/Egoista/cmake-build-debug /home/jdtm23/Documents/ReadersWriters/Egoista/cmake-build-debug /home/jdtm23/Documents/ReadersWriters/Egoista/cmake-build-debug/CMakeFiles/Egoista.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Egoista.dir/depend

