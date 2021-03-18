unit epanetmsx;

{ Declarations of imported procedures from the EPANET-MSX TOOLKIT }
{ (EPANETMSX.DLL) }

{Last updated on 4/3/07}

interface

const
 MSX_NODE      = 0
 MSX_LINK      = 1
 MSX_TANK      = 2
 MSX_SPECIES   = 3
 MSX_TERM      = 4
 MSX_PARAMETER = 5
 MSX_CONSTANT  = 6
 MSX_PATTERN   = 7

 MSX_BULK      = 0
 MSX_WALL      = 1

 MSX_NOSOURCE  = -1
 MSX_CONCEN    = 0
 MSX_MASS      = 1
 MSX_SETPOINT  = 2
 MSX_FLOWPACED = 3

function MSXopen(fname: Pchar): Integer; stdcall;
function MSXsolveH: Integer; stdcall;
function MSXusehydfile(fname: PChar): Integer; stdcall;
function MSXsolveQ: Integer; stdcall;
function MSXinit(saveFlag: Integer): Integer; stdcall;
function MSXstep(var t: Longint; var tleft: Longint): Integer; stdcall;
function MSXsaveoutfile(fname: Pchar): Integer; stdcall;
function MSXsavemsxfile(fname: Pchar): Integer; stdcall;
function MSXreport: Integer; stdcall;
function MSXclose: Integer; stdcall;

function MSXgetindex(objtype: Integer; id: Pchar; var index: Integer):
         Integer; stdcall;
function MSXgetIDlen(objtype: Integer; index: Integer; var len: Integer):
         Integer; stdcall;
function MSXgetID(objtype: Integer; index: Integer; var id: Pchar; len: Integer):
         Integer; stdcall;
function MSXgetcount(objtype: Integer; var count: Integer): Integer; stdcall;
function MSXgetspecies(index: Integer; var stype: Integer; var units: Pchar;
         var aTol: Double; var rTol: Double): Integer; stdcall;
function MSXgetconstant(index: Integer; var value: Double): Integer; stdcall;
function MSXgetparameter(objtype: Integer; index: Integer; param: Integer;
         var value: Double): Integer; stdcall;
function MSXgetsource(node: Integer; species: Integer; var srctype: Integer;
         var level: Double; var pat: Integer): Integer; stdcall;
function MSXgetpatternlen(pat: Integer; var len: Integer): Integer; stdcall;
function MSXgetpatternvalue(pat: Integer; period: Integer; var value: Double):
         Integer; stdcall;
function MSXgetinitqual(objtype: Integer; index: Integer; species: Integer;
         var value: Double): Integer; stdcall;
function MSXgetqual(objtype: Integer; index: Integer; species: Integer;
         var value: Double): Integer; stdcall;
function MSXgeterror(code: Integer; var msg: Pchar; len: Integer):
         Integer; stdcall;

function MSXsetconstant(index: Integer; value: Double): Integer; stdcall;
function MSXsetparameter(objtype: Integer; index: Integer; param: Integer;
         value: Double): Integer; stdcall;
function MSXsetinitqual(objtype: Integer; index: Integer; species: Integer;
         value: Double): Integer; stdcall;
function MSXsetsource(node: Integer; species: Integer; stype: Integer;
         level: Double; pat: Integer): Integer; stdcall;
function MSXsetpatternvalue(pat: Integer; period: Integer; value: Double):
         Integer; stdcall;
function MSXsetpattern(pat: Integer; mult: array of Double; len: Integer):
         Integer; stdcall;
function MSXaddpattern(id: Pchar): Integer; stdcall;

implementation

function MSXopen;            external 'EPANETMSX.DLL';
function MSXsolveH;          external 'EPANETMSX.DLL';
function MSXusehydfile;      external 'EPANETMSX.DLL';
function MSXsolveQ;          external 'EPANETMSX.DLL';
function MSXinit;            external 'EPANETMSX.DLL';
function MSXstep;            external 'EPANETMSX.DLL';
function MSXsaveoutfile;     external 'EPANETMSX.DLL';
function MSXsavemsxfile;     external 'EPANETMSX.DLL';
function MSXreport;          external 'EPANETMSX.DLL';
function MSXclose;           external 'EPANETMSX.DLL';

function MSXgetindex;        external 'EPANETMSX.DLL';
function MSXgetIDlen;        external 'EPANETMSX.DLL';
function MSXgetID;           external 'EPANETMSX.DLL';
function MSXgetcount;        external 'EPANETMSX.DLL';
function MSXgetspecies;       external 'EPANETMSX.DLL';
function MSXgetconstant;     external 'EPANETMSX.DLL';
function MSXgetparameter;    external 'EPANETMSX.DLL';
function MSXgetsource;       external 'EPANETMSX.DLL';
function MSXgetpatternlen;   external 'EPANETMSX.DLL';
function MSXgetpatternvalue; external 'EPANETMSX.DLL';
function MSXgetinitqual;     external 'EPANETMSX.DLL';
function MSXgetqual;         external 'EPANETMSX.DLL';
function MSXgeterror;        external 'EPANETMSX.DLL';

function MSXsetconstant;     external 'EPANETMSX.DLL';
function MSXsetparameter;    external 'EPANETMSX.DLL';
function MSXsetinitqual;     external 'EPANETMSX.DLL';
function MSXsetsource;       external 'EPANETMSX.DLL';
function MSXsetpatternvalue; external 'EPANETMSX.DLL';
function MSXsetpattern;      external 'EPANETMSX.DLL';
function MSXaddpattern;      external 'EPANETMSX.DLL';

end.
