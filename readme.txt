The EPANET-MSX 1.1 program is free software; you can redistribute
it and/or modify it under the terms of the BSD License.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the license.txt
file for more details.

The contents of this archive are as follows:

\Bin
   epanetmsx.exe        command line version of EPANET-MSX
   epanetmsx.dll        EPANET-MSX function library
   epanet2.dll          standard EPANET function library (release 2.0.12)

\Examples
   readme.txt           descriptions of the example files
   example.inp          example EPANET input file
   example.msx          example MSX input file
   example.rpt          example results file

\Doc
   epanetmsx.pdf        EPANET-MSX users manual
   license.txt          licensing agreement for using EPANET-MSX

\Include
   epanetmsx.h          C/C++ header file for EPANET-MSX function library
   epanetmsx.bas        Visual Basic declarations of EPANET-MSX functions
   epanetmsx.pas        Delphi-Pascal declarations of EPANET-MSX functions
   epanetmsx.lib        Microsoft C/C++ LIB file for epanetmsx.dll
   epanet2.h            C/C++ header file for EPANET2 function library
   epanet2.bas          Visual Basic declarations of EPANET2 functions
   epanet2.pas          Delphi-Pascal declarations of EPANET2 functions
   epanet2.lib          Microsoft C/C++ LIB file for epanet2.dll

\Src
   *.c, *.h             EPANET-MSX source code files

\Make
   \Windows
      readme.txt        instructions for compiling EPANET-MSX on Windows
      msxexe.rsp        command file for compiling epanetmsx.exe
      msxdll.mak        command for compiling epanetmsx.dll
   \Linux
      Makefile          makefile for compiling both a shared object library
                        and a stand-alone executable version of EPANET-MSX
                        using the GNU C++ compiler on the Linux platform
      runepanetmsx.sh   a shell wrapper for running EPANET-MSX under Linux

EPANET-MSX will only run correctly with release 2.0.12 or higher of the
EPANET2 engine which is supplied with this archive. The source code for
EPANET-MSX is copyrighted and is distributed under the BSD License. See
license.txt for the terms of this license.
