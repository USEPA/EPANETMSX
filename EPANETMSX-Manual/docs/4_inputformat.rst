
.. raw:: latex

    \clearpage

.. highlight:: none

.. _inputformat:


INPUT FILE FORMAT
==================

The input file used by EPANET-MSX to describe the species and reaction
system being modeled is organized into sections, where each section
begins with a keyword enclosed in brackets. The various section keywords
are listed in :numref:`table-input`. :numref:`ex_template_inp` contains a template of what the
input file layout looks like.

.. tabularcolumns:: |p{4cm}|p{11cm}|

.. _table-input:
.. table:: EPANET-MSX input file section keywords	

 +---------------------+----------------------------------------------------+
 | **[TITLE]**         | adds a descriptive title to the data set           |
 +---------------------+----------------------------------------------------+
 | **[OPTIONS]**       | sets the values of computational options           |
 +---------------------+----------------------------------------------------+
 | **[SPECIES]**       | names the chemical species being analyzed          |
 +---------------------+----------------------------------------------------+
 | **[COEFFICIENTS]**  | names the parameters and constants used in         |
 |                     | chemical rate and equilibrium expressions          |
 +---------------------+----------------------------------------------------+
 | **[TERMS]**         | defines intermediate terms used in chemical rate   |
 |                     | and equilibrium expressions                        |
 +---------------------+----------------------------------------------------+
 | **[PIPES]**         | supplies the rate and equilibrium expressions that |
 |                     | govern species dynamics in pipes                   |
 +---------------------+----------------------------------------------------+
 | **[TANKS]**         | supplies the rate and equilibrium expressions that |
 |                     | govern species dynamics in storage tanks           |
 +---------------------+----------------------------------------------------+
 | **[SOURCES]**       | identifies input sources (i.e., boundary           |
 |                     | conditions) for selected species                   |
 +---------------------+----------------------------------------------------+
 | **[QUALITY]**       | supplies initial conditions for selected species   |
 |                     | throughout the network                             |
 +---------------------+----------------------------------------------------+
 | **[PARAMETERS]**    | allows parameter values to be assigned on a pipe   |
 |                     | by pipe basis                                      |
 +---------------------+----------------------------------------------------+
 | **[DISPERSION]**    | specifies the relative molecular diffusivity of    |
 |                     | bulk species                                       |
 +---------------------+----------------------------------------------------+
 | **[PATTERNS]**      | defines time patterns used with input sources      |
 +---------------------+----------------------------------------------------+
 | **[REPORT]**        | specifies reporting options                        |
 +---------------------+----------------------------------------------------+

Each section can contain any number of lines of data and appear in any
order. Blank lines can appear anywhere in the file and the semicolon (;)
can be used to indicate what follows on the line is a comment, not data.
A maximum of 1024 characters can appear on a line. The ID labels used to
identify objects can be any combination of characters and numbers that
do not contain square brackets ([]), double quotes or a semicolon.

On the pages that follow the contents and formats of each input file
section are described in the order shown above. Reserved keywords are
shown in bold and option choices are separated by slashes.

.. literalinclude:: template.inp
    :language: none
    :name: ex_template_inp
    :caption: EPANET-MSX input file template
..

.. _title:

**[TITLE]**
-----------

   **Purpose:**

       Attaches a descriptive title to the problem being analyzed.

   **Format:**

      A single line of text.

   **Remarks:**

      The **[TITLE]** section is optional.

.. _options:

**[OPTIONS]**
--------------

   **Purpose:**
   
      Defines various simulation options.

   **Formats:**

      .. only:: html

         **AREA_UNITS** :math:`\ \;\;` **FT2/M2/CM2**

         **TIME_UNITS** :math:`\ \ \ ` **SEC/MIN/HR/DAY**
      
         **SOLVER** :math:`\ \ \ \ \ \ \ \ \ \ \;` **EUL/RK5/ROS2**
      
         **COUPLING** :math:`\ \ \ \ \;\;` **FULL/NONE**
      
         **TIMESTEP** :math:`\ \ \ \ \ \ \;` *seconds*
      
         **ATOL** :math:`\ \ \ \ \ \ \ \ \ \ \ \ \ \;` *value*
      
         **RTOL** :math:`\ \ \ \ \ \ \ \ \ \ \ \ \ \;` *value*
      
         **COMPILER** :math:`\ \ \ \ \ \;` **NONE/VC/GC**
      
         **SEGMENTS** :math:`\ \ \ \ \ \ \;` *value*
      
         **PECLET** :math:`\ \ \ \ \ \ \ \ \ \ \;` *value* 

      .. raw:: latex  
  
         \begin{tabular}{l l}

            \textbf{AREA\_UNITS} & \textbf{FT2/M2/CM2} \\
            \textbf{Time\_UNITS} & \textbf{SEC/MIN/HR/DAY} \\
            \textbf{SOLVER} & \textbf{EUL/RK5/ROS2} \\
            \textbf{COUPLING} & \textbf{FULL/NONE} \\
            \textbf{TIMESTEP} & \textit{seconds} \\
            \textbf{ATOL} & \textit{value} \\
            \textbf{TROL} & \textit{value} \\
            \textbf{COMPILER} & \textbf{NONE/VC/GC} \\
            \textbf{SEGMENTS} & \textit{value} \\
            \textbf{PECLET} & \textit{value}
 
         \end{tabular}
  

   **Definitions:**

      **AREA_UNITS** sets the units used to express pipe wall surface area where:

         **FT2** = square feet

         **M2** :math:`\ ` = square meters

         **CM2** = square centimeters

         The default is **FT2**.

      **TIME_UNITS** is the units in which all reaction rate terms are expressed. The default units are hours (**HR**).

      **SOLVER** is the choice of numerical integration method used to solve the reaction system where:

         **EUL** :math:`\ ` = standard Euler integrator

         **RK5** :math:`\ ` = Runge-Kutta 5\ :sup:`th` order integrator

         **ROS2** = 2\ :sup:`nd` order Rosenbrock integrator.

         The default solver is **EUL**.

      **COUPLING** determines to what degree the solution of any algebraic equilibrium equations is coupled to the integration of the reaction rate equations. If coupling is **NONE** then the solution to the algebraic equations is only updated at the end of each integration time step. With **FULL** coupling the updating is done whenever a new set of values for the rate-dependent variables in the reaction rate expressions is computed. This can occur at several intermediate times during the normal integration time step when using the **RK5** and **ROS2** integration methods. Thus the **FULL** coupling option is more accurate, but can require significantly more computation time. The default is **NONE**.

      **TIMESTEP** is the time step, in seconds, used to integrate the reaction system. The default time step is 300 seconds (5 minutes).

      **ATOL** is the default absolute tolerance used to determine when two concentration levels of a species are the same. It applies to all species included in the model. Different values for individual species can be set in the :ref:`species` section of the input (see below). If no **ATOL** option is specified then it defaults to 0.01 (regardless of species concentration units).

      **RTOL** is the default relative accuracy level on a species' concentration used to adjust time steps in the **RK5** and **ROS2** integration methods. It applies to all species included in the model. Different values for individual species can be set in the :ref:`species` section of the input (see below). If no **RTOL** option is specified then it defaults to 0.001.

      **COMPILER** determines if the chemical reaction system being modeled should first be compiled before the simulation begins. 
      This option is available on Windows systems that have either the Microsoft Visual C++ or the MinGW compiler installed or on 
      Linux systems with the Gnu C++ compiler. The VC option is used for the Visual C++ compiler, the GC option is for the MinGW or 
      Gnu compilers, while NONE is the default which means that no compilation is performed. Using this option can result in faster run times by a factor of 2 to 5.

      **SEGMENTS** is the maximum number of water quality segments any pipe can have. The default setting is 5000. Reducing this number can
      speed up the simulation time but may lead to more numerical diffusion. 

      **PECLET** is the Peclet number above which the dispersion effect is neglected. The default value is 1000.0. Reducing this number excludes more pipes from dispersion modeling.

.. _species:

**[SPECIES]**
-------------

   **Purpose:**

      Defines each chemical species being simulated.

   **Formats:**

      .. only:: html

         **BULK** :math:`\ ` *name* :math:`\ \ ` *units* :math:`\ \ ` (*Atol*) :math:`\ \ ` (*Rtol*) 
      
         **WALL** :math:`\ ` *name* :math:`\ \ ` *units* :math:`\ \ ` (*Atol*) :math:`\ \ ` (*Rtol*)

      .. raw:: latex  

         \begin{tabular}{l l l l l}
            \textbf{BULK} & \textit{name} & \textit{units} &  (\textit{Atol}) & (\textit{Rtol}) \\
            \textbf{WALL} & \textit{name} & \textit{units} &  (\textit{Atol}) & (\textit{Rtol})
 
         \end{tabular}
  
   **Definitions:**

      .. only:: html

         :*name*: species name

         :*units*: species mass units

         :*Atol*: optional absolute tolerance that overrides the global value set in the :ref:`options` section

         :*Rtol*: optional relative tolerance that overrides the global value set in the :ref:`options` section

      .. raw:: latex

         \begin{tabular} {l l }

            \textit{name}: & species name \\
            \textit{units}: & species mass units \\
            \textit{Atol}: & optional absolute tolerance that overrides the global value set in the \textbf[OPTIONS] section\\
            \textit{Rtol}: & optional relative tolerance that overrides the global value set in the \textbf[OPTIONS] section\\
         \end{tabular}

   **Remarks:**

    -  The first format is used to define a bulk water (i.e., dissolved) species while the second is used for species attached (i.e., adsorbed) to the pipe wall.

    -  Bulk species are measured in concentration units of mass units per liter while wall species are measured in mass units per unit area.

    -  Any units can be used to represent species mass. The user is responsible for including any necessary unit conversion factors when specifying chemical reaction and equilibrium expressions that involve several species with different mass units.

    -  Values for both *Atol* and *Rtol* must be provided to override the default tolerances.

   **Examples:**

   ::

      [SPECIES]

      ;Bulk chlorine in mg/L with default tolerances

      BULK CL2 MG

      ;Bulk biomass in ug/L with specific tolerances

      BULK Xb UG 0.0001 0.01

      ;Attached biomass in ug/area with specific tolerances

      WALL Xa UG 0.0001 0.01

.. _coefficients:

**[COEFFICIENTS]**
------------------

   **Purpose:**

      Defines parameters and constants that are used in the reaction/equilibrium chemistry model.

   **Formats:**
    
      .. only:: html

         **PARAMETER** :math:`\quad` *name* :math:`\quad` *value*

         **CONSTANT** :math:`\quad` *name* :math:`\quad` *value*

      .. raw:: latex  
  
         \begin{tabular}{l l l}
            \textbf{PARAMETER} & \textit{name} & \textit{value}\\
            \textbf{CCONSTANT} & \textit{name} & \textit{value}\\
 
         \end{tabular}
      

   **Definitions:**

      .. only:: html

         :*name*: coefficient's identifying name

         :*value*: global value of the coefficient.
      
      .. raw:: latex

         \begin{tabular} {l l}

            \textit{name}:  & coefficient's identifying name \\
            \textit{value}: & global value of the coefficient. \\
         
         \end{tabular}

   **Remarks:**

      A **PARAMETER** is a coefficient whose value can be changed on a pipe by pipe (or tank by tank) basis (see the :ref:`parameters` section below) 
      while a **CONSTANT** coefficient maintains the same value throughout the pipe network.

   **Examples:**

   ::

      [COEFFICIENTS]

      ;Kb can vary by pipe

      PARAMETER Kb 0.1

      ;Kw is fixed for all pipes

      CONSTANT Kw 1.5

.. _terms:

**[TERMS]**
------------

   **Purpose:**

      Defines mathematical expressions that are used as intermediate terms in the expressions for the chemical reaction/equilibrium model.

   **Formats:**

      .. only:: html

         *termID* :math:`\quad` *expression*
   
      .. raw:: latex

         \begin{tabular}{l l}

            \textit{termID} & \textit{expression} 

         \end{tabular}
   
   
   **Definitions:**

      .. only:: html

         :*termID*: identifying name given to the term
         :*expression*: any well-formed mathematical expression involving species, parameters, constants, hydraulic variables or other terms.
      
      .. raw:: latex

         \begin{tabular} {l l }
            \textit{termID}: & identifying name given to the term \\
            \textit{expression}: & any well-formed mathematical expression involving species, parameters, constants, \\
             & hydraulic variables or other terms.
         \end{tabular} 


   **Remarks:**

      Terms can be used to simplify reaction rate or equilibrium expressions that would otherwise be unwieldy to write all on one line or have the same terms repeated in several different rate/equilibrium equations. The definition and use of TERMS, when those terms are common and appear in multiple rate or equilibrium expressions, may speed computation because the common term expression requires only one evaluation.

      Hydraulic variables consist of the following reserved names:

      **D** :math:`\ \ \ ` pipe diameter (feet or meters)

      **Kc** :math:`\,` pipe roughness coefficient (unitless for Hazen-Williams or Chezy-Manning head loss formulas, millifeet or millimeters for Darcy-Weisbach head loss formula)

      **Q** :math:`\ \ \ ` pipe flow rate (flow units)

      **U** :math:`\ \ \ ` pipe flow velocity (ft/sec or m/sec)

      **Re** :math:`\ \,` flow Reynolds number

      **Us** :math:`\ \,` pipe shear velocity (ft/sec or m/sec)

      **Ff** :math:`\ \ ` Darcy-Weisbach friction factor

      **Av** :math:`\ \,` Surface area per unit volume (area units/L)

   **Examples:**

   ::

      [TERMS]

      ;A mass transfer coefficient

      Kf 1.2e-4*Re^0.88/D

      ;A reaction term

      a1 k1*HOCL*NH3

.. _pipes:

**[PIPES]**
---------------

   **Purpose:**

      Supplies the rate and equilibrium expressions that govern species dynamics in pipes.

   **Formats:**

      .. only:: html

         **EQUIL** :math:`\ \ \ \ \ \ \ \ ` *specieID* :math:`\ \ ` *expression*

         **RATE** :math:`\ \ \ \ \ \ \ \ \ ` *specieID* :math:`\ \ ` *expression*

         **FORMULA** :math:`\ ` *specieID* :math:`\ \ ` *expression*

      .. raw:: latex

         \begin{tabular}{l l l}

            \textbf{EQUIL} & \textit{specieID} & \textit{expression} \\
            \textbf{RATE}  & \textit{specieID} & \textit{expression} \\
            \textbf{FORMULA}  & \textit{specieID} & \textit{expression} 

         \end{tabular}

   **Definitions:**

      .. only:: html
         
         :*specieID*: a species identifier

         :*expression*: any well-formed mathematical expression involving species, parameters, constants, hydraulic variables or terms.

      .. raw:: latex

         \begin{tabular} {l l }

            \textit{specieID}: & a species identifier \\
            \textit{expression}: & any well-formed mathematical expression involving species, parameters, constants, \\
             & hydraulic variables or terms.

         \end{tabular}


   **Remarks:**

      -  There should be one expression supplied for each species defined in the model.

      -  The allowable hydraulic variables were defined above in the description of the :ref:`terms` section.

      -  The **EQUIL** format is used for equilibrium expressions where it is assumed that the expression supplied is being equated to zero. Thus formally there is no need to supply the name of a species, but requiring one encourages the user to make sure that all species are accounted for.

      -  The **RATE** format is used to supply the equation that expresses the rate of change of the given species with respect to time as a function of the other species in the model.

      -  The **FORMULA** format is used when the concentration of the named species is a simple function of the remaining species.

   **Examples:**

   ::

      [PIPES]

      ;Bulk chlorine decay

      RATE CL2 -Kb*CL2

      ;Adsorption equilibrium between Cb in bulk and Cw on wall

      EQUIL Cw Cmax*k*Cb / (1 + k*Cb) - Cw

      ;Conversion between biomass (X) and cell numbers (N)

      FORMULA N log10(X*1.0e6)

      ;Bulk C formation plus non-equilibrium sorption between C and Cs

      ;Using hydraulic variable Av [Area-Units/Liter]

      RATE C K*C - Av*(K1*(Smax-Cs)*C - K2*Cs)

      ;Equivalent sorption model, using 1/hydraulic radius = 4/D

      ;Assumes area units are FT2 and diameter in FT

      ;CFPL is TERM equal to FT3/Liter, thus (4*CFPL/D) == Av

      RATE C K*C - (4*CFPL/D)*(K1*(Smax-Cs)*C - K2*Cs)

.. _tanks:

**[TANKS]**
------------

   **Purpose:**

      Supplies the rate and equilibrium expressions that govern species dynamics in storage tanks.

   **Formats:**

      .. only:: html

         **EQUIL** :math:`\ \ \ \ \ \ \ \ ` *specieID* :math:`\ \ ` *expression*

         **RATE** :math:`\ \ \ \ \ \ \ \ \ ` *specieID* :math:`\ \ ` *expression*

         **FORMULA** :math:`\ ` *specieID* :math:`\ \ ` *expression*

      .. raw:: latex

         \begin{tabular}{l l l}

            \textbf{EQUIL} & \textit{specieID} & \textit{expression} \\
            \textbf{RATE}  & \textit{specieID} & \textit{expression} \\
            \textbf{FORMULA}  & \textit{specieID} & \textit{expression} 

         \end{tabular}

   **Definitions:**

      .. only:: html

         :*specieID*: a species identifier

         :*expression*: any well-formed mathematical expression involving species, parameters, constants, or terms.

      .. raw:: latex

         \begin{tabular} {l l }

            \textit{specieID}: & a species identifier \\
            \textit{expression}: & any well-formed mathematical expression involving species, parameters, constants, or terms.

         \end{tabular}      

   **Remarks:**

      -  A **[TANKS]** section is always required when a model contains both bulk and wall species, even when there are no tanks in the pipe network. If the model contains only bulk species, then this section can be omitted if the reaction expressions within tanks are the same as within pipes.

      -  There should be one expression supplied for each bulk species defined in the model. By definition, wall species do not exist within tanks.

      -  Hydraulic variables are associated only with pipes and cannot appear in tank expressions.

      -  The **EQUIL** format is used for equilibrium expressions where it is assumed that the expression supplied is being equated to zero. Thus formally there is no need to supply the name of a species but doing so allows one to make sure that all species are accounted for.

      -  The **RATE** format is used to supply the equation that expresses the rate of change of the given species with respect to time as a function of the other species in the model.

      -  The **FORMULA** format is used when the concentration of the named species is a simple function of the remaining species.

   **Examples:**

      See the examples listed for the :ref:`pipes` section.

.. _sources: 

**[SOURCES]**
--------------

   **Purpose:**

      Defines the locations where external sources of particular species enter the pipe network.

   **Formats:**

      .. Only:: html
         
         *sourceType* :math:`\quad` *nodeID* :math:`\quad` *specieID* :math:`\quad` *strength* :math:`\quad` *(patternID)*

      .. raw:: latex

         \begin{tabular} {l l l l l}

            \textit{sourceType} & \textit{nodeID} & \textit{specieID} & \textit{strength} & (\textit{patternID})

         \end{tabular}

   **Definitions:**

      .. ONLY:: html

         :*sourceType*: either **MASS**, **CONCEN**, **FLOWPACED**, or **SETPOINT**

         :*nodeID*: the ID label of the network node where the source is located

         :*specieID*: a bulk species identifier

         :*strength*: the baseline mass inflow rate (mass/minute) for **MASS** sources or concentration (mass/L) for all other source types

         :*patternID*: the name of an optional time pattern that is used to vary the source strength over time.

      .. raw:: latex

         \begin{tabular}{l l}
            
            \textit{sourceType}: & either \textbf{MASS}, \textbf{CONCEN}, \textbf{FLOWPACED}, or \textbf{SETPOINT} \\
            \textit{nodeID}: & the ID label of the network node where the source is located \\
            \textit{specieID}: & a bulk species identifier \\
            \textit{strength}: & the baseline mass inflow rarte (mass/minute) for \textbf{MASS} source or concentration (mass/L)\\
              & for all other source types \\
            \textit{patternID}: & the name of an optional time pattern that is used to vary the source strength over time.

         \end{tabular}


   **Remarks:**

      -  Use one line for each species that has non-zero source strength.

      -  Only bulk species can enter the pipe network, not wall species.

      -  The definitions of the different source types conform to those used in the original EPANET program are as follows:

         -  A **MASS** type source adds a specific mass of species per unit of time to the total flow entering the source node from all connecting pipes.

         -  A **CONCEN** type source sets the concentration of the species in any external source inflow (i.e., a negative demand) entering the node. The external inflow must be established as part of the hydraulic specification of the network model.

         -  A **FLOWPACED** type source adds a specific concentration to the concentration that results when all inflows to the source node from its connecting pipes are mixed together.

         -  A **SETPOINT** type source fixes the concentration leaving the source node to a specific level as long as the mixture concentration of flows from all connecting pipes entering the node is less than the set point concentration.

      -  If a time pattern is supplied for the source, it must be one defined in the :ref:`patterns` section of the MSX file, not a pattern from the associated EPANET input file.

   **Examples:**

   ::

      [SOURCES]

      ;Inject 6.5 mg/minute of chemical X into Node N1 over the period of time 
      
      ;defined by pattern PAT1

      MASS N1 X 6.5 PAT1

      ;Maintain a 1.0 mg/L level of chlorine at node N100

      SETPOINT N100 CL2 1.0

.. _quality:

**[QUALITY]**
---------------

   **Purpose:**

      Specifies the initial concentrations of species throughout the pipe network.

   **Formats:**

      .. only:: html

         **GLOBAL** :math:`\ ` *specieID* :math:`\ ` *concen*

         **NODE** :math:`\ \ \ \ ` *nodeID* :math:`\ \ \ ` *specieID* :math:`\ ` *concen*

         **LINK** :math:`\ \ \ \ \ \ ` *linkID* :math:`\ \ \ \ \ ` *specieID* :math:`\ ` *concen*

      .. raw:: latex

         \begin{tabular}{l l l l}

            \textbf{GLOBAL} & \textit{specieID} & \textit{concen} & \\
            \textbf{NODE} & \textit{nodeID} & \textit{specieID} & \textit{concen} \\
            \textbf{LINK} & \textit{linkID} & \textit{specieID} & \textit{concen}

         \end{tabular}

   **Definitions:**

      .. only:: html

         :*specieID*: a species identifier

         :*nodeID*: a network node ID label

         :*linkID*: a network link ID label

         :*concen*: a species concentration

      .. raw:: latex

         \begin{tabular} {l l}

            \textit{specieID}: & a species identifier \\
            \textit{nodeID}: & a network node ID label \\
            \textit{linkID}: & a network link ID label \\
            \textit{concen}: & a species concentration

         \end{tabular}

   **Remarks:**

      -  Use as many lines as necessary to define a network's initial condition.

      -  Use the **GLOBAL** format to set the same initial concentration at all nodes (for bulk species) or within all pipes (for wall species).

      -  Use the **NODE** format to set an initial concentration of a bulk species at a particular node.

      -  Use the **LINK** format to set an initial concentration of a wall species within a particular pipe.

      -  The initial concentration of a bulk species within a pipe is assumed equal to the initial concentration at the downstream node of the pipe.

      -  All initial concentrations are assumed to be zero unless otherwise specified in this section.

      -  Models with equilibrium equations will require that reasonable initial conditions be set so that the equations are solvable. For example, if they contain a ratio of species concentrations then a divide by zero condition will occur if all initial concentrations are set to zero.

   **Examples:**

   ::

      [QUALITY]

      ;Set concentration of bulk species Cb to 1.0 at all nodes

      GLOBAL Cb 1.0

      ;Override above condition for node N100

      NODE N100 Cb 0.5

.. _parameters:

**[PARAMETERS]**
------------------

   **Purpose:**

      Defines values for specific reaction rate parameters on a pipe by pipe or tank by tank basis.

   **Formats:**

      .. only:: html

         **PIPE** :math:`\ \ \ ` *pipeID* :math:`\ ` *paramID* :math:`\ ` *value*

         **TANK** :math:`\ ` *tankID* :math:`\ ` *paramID* :math:`\ ` *value*
      
      .. raw:: latex

         \begin{tabular}{l l l l}

            \textbf{PIPE} & \textit{pipeID} & \textit{paramID} & \textit{value} \\
            \textbf{TANK} & \textit{tankID} & \textit{paramID} & \textit{value}

         \end{tabular}

   **Definitions:**

      .. only:: html

         :*pipeID*: the ID label of a pipe link in the network

         :*tankID*: the ID label of a tank node in the network

         :*paramID*: the name of one of the reaction rate parameters listed in the :ref:`coefficients` section

         :*value*: the parameter's value used for the specified pipe or tank.
      
      .. raw:: latex

         \begin{tabular} {l l }

            \textit{pipeID}: & the ID label of a pipe link in the network \\
            \textit{tankID}: & the ID label of a tank node in the network \\
            \textit{paramID}: & the name of one of the reaction rate parameters \\
            \textit{value}: & the parameter's value used for the specified pipe or tank.
         
         \end{tabular}

   **Remarks:**

      -  Use one line for each pipe or tank whose parameter value is different than the global value.

.. _patterns:

**[PATTERNS]**
---------------

   **Purpose:**

      Defines time patterns used to vary external source strength over time.

   **Formats:**

      *name* :math:`\quad` *multiplier* :math:`\ ` *multiplier* :math:`\quad` ...

   **Definitions:**

      .. only:: html
               
       :*name*: an identifier assigned to the time pattern

       :*multiplier*: a multiplier used to adjust a baseline value
      
      .. raw:: latex

       \begin{tabular} {l l}

        \textit{name}: & an identifier assigned to the time pattern \\
        \textit{multiplier}: & a multiplier used to adjust a baseline value

       \end{tabular}


   **Remarks:**

      -  Use one or more lines for each time pattern included in the model.

      -  If extending the list of multipliers to another line remember to begin the line with the pattern name.

      -  All patterns share the same time period interval as defined in the **[TIMES]** section of the EPANET input file being used in conjunction with the EPANET-MSX input file.

      -  Each pattern can have a different number of time periods.

      -  When the simulation time exceeds the pattern length the pattern wraps around to its first period.

   **Examples:**

   ::

      [PATTERNS]

      ;A 3-hour injection pattern over a 24 hour period 
      
      ;(assuming a 1-hour pattern time interval is in use)

      P1 0.0 0.0 0.0 0.0 1.0 1.0

      P1 1.0 0.0 0.0 0.0 0.0 0.0

      P1 0.0 0.0 0.0 0.0 0.0 0.0

      P1 0.0 0.0 0.0 0.0 0.0 0.0


.. _dispersion:

**[DISPERSION]**
------------------

   **Purpose:**

      Defines the relative diffusivity of the species to be incorporated in the dispesion modeling.

   **Formats:**
    
      .. only:: html

         *specieID* :math:`\quad` *value*

      .. raw:: latex  
  
         \begin{tabular}{l l }

            \textit{specieID} & \textit{value}\\
 
         \end{tabular}
      

   **Definitions:**

      .. only:: html

         :*specieID*: a species identifier

         :*value*: relative diffusivity of the species.
      
      .. raw:: latex

         \begin{tabular} {l l}

            \textit{specieID}:  & a species identifier \\
            \textit{value}: & relative diffusivity of the species. \\
         
         \end{tabular}

   **Remarks:**
   
      The relative diffusivity is the ratio of the molecular diffusivity of the species to that of chlorine at 20 deg. C (:math:`0.00112 ft^2/day`). 
      If the relative diffusivity of a species is not defined in this section, the dispersion of the species is neglected. 

   
   **Examples:**

   ::

      [DISPERISON]

      ; same molucular diffusivity as chlorine at 20 deg. C

      HOCL 1.0
      
      NH3 1.0
      
      NH2CL 1.0
      
      NHCL2 1.0


.. _report:

**[REPORT]**
--------------

   **Purpose:**
   
      Describes the contents of the output report produced from a simulation.

   **Formats:**

      .. only:: html
         
         **NODES** :math:`\ \ \ \ \ \ ` **ALL**

         **NODES** :math:`\ \ \ \ \ \ ` *node1 node2 ...*

         **LINKS** :math:`\ \ \ \ \ \ \ ` **ALL**

         **LINKS** :math:`\ \ \ \ \ \ \ ` *link1 link2 ...*

         **SPECIES** :math:`\ \ \ ` *speciesID* :math:`\ ` **YES**\ */*\ **NO** :math:`\ ` (*precision*)

         **FILE** :math:`\ \ \ \ \ \ \ \ \ \ ` *filename*

         **PAGESIZE** :math:`\ ` *lines*

      .. raw:: latex

         \begin{tabular} {l l l l}

            \textbf{NODES} & \textbf{ALL} & & \\
            \textbf{NODES} & \textit{node1 node2 ...} & & \\
            \textbf{LINKS} & \textbf{ALL} & & \\
            \textbf{LINKS} & \textit{link1 link2 ...} & & \\
            \textbf{SPECIES} & \textit{speciesID} & YES \slash No & (\textit{precision}) \\
            \textbf{FILE} & \textit{filename} & & \\
            \textbf{PAGESIZE} & \textit{lines} & &
         
         \end{tabular}


   **Definitions:**

      .. only:: html

         :*node1, node2, etc.*: a list of nodes whose results are to be reported

         :*link1, link2, etc.*: a list of links whose results are to be reported

         :*specieID*: the name of a species to be reported on

         :*precision*: number of decimal places used to report a species' concentration

         :*filename*: the name of a file to which the report will be written

         :*lines*: the number of lines per page to use in the report.

      .. raw:: latex

         \begin{tabular} {ll}

            \textit{node1, node2, etc.}: & a list of nodes whose results are to be reported \\
            \textit{link1, link2, etc.}: & a list of links whose results are to be reported \\
            \textit{specieID}: &  the name of a species to be reported on \\
            \textit{precision}: & number of decimal places used to report a species' concentration \\
            \textit{filename}: &  the name of a file to which the report will be written \\
            \textit{lines}: & the number of lines per page to use in the report.
         
         \end{tabular}


   **Remarks:**

      -  Use as many **NODES** and **LINKS** lines as it takes to specify which locations get reported. The default is not to report results for any nodes or links.

      -  Use the **SPECIES** line to specify which species get reported and at what precision. The default is to report all species at two decimal places of precision.

      -  The **FILE** line is used to have the report written to a specific file. If not provided the report will be written to the same file used for reporting program errors and simulation status.

   **Examples:**

   ::

      [REPORT]

      ;Write results for all species at all nodes and links 
      
      ;at all time periods to a specific file

      NODES ALL

      LINKS ALL

      FILE "c:\my files\epanet-msx\myreport.txt"

      [REPORT]

      ;Write nodal results for species S1 and S2 using

      ;4 decimal places to the standard EPANET report file

      SPECIES S1 YES 4

      SPECIES S2 YES 4

      NODES ALL
