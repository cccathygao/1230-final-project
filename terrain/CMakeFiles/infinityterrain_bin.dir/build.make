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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.25.1/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.25.1/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/vincentzhu/infinity-terrain-3d

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/vincentzhu/infinity-terrain-3d

# Include any dependencies generated for this target.
include CMakeFiles/infinityterrain_bin.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/infinityterrain_bin.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/infinityterrain_bin.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/infinityterrain_bin.dir/flags.make

CMakeFiles/infinityterrain_bin.dir/src/lib/Helpers.cpp.o: CMakeFiles/infinityterrain_bin.dir/flags.make
CMakeFiles/infinityterrain_bin.dir/src/lib/Helpers.cpp.o: src/lib/Helpers.cpp
CMakeFiles/infinityterrain_bin.dir/src/lib/Helpers.cpp.o: CMakeFiles/infinityterrain_bin.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/vincentzhu/infinity-terrain-3d/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/infinityterrain_bin.dir/src/lib/Helpers.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/infinityterrain_bin.dir/src/lib/Helpers.cpp.o -MF CMakeFiles/infinityterrain_bin.dir/src/lib/Helpers.cpp.o.d -o CMakeFiles/infinityterrain_bin.dir/src/lib/Helpers.cpp.o -c /Users/vincentzhu/infinity-terrain-3d/src/lib/Helpers.cpp

CMakeFiles/infinityterrain_bin.dir/src/lib/Helpers.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/infinityterrain_bin.dir/src/lib/Helpers.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/vincentzhu/infinity-terrain-3d/src/lib/Helpers.cpp > CMakeFiles/infinityterrain_bin.dir/src/lib/Helpers.cpp.i

CMakeFiles/infinityterrain_bin.dir/src/lib/Helpers.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/infinityterrain_bin.dir/src/lib/Helpers.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/vincentzhu/infinity-terrain-3d/src/lib/Helpers.cpp -o CMakeFiles/infinityterrain_bin.dir/src/lib/Helpers.cpp.s

CMakeFiles/infinityterrain_bin.dir/src/main.cpp.o: CMakeFiles/infinityterrain_bin.dir/flags.make
CMakeFiles/infinityterrain_bin.dir/src/main.cpp.o: src/main.cpp
CMakeFiles/infinityterrain_bin.dir/src/main.cpp.o: CMakeFiles/infinityterrain_bin.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/vincentzhu/infinity-terrain-3d/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/infinityterrain_bin.dir/src/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/infinityterrain_bin.dir/src/main.cpp.o -MF CMakeFiles/infinityterrain_bin.dir/src/main.cpp.o.d -o CMakeFiles/infinityterrain_bin.dir/src/main.cpp.o -c /Users/vincentzhu/infinity-terrain-3d/src/main.cpp

CMakeFiles/infinityterrain_bin.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/infinityterrain_bin.dir/src/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/vincentzhu/infinity-terrain-3d/src/main.cpp > CMakeFiles/infinityterrain_bin.dir/src/main.cpp.i

CMakeFiles/infinityterrain_bin.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/infinityterrain_bin.dir/src/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/vincentzhu/infinity-terrain-3d/src/main.cpp -o CMakeFiles/infinityterrain_bin.dir/src/main.cpp.s

# Object files for target infinityterrain_bin
infinityterrain_bin_OBJECTS = \
"CMakeFiles/infinityterrain_bin.dir/src/lib/Helpers.cpp.o" \
"CMakeFiles/infinityterrain_bin.dir/src/main.cpp.o"

# External object files for target infinityterrain_bin
infinityterrain_bin_EXTERNAL_OBJECTS =

infinityterrain_bin: CMakeFiles/infinityterrain_bin.dir/src/lib/Helpers.cpp.o
infinityterrain_bin: CMakeFiles/infinityterrain_bin.dir/src/main.cpp.o
infinityterrain_bin: CMakeFiles/infinityterrain_bin.dir/build.make
infinityterrain_bin: libglfw3.a
infinityterrain_bin: CMakeFiles/infinityterrain_bin.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/vincentzhu/infinity-terrain-3d/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable infinityterrain_bin"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/infinityterrain_bin.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/infinityterrain_bin.dir/build: infinityterrain_bin
.PHONY : CMakeFiles/infinityterrain_bin.dir/build

CMakeFiles/infinityterrain_bin.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/infinityterrain_bin.dir/cmake_clean.cmake
.PHONY : CMakeFiles/infinityterrain_bin.dir/clean

CMakeFiles/infinityterrain_bin.dir/depend:
	cd /Users/vincentzhu/infinity-terrain-3d && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/vincentzhu/infinity-terrain-3d /Users/vincentzhu/infinity-terrain-3d /Users/vincentzhu/infinity-terrain-3d /Users/vincentzhu/infinity-terrain-3d /Users/vincentzhu/infinity-terrain-3d/CMakeFiles/infinityterrain_bin.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/infinityterrain_bin.dir/depend

