# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /home/david/school/s19/csc471/p3.1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/david/school/s19/csc471/p3.1/build

# Include any dependencies generated for this target.
include CMakeFiles/p3.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/p3.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/p3.dir/flags.make

CMakeFiles/p3.dir/src/GLSL.cpp.o: CMakeFiles/p3.dir/flags.make
CMakeFiles/p3.dir/src/GLSL.cpp.o: ../src/GLSL.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/david/school/s19/csc471/p3.1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/p3.dir/src/GLSL.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/p3.dir/src/GLSL.cpp.o -c /home/david/school/s19/csc471/p3.1/src/GLSL.cpp

CMakeFiles/p3.dir/src/GLSL.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/p3.dir/src/GLSL.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/david/school/s19/csc471/p3.1/src/GLSL.cpp > CMakeFiles/p3.dir/src/GLSL.cpp.i

CMakeFiles/p3.dir/src/GLSL.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/p3.dir/src/GLSL.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/david/school/s19/csc471/p3.1/src/GLSL.cpp -o CMakeFiles/p3.dir/src/GLSL.cpp.s

CMakeFiles/p3.dir/src/GLSL.cpp.o.requires:

.PHONY : CMakeFiles/p3.dir/src/GLSL.cpp.o.requires

CMakeFiles/p3.dir/src/GLSL.cpp.o.provides: CMakeFiles/p3.dir/src/GLSL.cpp.o.requires
	$(MAKE) -f CMakeFiles/p3.dir/build.make CMakeFiles/p3.dir/src/GLSL.cpp.o.provides.build
.PHONY : CMakeFiles/p3.dir/src/GLSL.cpp.o.provides

CMakeFiles/p3.dir/src/GLSL.cpp.o.provides.build: CMakeFiles/p3.dir/src/GLSL.cpp.o


CMakeFiles/p3.dir/src/MatrixStack.cpp.o: CMakeFiles/p3.dir/flags.make
CMakeFiles/p3.dir/src/MatrixStack.cpp.o: ../src/MatrixStack.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/david/school/s19/csc471/p3.1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/p3.dir/src/MatrixStack.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/p3.dir/src/MatrixStack.cpp.o -c /home/david/school/s19/csc471/p3.1/src/MatrixStack.cpp

CMakeFiles/p3.dir/src/MatrixStack.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/p3.dir/src/MatrixStack.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/david/school/s19/csc471/p3.1/src/MatrixStack.cpp > CMakeFiles/p3.dir/src/MatrixStack.cpp.i

CMakeFiles/p3.dir/src/MatrixStack.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/p3.dir/src/MatrixStack.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/david/school/s19/csc471/p3.1/src/MatrixStack.cpp -o CMakeFiles/p3.dir/src/MatrixStack.cpp.s

CMakeFiles/p3.dir/src/MatrixStack.cpp.o.requires:

.PHONY : CMakeFiles/p3.dir/src/MatrixStack.cpp.o.requires

CMakeFiles/p3.dir/src/MatrixStack.cpp.o.provides: CMakeFiles/p3.dir/src/MatrixStack.cpp.o.requires
	$(MAKE) -f CMakeFiles/p3.dir/build.make CMakeFiles/p3.dir/src/MatrixStack.cpp.o.provides.build
.PHONY : CMakeFiles/p3.dir/src/MatrixStack.cpp.o.provides

CMakeFiles/p3.dir/src/MatrixStack.cpp.o.provides.build: CMakeFiles/p3.dir/src/MatrixStack.cpp.o


CMakeFiles/p3.dir/src/Program.cpp.o: CMakeFiles/p3.dir/flags.make
CMakeFiles/p3.dir/src/Program.cpp.o: ../src/Program.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/david/school/s19/csc471/p3.1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/p3.dir/src/Program.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/p3.dir/src/Program.cpp.o -c /home/david/school/s19/csc471/p3.1/src/Program.cpp

CMakeFiles/p3.dir/src/Program.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/p3.dir/src/Program.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/david/school/s19/csc471/p3.1/src/Program.cpp > CMakeFiles/p3.dir/src/Program.cpp.i

CMakeFiles/p3.dir/src/Program.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/p3.dir/src/Program.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/david/school/s19/csc471/p3.1/src/Program.cpp -o CMakeFiles/p3.dir/src/Program.cpp.s

CMakeFiles/p3.dir/src/Program.cpp.o.requires:

.PHONY : CMakeFiles/p3.dir/src/Program.cpp.o.requires

CMakeFiles/p3.dir/src/Program.cpp.o.provides: CMakeFiles/p3.dir/src/Program.cpp.o.requires
	$(MAKE) -f CMakeFiles/p3.dir/build.make CMakeFiles/p3.dir/src/Program.cpp.o.provides.build
.PHONY : CMakeFiles/p3.dir/src/Program.cpp.o.provides

CMakeFiles/p3.dir/src/Program.cpp.o.provides.build: CMakeFiles/p3.dir/src/Program.cpp.o


CMakeFiles/p3.dir/src/Shape.cpp.o: CMakeFiles/p3.dir/flags.make
CMakeFiles/p3.dir/src/Shape.cpp.o: ../src/Shape.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/david/school/s19/csc471/p3.1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/p3.dir/src/Shape.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/p3.dir/src/Shape.cpp.o -c /home/david/school/s19/csc471/p3.1/src/Shape.cpp

CMakeFiles/p3.dir/src/Shape.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/p3.dir/src/Shape.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/david/school/s19/csc471/p3.1/src/Shape.cpp > CMakeFiles/p3.dir/src/Shape.cpp.i

CMakeFiles/p3.dir/src/Shape.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/p3.dir/src/Shape.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/david/school/s19/csc471/p3.1/src/Shape.cpp -o CMakeFiles/p3.dir/src/Shape.cpp.s

CMakeFiles/p3.dir/src/Shape.cpp.o.requires:

.PHONY : CMakeFiles/p3.dir/src/Shape.cpp.o.requires

CMakeFiles/p3.dir/src/Shape.cpp.o.provides: CMakeFiles/p3.dir/src/Shape.cpp.o.requires
	$(MAKE) -f CMakeFiles/p3.dir/build.make CMakeFiles/p3.dir/src/Shape.cpp.o.provides.build
.PHONY : CMakeFiles/p3.dir/src/Shape.cpp.o.provides

CMakeFiles/p3.dir/src/Shape.cpp.o.provides.build: CMakeFiles/p3.dir/src/Shape.cpp.o


CMakeFiles/p3.dir/src/Texture.cpp.o: CMakeFiles/p3.dir/flags.make
CMakeFiles/p3.dir/src/Texture.cpp.o: ../src/Texture.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/david/school/s19/csc471/p3.1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/p3.dir/src/Texture.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/p3.dir/src/Texture.cpp.o -c /home/david/school/s19/csc471/p3.1/src/Texture.cpp

CMakeFiles/p3.dir/src/Texture.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/p3.dir/src/Texture.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/david/school/s19/csc471/p3.1/src/Texture.cpp > CMakeFiles/p3.dir/src/Texture.cpp.i

CMakeFiles/p3.dir/src/Texture.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/p3.dir/src/Texture.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/david/school/s19/csc471/p3.1/src/Texture.cpp -o CMakeFiles/p3.dir/src/Texture.cpp.s

CMakeFiles/p3.dir/src/Texture.cpp.o.requires:

.PHONY : CMakeFiles/p3.dir/src/Texture.cpp.o.requires

CMakeFiles/p3.dir/src/Texture.cpp.o.provides: CMakeFiles/p3.dir/src/Texture.cpp.o.requires
	$(MAKE) -f CMakeFiles/p3.dir/build.make CMakeFiles/p3.dir/src/Texture.cpp.o.provides.build
.PHONY : CMakeFiles/p3.dir/src/Texture.cpp.o.provides

CMakeFiles/p3.dir/src/Texture.cpp.o.provides.build: CMakeFiles/p3.dir/src/Texture.cpp.o


CMakeFiles/p3.dir/src/WindowManager.cpp.o: CMakeFiles/p3.dir/flags.make
CMakeFiles/p3.dir/src/WindowManager.cpp.o: ../src/WindowManager.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/david/school/s19/csc471/p3.1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/p3.dir/src/WindowManager.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/p3.dir/src/WindowManager.cpp.o -c /home/david/school/s19/csc471/p3.1/src/WindowManager.cpp

CMakeFiles/p3.dir/src/WindowManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/p3.dir/src/WindowManager.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/david/school/s19/csc471/p3.1/src/WindowManager.cpp > CMakeFiles/p3.dir/src/WindowManager.cpp.i

CMakeFiles/p3.dir/src/WindowManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/p3.dir/src/WindowManager.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/david/school/s19/csc471/p3.1/src/WindowManager.cpp -o CMakeFiles/p3.dir/src/WindowManager.cpp.s

CMakeFiles/p3.dir/src/WindowManager.cpp.o.requires:

.PHONY : CMakeFiles/p3.dir/src/WindowManager.cpp.o.requires

CMakeFiles/p3.dir/src/WindowManager.cpp.o.provides: CMakeFiles/p3.dir/src/WindowManager.cpp.o.requires
	$(MAKE) -f CMakeFiles/p3.dir/build.make CMakeFiles/p3.dir/src/WindowManager.cpp.o.provides.build
.PHONY : CMakeFiles/p3.dir/src/WindowManager.cpp.o.provides

CMakeFiles/p3.dir/src/WindowManager.cpp.o.provides.build: CMakeFiles/p3.dir/src/WindowManager.cpp.o


CMakeFiles/p3.dir/src/main.cpp.o: CMakeFiles/p3.dir/flags.make
CMakeFiles/p3.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/david/school/s19/csc471/p3.1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/p3.dir/src/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/p3.dir/src/main.cpp.o -c /home/david/school/s19/csc471/p3.1/src/main.cpp

CMakeFiles/p3.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/p3.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/david/school/s19/csc471/p3.1/src/main.cpp > CMakeFiles/p3.dir/src/main.cpp.i

CMakeFiles/p3.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/p3.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/david/school/s19/csc471/p3.1/src/main.cpp -o CMakeFiles/p3.dir/src/main.cpp.s

CMakeFiles/p3.dir/src/main.cpp.o.requires:

.PHONY : CMakeFiles/p3.dir/src/main.cpp.o.requires

CMakeFiles/p3.dir/src/main.cpp.o.provides: CMakeFiles/p3.dir/src/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/p3.dir/build.make CMakeFiles/p3.dir/src/main.cpp.o.provides.build
.PHONY : CMakeFiles/p3.dir/src/main.cpp.o.provides

CMakeFiles/p3.dir/src/main.cpp.o.provides.build: CMakeFiles/p3.dir/src/main.cpp.o


CMakeFiles/p3.dir/src/objectDefs.cpp.o: CMakeFiles/p3.dir/flags.make
CMakeFiles/p3.dir/src/objectDefs.cpp.o: ../src/objectDefs.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/david/school/s19/csc471/p3.1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/p3.dir/src/objectDefs.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/p3.dir/src/objectDefs.cpp.o -c /home/david/school/s19/csc471/p3.1/src/objectDefs.cpp

CMakeFiles/p3.dir/src/objectDefs.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/p3.dir/src/objectDefs.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/david/school/s19/csc471/p3.1/src/objectDefs.cpp > CMakeFiles/p3.dir/src/objectDefs.cpp.i

CMakeFiles/p3.dir/src/objectDefs.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/p3.dir/src/objectDefs.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/david/school/s19/csc471/p3.1/src/objectDefs.cpp -o CMakeFiles/p3.dir/src/objectDefs.cpp.s

CMakeFiles/p3.dir/src/objectDefs.cpp.o.requires:

.PHONY : CMakeFiles/p3.dir/src/objectDefs.cpp.o.requires

CMakeFiles/p3.dir/src/objectDefs.cpp.o.provides: CMakeFiles/p3.dir/src/objectDefs.cpp.o.requires
	$(MAKE) -f CMakeFiles/p3.dir/build.make CMakeFiles/p3.dir/src/objectDefs.cpp.o.provides.build
.PHONY : CMakeFiles/p3.dir/src/objectDefs.cpp.o.provides

CMakeFiles/p3.dir/src/objectDefs.cpp.o.provides.build: CMakeFiles/p3.dir/src/objectDefs.cpp.o


CMakeFiles/p3.dir/src/tifftest.cpp.o: CMakeFiles/p3.dir/flags.make
CMakeFiles/p3.dir/src/tifftest.cpp.o: ../src/tifftest.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/david/school/s19/csc471/p3.1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/p3.dir/src/tifftest.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/p3.dir/src/tifftest.cpp.o -c /home/david/school/s19/csc471/p3.1/src/tifftest.cpp

CMakeFiles/p3.dir/src/tifftest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/p3.dir/src/tifftest.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/david/school/s19/csc471/p3.1/src/tifftest.cpp > CMakeFiles/p3.dir/src/tifftest.cpp.i

CMakeFiles/p3.dir/src/tifftest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/p3.dir/src/tifftest.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/david/school/s19/csc471/p3.1/src/tifftest.cpp -o CMakeFiles/p3.dir/src/tifftest.cpp.s

CMakeFiles/p3.dir/src/tifftest.cpp.o.requires:

.PHONY : CMakeFiles/p3.dir/src/tifftest.cpp.o.requires

CMakeFiles/p3.dir/src/tifftest.cpp.o.provides: CMakeFiles/p3.dir/src/tifftest.cpp.o.requires
	$(MAKE) -f CMakeFiles/p3.dir/build.make CMakeFiles/p3.dir/src/tifftest.cpp.o.provides.build
.PHONY : CMakeFiles/p3.dir/src/tifftest.cpp.o.provides

CMakeFiles/p3.dir/src/tifftest.cpp.o.provides.build: CMakeFiles/p3.dir/src/tifftest.cpp.o


CMakeFiles/p3.dir/ext/tiny_obj_loader/tiny_obj_loader.cpp.o: CMakeFiles/p3.dir/flags.make
CMakeFiles/p3.dir/ext/tiny_obj_loader/tiny_obj_loader.cpp.o: ../ext/tiny_obj_loader/tiny_obj_loader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/david/school/s19/csc471/p3.1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/p3.dir/ext/tiny_obj_loader/tiny_obj_loader.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/p3.dir/ext/tiny_obj_loader/tiny_obj_loader.cpp.o -c /home/david/school/s19/csc471/p3.1/ext/tiny_obj_loader/tiny_obj_loader.cpp

CMakeFiles/p3.dir/ext/tiny_obj_loader/tiny_obj_loader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/p3.dir/ext/tiny_obj_loader/tiny_obj_loader.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/david/school/s19/csc471/p3.1/ext/tiny_obj_loader/tiny_obj_loader.cpp > CMakeFiles/p3.dir/ext/tiny_obj_loader/tiny_obj_loader.cpp.i

CMakeFiles/p3.dir/ext/tiny_obj_loader/tiny_obj_loader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/p3.dir/ext/tiny_obj_loader/tiny_obj_loader.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/david/school/s19/csc471/p3.1/ext/tiny_obj_loader/tiny_obj_loader.cpp -o CMakeFiles/p3.dir/ext/tiny_obj_loader/tiny_obj_loader.cpp.s

CMakeFiles/p3.dir/ext/tiny_obj_loader/tiny_obj_loader.cpp.o.requires:

.PHONY : CMakeFiles/p3.dir/ext/tiny_obj_loader/tiny_obj_loader.cpp.o.requires

CMakeFiles/p3.dir/ext/tiny_obj_loader/tiny_obj_loader.cpp.o.provides: CMakeFiles/p3.dir/ext/tiny_obj_loader/tiny_obj_loader.cpp.o.requires
	$(MAKE) -f CMakeFiles/p3.dir/build.make CMakeFiles/p3.dir/ext/tiny_obj_loader/tiny_obj_loader.cpp.o.provides.build
.PHONY : CMakeFiles/p3.dir/ext/tiny_obj_loader/tiny_obj_loader.cpp.o.provides

CMakeFiles/p3.dir/ext/tiny_obj_loader/tiny_obj_loader.cpp.o.provides.build: CMakeFiles/p3.dir/ext/tiny_obj_loader/tiny_obj_loader.cpp.o


CMakeFiles/p3.dir/ext/glad/src/glad.c.o: CMakeFiles/p3.dir/flags.make
CMakeFiles/p3.dir/ext/glad/src/glad.c.o: ../ext/glad/src/glad.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/david/school/s19/csc471/p3.1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building C object CMakeFiles/p3.dir/ext/glad/src/glad.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/p3.dir/ext/glad/src/glad.c.o   -c /home/david/school/s19/csc471/p3.1/ext/glad/src/glad.c

CMakeFiles/p3.dir/ext/glad/src/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/p3.dir/ext/glad/src/glad.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/david/school/s19/csc471/p3.1/ext/glad/src/glad.c > CMakeFiles/p3.dir/ext/glad/src/glad.c.i

CMakeFiles/p3.dir/ext/glad/src/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/p3.dir/ext/glad/src/glad.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/david/school/s19/csc471/p3.1/ext/glad/src/glad.c -o CMakeFiles/p3.dir/ext/glad/src/glad.c.s

CMakeFiles/p3.dir/ext/glad/src/glad.c.o.requires:

.PHONY : CMakeFiles/p3.dir/ext/glad/src/glad.c.o.requires

CMakeFiles/p3.dir/ext/glad/src/glad.c.o.provides: CMakeFiles/p3.dir/ext/glad/src/glad.c.o.requires
	$(MAKE) -f CMakeFiles/p3.dir/build.make CMakeFiles/p3.dir/ext/glad/src/glad.c.o.provides.build
.PHONY : CMakeFiles/p3.dir/ext/glad/src/glad.c.o.provides

CMakeFiles/p3.dir/ext/glad/src/glad.c.o.provides.build: CMakeFiles/p3.dir/ext/glad/src/glad.c.o


# Object files for target p3
p3_OBJECTS = \
"CMakeFiles/p3.dir/src/GLSL.cpp.o" \
"CMakeFiles/p3.dir/src/MatrixStack.cpp.o" \
"CMakeFiles/p3.dir/src/Program.cpp.o" \
"CMakeFiles/p3.dir/src/Shape.cpp.o" \
"CMakeFiles/p3.dir/src/Texture.cpp.o" \
"CMakeFiles/p3.dir/src/WindowManager.cpp.o" \
"CMakeFiles/p3.dir/src/main.cpp.o" \
"CMakeFiles/p3.dir/src/objectDefs.cpp.o" \
"CMakeFiles/p3.dir/src/tifftest.cpp.o" \
"CMakeFiles/p3.dir/ext/tiny_obj_loader/tiny_obj_loader.cpp.o" \
"CMakeFiles/p3.dir/ext/glad/src/glad.c.o"

# External object files for target p3
p3_EXTERNAL_OBJECTS =

p3: CMakeFiles/p3.dir/src/GLSL.cpp.o
p3: CMakeFiles/p3.dir/src/MatrixStack.cpp.o
p3: CMakeFiles/p3.dir/src/Program.cpp.o
p3: CMakeFiles/p3.dir/src/Shape.cpp.o
p3: CMakeFiles/p3.dir/src/Texture.cpp.o
p3: CMakeFiles/p3.dir/src/WindowManager.cpp.o
p3: CMakeFiles/p3.dir/src/main.cpp.o
p3: CMakeFiles/p3.dir/src/objectDefs.cpp.o
p3: CMakeFiles/p3.dir/src/tifftest.cpp.o
p3: CMakeFiles/p3.dir/ext/tiny_obj_loader/tiny_obj_loader.cpp.o
p3: CMakeFiles/p3.dir/ext/glad/src/glad.c.o
p3: CMakeFiles/p3.dir/build.make
p3: /home/david/school/s19/csc471/deps/glfw-3.2.1/debug/src/libglfw3.a
p3: /usr/lib/x86_64-linux-gnu/librt.so
p3: /usr/lib/x86_64-linux-gnu/libm.so
p3: /usr/lib/x86_64-linux-gnu/libX11.so
p3: /usr/lib/x86_64-linux-gnu/libXrandr.so
p3: /usr/lib/x86_64-linux-gnu/libXinerama.so
p3: /usr/lib/x86_64-linux-gnu/libXxf86vm.so
p3: /usr/lib/x86_64-linux-gnu/libXcursor.so
p3: CMakeFiles/p3.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/david/school/s19/csc471/p3.1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Linking CXX executable p3"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/p3.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/p3.dir/build: p3

.PHONY : CMakeFiles/p3.dir/build

CMakeFiles/p3.dir/requires: CMakeFiles/p3.dir/src/GLSL.cpp.o.requires
CMakeFiles/p3.dir/requires: CMakeFiles/p3.dir/src/MatrixStack.cpp.o.requires
CMakeFiles/p3.dir/requires: CMakeFiles/p3.dir/src/Program.cpp.o.requires
CMakeFiles/p3.dir/requires: CMakeFiles/p3.dir/src/Shape.cpp.o.requires
CMakeFiles/p3.dir/requires: CMakeFiles/p3.dir/src/Texture.cpp.o.requires
CMakeFiles/p3.dir/requires: CMakeFiles/p3.dir/src/WindowManager.cpp.o.requires
CMakeFiles/p3.dir/requires: CMakeFiles/p3.dir/src/main.cpp.o.requires
CMakeFiles/p3.dir/requires: CMakeFiles/p3.dir/src/objectDefs.cpp.o.requires
CMakeFiles/p3.dir/requires: CMakeFiles/p3.dir/src/tifftest.cpp.o.requires
CMakeFiles/p3.dir/requires: CMakeFiles/p3.dir/ext/tiny_obj_loader/tiny_obj_loader.cpp.o.requires
CMakeFiles/p3.dir/requires: CMakeFiles/p3.dir/ext/glad/src/glad.c.o.requires

.PHONY : CMakeFiles/p3.dir/requires

CMakeFiles/p3.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/p3.dir/cmake_clean.cmake
.PHONY : CMakeFiles/p3.dir/clean

CMakeFiles/p3.dir/depend:
	cd /home/david/school/s19/csc471/p3.1/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/david/school/s19/csc471/p3.1 /home/david/school/s19/csc471/p3.1 /home/david/school/s19/csc471/p3.1/build /home/david/school/s19/csc471/p3.1/build /home/david/school/s19/csc471/p3.1/build/CMakeFiles/p3.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/p3.dir/depend
