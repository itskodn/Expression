# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

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
CMAKE_COMMAND = /opt/homebrew/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/kira/expression

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/kira/expression/build

# Include any dependencies generated for this target.
include CMakeFiles/differentiator.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/differentiator.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/differentiator.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/differentiator.dir/flags.make

CMakeFiles/differentiator.dir/codegen:
.PHONY : CMakeFiles/differentiator.dir/codegen

CMakeFiles/differentiator.dir/main.cpp.o: CMakeFiles/differentiator.dir/flags.make
CMakeFiles/differentiator.dir/main.cpp.o: /Users/kira/expression/main.cpp
CMakeFiles/differentiator.dir/main.cpp.o: CMakeFiles/differentiator.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/kira/expression/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/differentiator.dir/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/differentiator.dir/main.cpp.o -MF CMakeFiles/differentiator.dir/main.cpp.o.d -o CMakeFiles/differentiator.dir/main.cpp.o -c /Users/kira/expression/main.cpp

CMakeFiles/differentiator.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/differentiator.dir/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/kira/expression/main.cpp > CMakeFiles/differentiator.dir/main.cpp.i

CMakeFiles/differentiator.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/differentiator.dir/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/kira/expression/main.cpp -o CMakeFiles/differentiator.dir/main.cpp.s

# Object files for target differentiator
differentiator_OBJECTS = \
"CMakeFiles/differentiator.dir/main.cpp.o"

# External object files for target differentiator
differentiator_EXTERNAL_OBJECTS =

differentiator: CMakeFiles/differentiator.dir/main.cpp.o
differentiator: CMakeFiles/differentiator.dir/build.make
differentiator: lib/libgtest.a
differentiator: lib/libgtest_main.a
differentiator: lib/libgtest.a
differentiator: CMakeFiles/differentiator.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/kira/expression/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable differentiator"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/differentiator.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/differentiator.dir/build: differentiator
.PHONY : CMakeFiles/differentiator.dir/build

CMakeFiles/differentiator.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/differentiator.dir/cmake_clean.cmake
.PHONY : CMakeFiles/differentiator.dir/clean

CMakeFiles/differentiator.dir/depend:
	cd /Users/kira/expression/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/kira/expression /Users/kira/expression /Users/kira/expression/build /Users/kira/expression/build /Users/kira/expression/build/CMakeFiles/differentiator.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/differentiator.dir/depend

