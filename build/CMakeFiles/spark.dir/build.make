# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/mnt/c/Documents and Settings/Audib/Documents/PersonProject/Compiler"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/mnt/c/Documents and Settings/Audib/Documents/PersonProject/Compiler/build"

# Include any dependencies generated for this target.
include CMakeFiles/spark.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/spark.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/spark.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/spark.dir/flags.make

CMakeFiles/spark.dir/src/Spark.cpp.o: CMakeFiles/spark.dir/flags.make
CMakeFiles/spark.dir/src/Spark.cpp.o: ../src/Spark.cpp
CMakeFiles/spark.dir/src/Spark.cpp.o: CMakeFiles/spark.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/c/Documents and Settings/Audib/Documents/PersonProject/Compiler/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/spark.dir/src/Spark.cpp.o"
	g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/spark.dir/src/Spark.cpp.o -MF CMakeFiles/spark.dir/src/Spark.cpp.o.d -o CMakeFiles/spark.dir/src/Spark.cpp.o -c "/mnt/c/Documents and Settings/Audib/Documents/PersonProject/Compiler/src/Spark.cpp"

CMakeFiles/spark.dir/src/Spark.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/spark.dir/src/Spark.cpp.i"
	g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/mnt/c/Documents and Settings/Audib/Documents/PersonProject/Compiler/src/Spark.cpp" > CMakeFiles/spark.dir/src/Spark.cpp.i

CMakeFiles/spark.dir/src/Spark.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/spark.dir/src/Spark.cpp.s"
	g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/mnt/c/Documents and Settings/Audib/Documents/PersonProject/Compiler/src/Spark.cpp" -o CMakeFiles/spark.dir/src/Spark.cpp.s

# Object files for target spark
spark_OBJECTS = \
"CMakeFiles/spark.dir/src/Spark.cpp.o"

# External object files for target spark
spark_EXTERNAL_OBJECTS =

spark: CMakeFiles/spark.dir/src/Spark.cpp.o
spark: CMakeFiles/spark.dir/build.make
spark: CMakeFiles/spark.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/mnt/c/Documents and Settings/Audib/Documents/PersonProject/Compiler/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable spark"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/spark.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/spark.dir/build: spark
.PHONY : CMakeFiles/spark.dir/build

CMakeFiles/spark.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/spark.dir/cmake_clean.cmake
.PHONY : CMakeFiles/spark.dir/clean

CMakeFiles/spark.dir/depend:
	cd "/mnt/c/Documents and Settings/Audib/Documents/PersonProject/Compiler/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/mnt/c/Documents and Settings/Audib/Documents/PersonProject/Compiler" "/mnt/c/Documents and Settings/Audib/Documents/PersonProject/Compiler" "/mnt/c/Documents and Settings/Audib/Documents/PersonProject/Compiler/build" "/mnt/c/Documents and Settings/Audib/Documents/PersonProject/Compiler/build" "/mnt/c/Documents and Settings/Audib/Documents/PersonProject/Compiler/build/CMakeFiles/spark.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/spark.dir/depend

