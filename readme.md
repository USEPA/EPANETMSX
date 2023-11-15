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
cmake -B build .
cmake --build build --config Release --target package

```

Note: under Windows, the third command should be cmake .. -A Win32 for a 32-bit build or cmake .. -A x64 for a 64-bit build 
when Microsoft Visual Studio is the default compiler. 64-bit EPANETMSX application need to work with 64-bit EPANET2 engine. 

# Documentation

EPANET-MSX manual can be found here: [MSX manual](https://github.com/USEPA/EPANETMSX/blob/master/Doc/EPANETMSX.pdf)
