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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/robot/catkin_ws/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/robot/catkin_ws/build

# Utility rule file for clean_test_results_mcthuggets.

# Include the progress variables for this target.
include mcthugget/CMakeFiles/clean_test_results_mcthuggets.dir/progress.make

mcthugget/CMakeFiles/clean_test_results_mcthuggets:
	cd /home/robot/catkin_ws/build/mcthugget && /usr/bin/python /opt/ros/kinetic/share/catkin/cmake/test/remove_test_results.py /home/robot/catkin_ws/build/test_results/mcthuggets

clean_test_results_mcthuggets: mcthugget/CMakeFiles/clean_test_results_mcthuggets
clean_test_results_mcthuggets: mcthugget/CMakeFiles/clean_test_results_mcthuggets.dir/build.make

.PHONY : clean_test_results_mcthuggets

# Rule to build all files generated by this target.
mcthugget/CMakeFiles/clean_test_results_mcthuggets.dir/build: clean_test_results_mcthuggets

.PHONY : mcthugget/CMakeFiles/clean_test_results_mcthuggets.dir/build

mcthugget/CMakeFiles/clean_test_results_mcthuggets.dir/clean:
	cd /home/robot/catkin_ws/build/mcthugget && $(CMAKE_COMMAND) -P CMakeFiles/clean_test_results_mcthuggets.dir/cmake_clean.cmake
.PHONY : mcthugget/CMakeFiles/clean_test_results_mcthuggets.dir/clean

mcthugget/CMakeFiles/clean_test_results_mcthuggets.dir/depend:
	cd /home/robot/catkin_ws/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/robot/catkin_ws/src /home/robot/catkin_ws/src/mcthugget /home/robot/catkin_ws/build /home/robot/catkin_ws/build/mcthugget /home/robot/catkin_ws/build/mcthugget/CMakeFiles/clean_test_results_mcthuggets.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : mcthugget/CMakeFiles/clean_test_results_mcthuggets.dir/depend

