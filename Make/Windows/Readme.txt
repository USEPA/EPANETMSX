These compilation instructions are for the Microsoft C/C++ compiler.

1. Create a sub-directory under the directory where the EPANET-MSX
   source code files are stored and copy MSXDLL.CMD and MSXEXE.CMD
   to it.

2. Make sure that the EPANET2.LIB file is also present in the 
   source code directory.

3. Open a command line window and navigate to the sub-directory.

4. Issue the command:

     CL @MSXEXE.CMD

   to create the command line executable EPANETMSX.EXE.

5. Issue the command:

     CL @MSXDLL.CMD

   to create the function library EPANETMSX.DLL.

The directory will also contain a EPANETMSX.LIB file which will
need to be linked in with any C/C++ application that uses the
library.


