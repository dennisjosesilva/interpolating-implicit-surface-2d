# 2D Interpolating implicit function

## Included 3rd-party source code 

**psimpl**

include/Contour/Simplification/psimpl.h is a 3rd-party library 
used for countor simplification. Its features is described in this 
[post](https://www.codeproject.com/Articles/114797/Polyline-Simplification).

## Build

**Requirements:**  C++ 17 compiler, Qt5, OpenGL, Conan and CMake.   

To build the program, we should create a directory and move to it (In Linux):

```shell
$ mkdir build
$ cd build
```

Then, we should install the conan dependecies:

```shell
$ conan install ..
```

Finally, we run cmake and build the source code:

```shell
$ cmake ..
$ make
```

It will produces a binary which can be executed.


