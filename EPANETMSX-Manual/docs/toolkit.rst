.. raw:: latex

    \renewcommand{\thetable}{\Alph{chapter}.\arabic{table}}
    \renewcommand{\theliteralblock}{\Alph{chapter}.\arabic{literalblock}}

.. highlight:: none

.. _sectoolkit:

MSX TOOLKIT FUNCTIONS
=====================

The EPANET-MSX toolkit is a library of functions that programmers can
use to create their own custom versions of the multi-species extension
of EPANET. The MSX functions are used in conjunction with the standard
EPANET toolkit functions :cite:`Rossman1999` which can also provide additional flexibility
for programmers. Information on using the standard EPANET toolkit is
available through a Windows Help file named TOOLKIT.HLP that is part of
the EN2toolkit.zip file available at the web site
`https://www.epa.gov/water-research/epanet <https://www.epa.gov/water-research/epanet>`__. 
:numref:`t-toolkit1`, :numref:`t-toolkit2`, and :numref:`t-toolkit3` list the name of each MSX toolkit function
along with a brief description of its purpose.

These functions reside in a Windows Dynamic Link Library (DLL) named
*epanetmsx.dll* and can be used in any programming language that can
access DLLs, such as C/C++, Delphi Pascal, Visual Basic, and MatLab. The
toolkit also includes special header files that must be included in any
program modules that reference the MSX functions. These header files are
named *epanetmsx.h* for C/C++ programs, *epanetmsx.pas* for Delphi
programs, and *epanetmsx.bas* for Visual Basic programs.

Prior to using any of the MSX toolkit functions a standard EPANET input
file must be opened using the *ENopen* function from the standard EPANET
toolkit DLL, *epanet2.dll*. In addition, after all processing is
completed the *ENclose* function from the standard toolkit must be
called. Thus the header files for the standard toolkit (*epanet2.h*,
*epanet2.pas*, or *epanet2.bas*) must also be included in the
application's code. Finally, if a stand-alone command line executable is
being produced from C/C++ then the LIB files *epanet2.lib* and
*epanetmsx.lib* must be linked in when the compiled source files are
linked together. (Note: The LIB files supplied with the EPANET-MSX
distribution are compatible with the Microsoft C/C++ compiler version 6
and higher.)

The following pages provide a description of each toolkit function using
C/C++ syntax to represent argument variables and return types.

.. tabularcolumns:: |p{4cm}|p{11cm}|

.. _t-toolkit1:
.. table:: EPANET-MSX toolkit processing functions

 +--------------------+-------------------------------------------------+
 | **Function Name**  | **Purpose**                                     |
 +====================+=================================================+
 | MSXopen            | Opens the EPANET-MSX toolkit system.            |  
 +--------------------+-------------------------------------------------+
 | MSXclose           | Closes the EPANET-MSX toolkit system.           |
 +--------------------+-------------------------------------------------+
 | MSXsolveH          | Solves for system hydraulics over the entire    |
 |                    | simulation period, saving results to an         |
 |                    | internal scratch file.                          |
 +--------------------+-------------------------------------------------+
 | MSXusehydfile      | Uses a previously saved EPANET hydraulics file  |
 |                    | as the source of hydraulic information.         |
 +--------------------+-------------------------------------------------+
 | MSXsolveQ          | Solves for water quality over the entire        |
 |                    | simulation period and saves the results to an   |
 |                    | internal scratch file.                          |
 +--------------------+-------------------------------------------------+
 | MSXinit            | Initializes the MSX system before solving for   |
 |                    | water quality results in a step-wise fashion.   |
 +--------------------+-------------------------------------------------+
 | MSXstep            | Advances the water quality solution through a   |
 |                    | single water quality time step when performing  |
 |                    | a step-wise simulation.                         |
 +--------------------+-------------------------------------------------+
 | MSXsaveoutfile     | Saves water quality results computed for each   |
 |                    | node, link and reporting time period to a named |
 |                    | binary file.                                    |
 +--------------------+-------------------------------------------------+
 | MSXsavemsxfile     | Saves the data associated with the current MSX  |
 |                    | project into a new MSX input file.              |
 +--------------------+-------------------------------------------------+
 | MSXreport          | Writes water quality simulations results as     |
 |                    | instructed by the MSX input file to a text      |
 |                    | file.                                           |
 +--------------------+-------------------------------------------------+

..

.. tabularcolumns:: |p{4cm}|p{11cm}|

.. _t-toolkit2:
.. table:: EPANET-MSX toolkit data retrieval functions

 +--------------------+-------------------------------------------------+
 | **Function Name**  | **Purpose**                                     |
 +====================+=================================================+
 | MSXgetindex        | Retrieves the internal index number of an MSX   |
 |                    | object given its ID name.                       |
 +--------------------+-------------------------------------------------+
 | MSXgetIDlen        | Retrieves the number of characters in the ID    |
 |                    | name of an MSX object given its internal index  |
 |                    | number.                                         |
 +--------------------+-------------------------------------------------+
 | MSXgetID           | Retrieves the ID name of an MSX object given    |
 |                    | its internal index number.                      |
 +--------------------+-------------------------------------------------+
 | MSXgetcount        | Retrieves the number of objects of a specific   |
 |                    | type.                                           |
 +--------------------+-------------------------------------------------+
 | MSXgetspecies      | Retrieves the attributes of a chemical species  |
 |                    | given its internal index number.                |
 +--------------------+-------------------------------------------------+
 | MSXgetinitqual     | Retrieves the initial concentration of a        |
 |                    | particular chemical species assigned to a       |
 |                    | specific node or link of the pipe network.      |
 +--------------------+-------------------------------------------------+
 | MSXgetqual         | Retrieves the concentration of a chemical       |
 |                    | species at a specific node or link of the       |
 |                    | network at the current simulation time step.    |
 +--------------------+-------------------------------------------------+
 | MSXgetconstant     | Retrieves the value of a particular reaction    |
 |                    | constant.                                       |
 +--------------------+-------------------------------------------------+
 | MSXgetparameter    | Retrieves the value of a particular reaction    |
 |                    | parameter for a given pipe or tank within the   |
 |                    | pipe network.                                   |
 +--------------------+-------------------------------------------------+
 | MSXgetsource       | Retrieves information on any external source of |
 |                    | a particular chemical species assigned to a     |
 |                    | specific node of the pipe network.              | 
 +--------------------+-------------------------------------------------+
 | MSXgetpatternlen   | Retrieves the number of time periods within a   |
 |                    | source time pattern.                            |
 +--------------------+-------------------------------------------------+
 | MSXgetpatternvalue | Retrieves the multiplier at a specific time     |
 |                    | period for a given source time pattern.         |
 +--------------------+-------------------------------------------------+
 | MSXgeterror        | Returns the text for an error message given its |
 |                    | error code.                                     |
 +--------------------+-------------------------------------------------+

.. 

.. tabularcolumns:: |p{4cm}|p{11cm}|

.. _t-toolkit3:
.. table:: EPANET-MSX data modification functions

 +--------------------+-------------------------------------------------+
 | **Function Name**  | **Purpose**                                     |
 +====================+=================================================+
 | MSXsetconstant     | Assigns a new value to a specific reaction      |
 |                    | constant.                                       |
 +--------------------+-------------------------------------------------+
 | MSXsetparameter    | Assigns a value to a particular reaction        |
 |                    | parameter for a given pipe or tank within the   |
 |                    | pipe network.                                   |
 +--------------------+-------------------------------------------------+
 | MSXsetinitqual     | Assigns an initial concentration of a           |
 |                    | particular chemical species to a specific node  |
 |                    | or link of the pipe network.                    |
 +--------------------+-------------------------------------------------+
 | MSXsetsource       | Sets the attributes of an external source of a  |
 |                    | particular chemical species to a specific node  |
 |                    | of the pipe network.                            |
 +--------------------+-------------------------------------------------+
 | MSXsetpattern      | Assigns a new set of multipliers to a given MSX |
 |                    | source time pattern.                            |
 +--------------------+-------------------------------------------------+
 | MSXsetpatternvalue | Assigns a new value to the multiplier for a     |
 |                    | specific time period in a given MSX source time |
 |                    | pattern.                                        |
 +--------------------+-------------------------------------------------+
 | MSXaddpattern      | Adds a new, empty MSX source time pattern to    |
 |                    | the project.                                    |
 +--------------------+-------------------------------------------------+

..

MSXopen
-------

**Declaration:**

   ::

      int MSXopen(char * f);

**Description:**

   Opens the EPANET-MSX toolkit system.

**Arguments:**

   f is a C-style character string containing the name of an EPANET-MSX input file.

**Returns:**

   Returns an error code or 0 for no error.

**Notes:**

   The standard EPANET toolkit function ENopen must have been called
   first to open the EPANET toolkit along with an associated EPANET
   input file for the network being analyzed as well as to identify the
   name of a report file to which results are written.

**Example:**

   .. code-block:: C

      //Open the EPANET toolkit

      int err = ENopen("example1.inp", "example1.rpt", "");

      //Open the MSX toolkit

      if (err == 0) err = MSXopen("example1.msx");

      //Add code to perform required analyses here

      if (err == 0) ...

      //Don't forget to close the toolkits

      MSXclose();

      ENclose();

      exit(err);

MSXclose
--------

**Declaration:**

   ::

      int MSXclose(void);

**Description:**

   Closes the EPANET-MSX toolkit system.

**Arguments:**

   None.

**Returns:**

   Returns an error code or 0 for no error.

**Notes:**

   The EPANET toolkit function ENclose should be called at some point
   after calling MSXclose to close the EPANET toolkit system.

**Example:**

   .. code-block:: C

      //Open the EPANET toolkit

      int err = ENopen("example1.inp", "example1.rpt", "");

      //Open the MSX toolkit

      if (err == 0) err = MSXopen("example1.msx");

      //Add code to perform required analyses here

      if (err == 0) ...

      //Don't forget to close both toolkits

      MSXclose();

      ENclose();
   

MSXsolveH
---------

**Declaration:**

   .. code-block::

      int MSXsolveH(void);

**Description:**

   Solves for system hydraulics over the entire simulation period and saves
   results to an internal scratch file.

**Arguments:**

   None.

**Returns:**

   Returns an error code or 0 for no error.

**Notes:**

   Either this function or MSXusehydfile (see below) must be called
   before any water quality processing is performed.

**Example:**

   .. code-block:: c

      //Open the EPANET & MSX toolkits

      int err = ENopen("example1.inp", "example1.rpt", "");

      if (err == 0) MSXopen("example1.msx");

      //Solve for hydraulics

      if (err == 0) err = MSXsolveH();

      //Perform water quality analysis starting here

      ...


MSXusehydfile
-------------

**Declaration:**

   .. code-block::

      int MSXusehydfile(char * f);

**Description:**

   Uses a previously saved EPANET hydraulics file as the source of
   hydraulic information.

**Arguments:**

   f is a C-style character string containing the name of a previously
   saved hydraulics file for the system being analyzed.

**Returns:**

   Returns an error code or 0 for no error.

**Notes:**

   Either this function or MSXsolveH (see above) must be called before
   any water quality processing is performed.

**Example:**

   .. code-block:: c

      //Open the EPANET toolkit

      int err = ENopen("example1.inp", "example1.rpt", "");

      if (err > 0) return err;

      //Use EPANET to solve & save hydraulic results

      ENsolveH();

      ENsavehydfile("example1.hyd");

      //Open the MSX toolkit

      err = MSXopen("example1.msx");

      if (err > 0) return err;

      //Utilize the hydraulic solution just saved to file

      err = MSXusehydfile("example1.hyd");

      //Perform water quality analysis starting here

      ...

MSXsolveQ
---------

**Declaration:**

   .. code-block::

      int MSXsolveQ(void);

**Description:**

   Solves for water quality over the entire simulation period and saves the results to an internal scratch file.

**Arguments:**

   None.

**Returns:**

   Returns an error code or 0 for no error.

**Notes:**

   This function does not allow access to computed water quality results
   as the simulation unfolds. If such information is required, use
   MSXinit in conjunction with step-wise calls to MSXstep (see below).

**Example:**

   .. code-block:: c

      //Open the EPANET & MSX toolkits

      int err = ENopen("example1.inp", "example1.rpt", "");

      if (err == 0) err = MSXopen("example1.msx");

      if (err > 0) return err;

      //Solve for hydraulics & water quality

      MSXsolveH();

      MSXsolveQ();

      //Report results

      MSXreport();

      //Close the toolkits

      MSXclose();

      ENclose();

MSXinit
-------

**Declaration:**

   .. code-block::

      int MSXinit(int saveFlag);

**Purpose:**

   Initializes the MSX system before solving for water quality results
   in step-wise fashion.

**Arguments:**

   Set saveFlag to 1 if water quality results should be saved to a
   scratch binary file, or to 0 if results are not saved to file.

**Returns:**

   Returns an error code or 0 for no error.

**Notes:**

   This function must be called before a step-wise water quality
   simulation is performed using MSXstep. Do not call this function if
   performing a complete simulation using MSXsolveQ.

**Example:**

   See the example provided for MSXstep.

MSXstep
-------

**Declaration:**

   .. code-block::

      int MSXstep(double * t, double * tleft);

**Description:**

   Advances the water quality solution through a single water quality
   time step when performing a step-wise simulation.

**Arguments:**

   Upon returning, t will contain the current simulation time at the end
   of the step (in seconds) while tleft will contain the time left in
   the simulation (also in seconds).

**Returns:**

   Returns an error code or 0 for no error.

**Notes:**

   This function should be placed in a loop that repeats until the value
   of tleft becomes 0. MSXinit should be called before beginning the
   loop.

   The water quality time step used by this function is specified in the
   :ref:`options` section of the MSX input file.

**Example:**

   .. code-block:: c

      //Declare time variables

      double t = 0.0, tleft = 0.0;

      int err;

      //Open the EPANET & MSX toolkits

      ...

      //Solve for hydraulics

      MSXsolveH();

      //Run a water quality simulation

      MSXinit(0);

      do {

            err = MSXstep(&t, &tleft);

            //Use MSXgetqual to retrieve results at time t

         } while (tleft > 0.0 && err == 0);


MSXsaveoutfile
--------------

**Declaration:**

   .. code-block::

      int MSXsaveoutfile(char * f);

**Description:**

   Saves water quality results computed for each node, link and reporting time period to a named binary file.

**Arguments:**

   f is a C-style character string containing the name of the permanent output results file.

**Returns:**

   Returns an error code or 0 for no error.

**Example:**

   .. code-block:: c

      //Open the EPANET & MSX toolkits

      ...

      //Solve for hydraulics & water quality

      MSXsolveH();

      MSXsolveQ();

      //Copy saved results to a permanent file

      MSXsaveoutfile("example1.out");

      //Close the toolkits

      ...

MSXsavemsxfile
--------------

**Declaration:**

   .. code-block::

      int MSXsavemsxfile(char * f);

**Description:**

   Saves the data associated with the current MSX project into a new MSX input file.

**Arguments:**

   f is a C-style character string containing the name of the file to which data are saved.

**Returns:**

   Returns an error code or 0 for no error.

**Notes:**

   For a step-wise simulation using MSXstep, this function only applies
   if MSXinit was called with its saveFlag parameter set to 1 (see
   MSXinit).

   The format of the binary results file is described in :ref:`binary`.

**Example:**

   .. code-block:: c

      //Open the EPANET & MSX toolkits

      int err = ENopen("example1.inp", "example1.rpt", "");

      if (err == 0) err = MSXopen("example1.msx");

      if (err > 0) return err;

      //Save the current MSX data to a different MSX file

      MSXsavemsxfile("example1a.msx");

      //Close the toolkits

      ...

MSXreport
---------

**Declaration:**

   .. code-block::

      int MSXreport(void);

**Description:**

   Writes water quality simulations results as instructed by the MSX
   input file to a text file.

**Arguments:**

   None.

**Returns:**

   Returns an error code or 0 for no error.

**Notes:**

   Results are written to the report file specified in the ENopen
   function, unless a specific water quality report file is named in the
   :ref:`report` section of the MSX input file.

**Example:**

   .. code-block:: c

      //Open the EPANET & MSX toolkits

      ...

      //Solve for hydraulics & water quality

      MSXsolveH();

      MSXsolveQ();

      //Write results to the "example1.rpt" file

      MSXreport();

      //Close the toolkits

      ...

MSXgetindex
-----------

**Declaration:**

   .. code-block::

      int MSXgetindex(int type, char * name, int * index);

**Description:**

   Retrieves the internal index number of an MSX object given its name.

**Arguments:**

   type is the type of object being sought and must be one of the
   following pre-defined constants::

      MSX_SPECIES    (for a chemical species)

      MSX_CONSTANT   (for a reaction constant)

      MSX_PARAMETER  (for a reaction parameter)

      MSX_PATTERN    (for a time pattern)

   name is a C-style character string containing the object's ID name;

   index is the sequence number (starting from 1) of the object in the
   order it was listed in the MSX input file. 

**Returns:**

   Returns an error code or 0 for no error.

**Example:**

   .. code-block:: c

      //Declare an index variable

      int i;

      //Open the EPANET & MSX toolkits

      ...

      //Get the index of the chemical species named "CL2"

      MSXgetindex(MSX_SPECIES, "CL2", &i);

MSXgetIDlen
-----------

**Declaration:**

   .. code-block::

      int MSXgetIDlen(int type, int index, int * len);

**Description:**

   Retrieves the number of characters in the ID name of an MSX object
   given its internal index number.

**Arguments:**

   type is the type of object being sought and must be one of the following pre-defined constants::

      MSX_SPECIES    (for a chemical species)

      MSX_CONSTANT   (for a reaction constant)

      MSX_PARAMETER  (for a reaction parameter)

      MSX_PATTERN    (for a time pattern)

   index is the sequence number of the object (starting from 1 as listed
   in the MSX input file);

   len is returned with the number of characters in the object's ID
   name, not counting the *null* termination character.

**Returns:**

   Returns an error code or 0 for no error.

**Example:**

   .. code-block:: c

      //This code finds the longest species name within a project

      //Declare some variables

      int count, i, len, maxlen = 0;

      //Open the EPANET & MSX toolkits

      ...

      //Examine each species

      MSXgetcount(MSX_SPECIES, &count);

      for (i=1; i<=count; i++) {

      //Update longest species name

         MSXgetIDlen(MSX_SPECIES, i, &len);

         if (len > maxlen) maxlen = len;

      }

MSXgetID
-------------

**Declaration:**

   .. code-block::

      int MSXgetID(int type, int index, char * id, int len);

**Description:**

   Retrieves the ID name of an object given its internal index number.

**Arguments:**

   type is the type of object being sought and must be one of the
   following pre-defined constants::

      MSX_SPECIES    (for a chemical species)

      MSX_CONSTANT   (for a reaction constant)

      MSX_PARAMETER  (for a reaction parameter)

      MSX_PATTERN    (for a time pattern)

   index is the sequence number of the object (starting from 1 as listed
   in the MSX input file);

   id is a C-style character string that is returned with the object's
   ID name.

   len is the maximum number of characters that id can hold, not
   counting the *null* termination character.

**Returns:**

   Returns an error code or 0 for no error.

**Notes:**

   The MSXgetIDlen function can determine the number of characters in an
   object's ID name so that the character array id can be properly sized
   (to len + 1).

**Example:**

   .. code-block:: c

      //Declare a string to hold a species ID

      char id[16];

      //Open the EPANET & MSX toolkits

      ...

      //Get the name of the 2nd species in the MSX input file

      MSXgetID(MSX_SPECIES, 2, id, sizeof(id)-1);

MSXgetcount
------------

**Declaration:**

   .. code-block::

      int MSXgetcount(int type, int * count);

**Description:**

   Retrieves the number of objects of a specific type.

**Arguments:**

   type is the type of object being sought and must be one of the following pre-defined constants::

      MSX_SPECIES    (for a chemical species)

      MSX_CONSTANT   (for a reaction constant)

      MSX_PARAMETER  (for a reaction parameter)

      MSX_PATTERN    (for a time pattern)

   count is the number of objects of that type defined in the MSX input file.

**Returns:**

   Returns an error code or 0 for no error.

**Example:**

   .. code-block:: c

      //Declare a variable for the number of chemical species

      int nSpecies;

      //Open the EPANET & MSX toolkits

      ...

      //Get the number of species

      MSXgetcount(MSX_SPECIES, &nSpecies);

MSXgetspecies
-------------

**Declaration:**

   .. code-block::

      int MSXgetspecies(int species, int * type, char * units, double * aTol, 
                        double * rTol);

**Description:**

   Retrieves the attributes of a chemical species given its internal
   index number.

**Arguments:**

   species is the sequence number of the species (starting from 1 as
   listed in the MSX input file);

   type is returned with one of the following pre-defined constants::

      MSX_BULK    (defined as 0)  for a bulk water species

      MSX_WALL    (defined as 1)  for a pipe wall surface species

   units is a C-style character string array that is returned with the
   mass units that were defined for the species in question. It must be
   sized to hold a maximum of 15 characters plus the terminating null
   character (for a total of 16).

   aTol is returned with the absolute concentration tolerance defined
   for the species (in concentration units);

   rTol is returned with the relative concentration tolerance defined
   for the species.

**Returns:**

   Returns an error code or 0 for no error.

**Example:**

   .. code-block:: c

      //Declare some variables

      int sIndex, sType, sUnits;

      double aTol, rTol;

      //Open the EPANET & MSX toolkits

      ...

      //Get attributes of the species named "Xwall"

      MSXgetindex(MSX_SPECIES, "Xwall", &sIndex);

      MSXgetspecie(sIndex, &sType, &sUnits, &aTol, &rTol);

MSXgetinitqual
---------------

**Declaration:**

   .. code-block::

      int MSXgetinitqual(int obj, int index, int species, double * value);

**Description:**

   Retrieves the initial concentration of a particular chemical species
   assigned to a specific node or link of the pipe network.

**Arguments:**

   obj is type of object being queried and must be either::

      MSX_NODE    (defined as 0)  for a node

      MSX_LINK    (defined as 1)  for a link

   index is the internal sequence number (starting from 1) assigned to
   the node or link;

   species is the sequence number of the species (starting from 1);

   value is returned with the initial concentration of the species at
   the node or link of interest.

**Returns:**

   Returns an error code or 0 for no error.

**Notes:**

   The EPANET toolkit functions ENgetnodeindex and ENgetlinkindex can be
   used to identify the index of a node or link from its ID name;

   Concentrations are expressed as mass units per liter for bulk species
   and as mass per unit area for surface species.

**Example:**

   .. code-block:: c

      int n, s;

      double c0;

      //Open the EPANET & MSX toolkits

      ...

      //Get initial concentration Of "CL2" in "Tank_A"

      ENgetnodeindex("Tank_A", &n);

      MSXgetindex(MSX_SPECIES, "CL2", &s);

      MSXgetinitqual(MSX_NODE, n, s, &c0);

MSXgetqual
----------

**Declaration:**

   .. code-block::

      int MSXgetqual(int obj, int index, int species, double * value);

**Description:**

   Retrieves a chemical species concentration at a given node or the
   average concentration along a link at the current simulation time
   step.

**Arguments:**

   obj is type of object being queried and must be either::

      MSX_NODE    (defined as 0)  for a node

      MSX_LINK    (defined as 1)  for a link

   index is the internal sequence number (starting from 1) assigned to
   the node or link;

   species is the sequence number of the species (starting from 1 as
   listed in the MSX input file);

   value is returned with the computed concentration of the species at
   the current time period.

**Returns:**

   Returns an error code or 0 for no error.

**Notes:**

   The EPANET toolkit functions ENgetnodeindex and ENgetlinkindex can be
   used to identify the index of a node or link from its ID name;

   Concentrations are expressed as mass units per liter for bulk species
   and as mass per unit area for surface species.

**Example:**

   .. code-block:: c

      //Declare some variables

      long t, tstep;

      int n, s;

      double c, cMax = 0.0;

      //Open the EPANET & MSX toolkits

      ...

      //Get the indexes of node "Tank_A" and species "CL2"

      ENgetnodeindex("Tank_A", &n);

      MSXgetindex(MSX_SPECIES, "CL2", &s);

      //Obtain a hydraulic solution

      MSXsolveH();

      //Run a step-wise water quality analysis

      //without saving results to file

      MSXinit(0);

      do {

            err = MSXstep(&t, &tleft);

            //Retrieve CL2 concentration at Tank_A

            MSXgetqual(MSX_NODE, n, s, &c);

           //Update the max. concentration

            if (c > cMax) cMax = c;

         } while (tleft > 0 && err == 0);

      //Close the toolkits

      ...

MSXgetconstant
--------------

**Declaration:**

   .. code-block::

      int MSXgetconstant(int index, double * value);

**Description:**

   Retrieves the value of a particular reaction constant.

**Arguments:**

   index is the sequence number of the reaction constant (starting from
   1) as it appeared in the MSX input file;

   value is returned with the value assigned to the constant.

**Returns:**

   Returns an error code or 0 for no error.

**Example:**

   .. code-block:: c

      //Declare some variables

      int i;

      double k1;

      //Open the EPANET & MSX toolkits

      ...

      //Get the index of the constant named K1

      MSXgetindex(MSX_CONSTANT, "K1", &i);

      //Get the value of K1

      MSXgetconstant(i, &k1);

MSXgetparameter
---------------

**Declaration:**

   .. code-block::

      int MSXgetparameter(int obj, int index, int param, double * value);

**Description:**

   Retrieves the value of a particular reaction parameter for a given
   pipe or tank within the pipe network.

**Arguments:**

   obj is type of object being queried and must be either::

      MSX_NODE    (defined as 0)  for a node

      MSX_LINK    (defined as 1)  for a link

   index is the internal sequence number (starting from 1) assigned to
   the node or link;

   param is the sequence number of the parameter (starting from 1 as
   listed in the MSX input file);

   value is returned with the value assigned to the parameter for the
   node or link of interest.

**Returns:**

   Returns an error code or 0 for no error.

**Notes:**

   Reaction parameters are only defined for storage tank nodes and pipe
   links. All other types of nodes and links have parameter values of 0.

**Example:**

   .. code-block:: c

      //Declare some variables

      int i, j;

      double k2;

      //Open the EPANET & MSX toolkits

      ...

      //Get the value of parameter "K2" for pipe "P1"

      ENgetlinkindex("P1", &i);

      MSXgetindex(MSX_PARAMETER, "K2", &j);

      MSXgetparameter(MSX_LINK, i, j, &k2);


MSXgetsource
------------

**Declaration:**

   .. code-block::

      int MSXgetsource(int node, int species, int * type, double * level, int * pat);

**Description:**

   Retrieves information on any external source of a particular chemical
   species assigned to a specific node of the pipe network.

**Arguments:**

   node is the internal sequence number (starting from 1) assigned to
   the node of interest;

   species is the sequence number of the species of interest (starting
   from 1 as listed in the MSX input file);

   type is returned with the type of external source and will be one of
   the following pre-defined constants::

      MSX_NOSOURCE   (defined as -1) for no source

      MSX_CONCEN     (defined as 0)  for a concentration source

      MSX_MASS       (defined as 1)  for a mass booster source

      MSX_SETPOINT   (defined as 2)  for a setpoint source

      MSX_FLOWPACED  (defined as 3)  for a flow paced source

   The meaning of these source types can be found in the description of the :ref:`sources` section of the MSX input file in :ref:`inputformat` of this manual.

   level is returned with the baseline concentration (or mass flow rate)
   of the source;

   pat is returned with the index of the time pattern used to add
   variability to the source's baseline level (and will be 0 if no
   pattern was defined for the source).

**Returns:**

   Returns an error code or 0 for no error.

**Example:**

   .. code-block:: c

      //Declare some variables

      int n, s, t, p;

      double c;

      //Open the EPANET & MSX toolkits

      ...

      //Get source information for species CL2 at node N1

      ENgetnodeindex("N1", &n);

      MSXgetindex(MSX_SPECIES, "CL2", &s);

      MSXgetsource(n, s, &t, &c, &p);

MSXgetpatternlen
----------------

**Declaration:**

   .. code-block::

      int MSXgetpatternlen(int pat, int * len);

**Description:**

   Retrieves the number of time periods within a source time pattern.

**Arguments:**

   pat is the internal sequence number (starting from 1) of the pattern
   as it appears in the MSX input file;

   len is returned with the number of time periods (and therefore number
   of multipliers) that appear in the pattern.

**Returns:**

   Returns an error code or 0 for no error.

**Notes:**

   This function only applies to source time patterns that appear in the
   MSX input file. There is a comparable EPANET toolkit function,
   ENgetpatternlen, which can be used for the demand patterns defined in
   the EPANET input file.

**Example:**

   .. code-block:: c

      //Declare some variables

      int i, n;

      //Open the EPANET & MSX toolkits

      ...

      //Get the number of multipliers (n) in pattern "P1"

      MSXgetindex("P1", &i);

      MSXgetpatternlen(i, &n);


MSXgetpatternvalue
------------------

**Declaration:**

   .. code-block::

      int MSXgetpatternvalue(int pat, int period, double * value);

**Description:**

   Retrieves the multiplier at a specific time period for a given source
   time pattern.

**Arguments:**

   pat is the internal sequence number (starting from 1) of the pattern
   as it appears in the MSX input file;

   period is the index of the time period (starting from 1) whose
   multiplier is being sought;

   value is returned with the value of the pattern's multiplier in the
   desired period.

**Returns:**

   Returns an error code or 0 for no error.

**Notes:**

   This function only applies to source time patterns that appear in the
   MSX input file. There is a comparable EPANET toolkit function,
   Engetpatternvalue, which can be used for the demand patterns defined
   in the EPANET input file.

**Example:**

   .. code-block:: c

      //Declare some variables

      int i, n;

      //Open the EPANET & MSX toolkits

      ...

      //Get the number of multipliers (n) in pattern "P1"

      MSXgetindex("P1", &i);

      MSXgetpatternlen(i, &n);


MSXgeterror
-----------

**Declaration:**

   .. code-block::

      int MSXgeterror(int code, char * msg, int len);

**Description:**

   Returns the text for an error message given its error code.

**Arguments:**

   code is the code number of an error condition generated by
   EPANET-MSX;

   msg is a C-style character string that is returned containing the
   text of the error message corresponding to the error code;

   len is the maximum number of characters that msg can contain.

**Returns:**

   Returns an error code or 0 for no error.

**Notes:**

   msg should be sized to accept a minimum of 80 characters.

   This function only applies to error codes generated by the MSX
   toolkit. There is a comparable EPANET toolkit function, ENgeterror,
   that applies to EPANET errors.

**Example:**

   .. code-block:: c

      char msg[81];

      //Open the EPANET toolkit & check for errors

      int err = Enopen("example1.inp", "example1.rpt", "");

      if (err > 0) ENgeterror(err, msg);

      //Open the MSX toolkit & check for errors

      else {

         err = MSXopen("example1.msx");

         if (err > 0) MSXgeterror(err, msg);

      }

      if (err > 0) printf("\n%s", msg);

      return err;

MSXsetconstant
--------------

**Declaration:**

   .. code-block::

      int MSXsetconstant(int index, double value);

**Description:**

   Assigns a new value to a specific reaction constant.

**Arguments:**

   index is the sequence number of the reaction constant (starting from
   1) as it appeared in the MSX input file;

   value is the new value to be assigned to the constant.

**Returns:**

   Returns an error code or 0 for no error.

**Example:**

   .. code-block:: c

      //Declare an index variable

      int i;

      //Open the EPANET & MSX toolkits

      ...

      //Get the index of the constant named K1

      MSXgetindex(MSX_CONSTANT, "K1", &i);

      //Set a new value of K1

      MSXsetconstant(i, 0.53);


MSXsetparameter
---------------

**Declaration:**

   .. code-block::

      int MSXsetparameter(int type, int index, int param, double value);

**Description:**

   Assigns a value to a particular reaction parameter for a given pipe
   or tank within the pipe network.

**Arguments:**

   type is type of object being queried and must be either::

      MSX_NODE    (defined as 0) for a node

      MSX_LINK    (defined as 1) for a link

   index is the internal sequence number (starting from 1) assigned to
   the node or link;

   param is the sequence number of the parameter (starting from 1 as
   listed in the MSX input file);

   value is the value to be assigned to the parameter for the node or
   link of interest.

**Returns:**

   Returns an error code or 0 for no error.

**Notes:**

   Reaction parameters are only defined for storage tank nodes and pipe
   links. Attempts to set parameter values for other types of nodes and
   links will be ignored.

**Example:**

   .. code-block:: c

      //Declare some index variables

      int i, j;

      //Open the EPANET & MSX toolkits

      ...

      //Get indexes for parameter "K2" for pipe "P1"

      ENgetlinkindex("P1", &i);

      MSXgetindex(MSX_PARAMETER, "K2", &j);

      //Set a new value for the parameter

      MSXsetparameter(MSX_LINK, i, j, 0.25);


MSXsetinitqual
--------------

**Declaration:**

   .. code-block::

      int MSXsetinitqual(int type, int index, int species, double value);

**Description:**

   Assigns an initial concentration of a particular chemical species to
   a specific node or link of the pipe network.

**Arguments:**

   type is type of object being queried and must be either::

      MSX_NODE    (defined as 0) for a node

      MSX_LINK    (defined as 1) for a link

   index is the internal sequence number (starting from 1) assigned to
   the node or link;

   species is the sequence number of the species (starting from 1 as
   listed in the MSX input file);

   value is the initial concentration of the species to be applied at
   the node or link of interest.

**Returns:**

   Returns an error code or 0 for no error.

**Notes:**

   The EPANET toolkit functions ENgetnodeindex and ENgetlinkindex can be
   used to identify the index of a node or link from its ID name;

   Concentrations are expressed as mass units per liter for bulk species
   and as mass per unit area for surface species.

**Example:**

   .. code-block:: c

      //Declare some index variables

      int n, s;

      //Open the EPANET & MSX toolkits

      ...

      //Get the indexes of node "Tank_A" and species "CL2"

      ENgetnodeindex("Tank_A", &n);

      MSXgetindex(MSX_SPECIES, "CL2", &s);

      //Then set the initial concentration

      MSXsetinitqual(MSX_NODE, n, s, 1.25);


MSXsetsource
------------

**Declaration:**

   .. code-block::

      int MSXsetsource(int node, int species, int type, double level, int pat);

**Description:**

   Sets the attributes of an external source of a particular chemical
   species to a specific node of the pipe network.

**Arguments:**

   node is the internal sequence number (starting from 1) assigned to
   the node of interest;

   species is the sequence number of the species of interest (starting
   from 1 as listed in the MSX input file);

   type is the type of external source to be utilized and will be one of
   the following pre-defined constants::

      MSX_NOSOURCE    (defined as -1) for no source

      MSX_CONCEN      (defined as 0)  for a concentration source

      MSX_MASS        (defined as 1)  for a mass booster source

      MSX_SETPOINT    (defined as 2)  for a setpoint source

      MSX_FLOWPACED   (defined as 3)  for a flow paced source

   The meaning of these source types can be found in the description of
   the :ref:`sources` section of the MSX input file in :ref:`inputformat` of this manual.

   level is the baseline concentration (or mass flow rate) of the
   source;

   pat is the index of the time pattern used to add variability to the
   source's baseline level (use 0 if the source has a constant
   strength).

**Returns:**

   Returns an error code or 0 for no error.

**Notes:**

   The EPANET toolkit function ENgetnodeindex can be used to identify
   the index of a node from its ID name;

   Concentrations are expressed as mass units per liter for bulk species
   and as mass per unit area for surface species.

**Example:**

   .. code-block:: c

      //Declare some index variables

      int n, s;

      //Open the EPANET & MSX toolkits

      ...

      //Get indexes for species CL2 and node N1

      ENgetnodeindex("N1", &n);

      MSXgetindex(MSX_SPECIES, "CL2", &s);

      //Assign a constant source strength of 1 mg/L

      MSXsetsource(n, s, MSX_SETPOINT, 1.0, 0);


MSXsetpattern
-------------

**Declaration:**

   .. code-block::

      int MSXsetpattern(int pat, double mult[], int len);

**Description:**

   Assigns a new set of multipliers to a given MSX source time pattern.

**Arguments:**

   pat is the internal sequence number (starting from 1) of the pattern
   as it appears in the MSX input file;

   mult[] is an array of multiplier values to replace those previously
   used by the pattern;

   len is the number of entries int the multiplier array mult.

**Returns:**

   Returns an error code or 0 for no error.

**Notes:**

   This function only applies to source time patterns that appear in the
   MSX input file. There is a comparable EPANET toolkit function,
   ENsetpattern, which can be used for the demand patterns defined in
   the EPANET input file.

**Example:**

   .. code-block:: c

      //Declare an array of multipliers

      double mult[6] = {1.1, 1.5, 0.8, 0.5, 0.2, 0.0};

      int i;

      //Open the EPANET & MSX toolkits

      ...

      //Get index for pattern "P1"

      MSXgetindex(MSX_PATTERN, "P1", &i);

      //Assign multipliers to the pattern

      MSXsetpattern(i, mult, 6);


MSXsetpatternvalue
------------------

**Declaration:**

   .. code-block::

      int MSXsetpatternvalue(int pat, int period, double value);

**Description:**

   Assigns a new value to the multiplier for a specific time period in a
   given MSX source time pattern.

**Arguments:**

   pat is the internal sequence number (starting from 1) of the pattern
   as it appears in the MSX input file;

   period is the time period (starting from 1) in the pattern to be
   replaced;

   value is the new multiplier value to use for that time period.

**Returns:**

   Returns an error code or 0 for no error.

**Notes:**

   This function only applies to source time patterns that appear in the
   MSX input file. There is a comparable EPANET toolkit function,
   ENsetpatternvalue, which can be used for the demand patterns defined
   in the EPANET input file.

**Example:**

   .. code-block::c

      //Declare some variables

      int i, p, n;

      double v;

      //Open the EPANET & MSX toolkits

      ...

      //Get index & number of multipliers for pattern "P1"

      MSXgetindex(MSX_PATTERN, "P1", &p);

      MSXgetpatternlen(p, &n);

      //Increase each multiplier by factor of 2

      for (i = 1; i <= n; i++) {

         MSXgetpatternvalue(p, &v);

         v = 2.0 \* v;

         MSXsetpatternvalue(p, i, v);

      }  

MSXaddpattern
-------------

**Declaration:**

   .. code-block::

      int MSXaddpattern(char \* id);

**Description:**

   Adds a new, empty MSX source time pattern to an MSX project

**Arguments:**

   id is a C-style character string containing the name of the new
   pattern.

**Returns:**

   Returns an error code or 0 for no error.

**Notes:**

   The new pattern has no time periods or multipliers assigned to it.
   The MSXsetpattern function can be used to assign an array of
   multipliers to the pattern.

**Example:**

   .. code-block:: c

      //Declare some variables

      int err, p;

      double mult[6] = {0.5, 0.8, 1.2, 1.0, 0.7, 0.3};

      //Create a new pattern named "newPat"

      err = MSXaddpattern("newPat");

      //Assign multipliers to it

      if (err == 0) {

         MSXgetindex(MSX_PATTERN, "newPat", &p);

         MSXsetpattern(p, mult, 6);

      }