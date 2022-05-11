
'EPANETMSX.BAS
'
'Declarations of functions in the EPANET-MSX TOOLKIT
'(EPANETMSX.DLL)

'Last updated on 4/3/07

' These are codes used by the DLL functions
Global Const MSX_NODE      = 0
Global Const MSX_LINK      = 1
Global Const MSX_TANK      = 2
Global Const MSX_SPECIES   = 3
Global Const MSX_TERM      = 4
Global Const MSX_PARAMETER = 5
Global Const MSX_CONSTANT  = 6
Global Const MSX_PATTERN   = 7

Global Const MSX_BULK      = 0
Global Const MSX_WALL      = 1

Global Const MSX_NOSOURCE  = -1
Global Const MSX_CONCEN    = 0
Global Const MSX_MASS      = 1
Global Const MSX_SETPOINT  = 2
Global Const MSX_FLOWPACED = 3

'These are the external functions that comprise the DLL

Declare Function MSXopen Lib "epanetmsx.dll" (ByVal fname As String) As Long
Declare Function MSXsolveH Lib "epanetmsx.dll" () As Long 
Declare Function MSXusehydfile Lib "epanetmsx.dll" (ByVal fname As String) As Long
Declare Function MSXsolveQ Lib "epanetmsx.dll" () As Long
Declare Function MSXinit Lib "epanetmsx.dll" (ByVal saveFlag As Long) As Long
Declare Function MSXstep Lib "epanetmsx.dll" (t As Long, tleft As Long) As Long
Declare Function MSXsaveoutfile Lib "epanetmsx.dll" (ByVal fname As String) As Long
Declare Function MSXsavemsxfile Lib "epanetmsx.dll" (ByVal fname As String) As Long
Declare Function MSXreport Lib "epanetmsx.dll" () As Long
Declare Function MSXclose Lib "epanetmsx.dll" () As Long

Declare Function MSXgetindex Lib "epanetmsx.dll" (ByVal objtype As Long, ByVal id As String, index As Long) As Long
Declare Function MSXgetIDlen Lib "epanetmsx.dll" (ByVal objtype As Long, ByVal index As Long, len As Long) As Long
Declare Function MSXgetID Lib "epanetmsx.dll" (ByVal objtype As Long, ByVal index As Long, id As String, ByVal len As Long) As Long
Declare Function MSXgetcount Lib "epanetmsx.dll" (ByVal objtype As Long, count As Long) As Long
Declare Function MSXgetspecies Lib "epanetmsx.dll" (ByVal index As Long, stype As Long, units As String, aTol As Double, rTol As Double) As Long
Declare Function MSXgetconstant Lib "epanetmsx.dll" (ByVal index As Long, value As Double) As Long
Declare Function MSXgetparameter Lib "epanetmsx.dll" (ByVal objtype As Long, ByVal index As Long, ByVal param As Long, value As Double) As Long
Declare Function MSXgetsource Lib "epanetmsx.dll" (ByVal node As Long, ByVal species As Long, srctype As Long, level As Double, pat As Long) As Long
Declare Function MSXgetpatternlen Lib "epanetmsx.dll" (ByVal pat As Long, len As Long) As Long
Declare Function MSXgetpatternvalue Lib "epanetmsx.dll" (ByVal pat As Long, ByVal period As Long, value As Double) As Long
Declare Function MSXgetinitqual Lib "epanetmsx.dll" (ByVal objtype As Long, ByVal index As Long, ByVal species As Long, value As Double) As Long
Declare Function MSXgetqual Lib "epanetmsx.dll" (ByVal objtype As Long, ByVal index As Long, ByVal species As Long, value As Double) As Long
Declare Function MSXgeterror Lib "epanetmsx.dll" (ByVal code As Long, msg As String, ByVal len As Integer) As Long

Declare Function MSXsetconstant Lib "epanetmsx.dll" (ByVal indexAs Long, ByVal value As Double) As Long
Declare Function MSXsetparameter Lib "epanetmsx.dll" (ByVal objtype As Long, ByVal index As Long, ByVal param As Long, ByVal value As Double) As Long
Declare Function MSXsetinitqual Lib "epanetmsx.dll" (ByVal objtype As Long, ByVal index As Long, ByVal species As Long, ByVal value As Double) As Long
Declare Function MSXsetsource Lib "epanetmsx.dll" (ByVal node As Long, ByVal species As Long, ByVal stype As Long, ByVal level As Double, ByVal pat As Long) As Long
Declare Function MSXsetpatternvalue Lib "epanetmsx.dll" (ByVal pat As Long ByVal period As Long, ByVal value As Double) As Long
Declare Function MSXsetpattern Lib "epanetmsx.dll" (ByVal pat As Long, mult As Any, ByVal len As Long) As Long
Declare Function MSXaddpattern Lib "epanetmsx.dll" (ByVal id As String) As Long
