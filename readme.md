# EPANET-MSX 2.0

The EPANET-MSX 2.0 is a free software program and can be used to model water quality problems involving multiple components. 
EPANET-MSX will only run correctly with release 2.2 or higher of the EPANET2 engine. 

## Build Status
[![Build and Test](../../actions/workflows/build-and-test.yml/badge.svg)](../../actions/workflows/build-and-test.yml)


## Build Instructions

CMake (https://cmake.org/) can be used to build EPANETMSX applications. The project's CMake file (CMakeLists.txt) is located in its 
root directory and supports builds for Linux, Mac OS and Windows. To build the EPANETMSX library and its command line executable 
using CMake, first open a console window and navigate to the project's root directory. Then enter the following commands:

```
git submodule init
git submodule update --recursive
cmake -B build .
cmake --build build --config Release --target package
```

# Documentation
EPANET-MSX manual can be found here: [MSX manual](https://github.com/USEPA/EPANETMSX/blob/master/Doc/EPANETMSX.pdf)
