/******************************************************************************
**  MODULE:        MSXERR.C
**  PROJECT:       EPANET-MSX
**  DESCRIPTION:   Math error reporting routines.
**  COPYRIGHT:     Copyright (C) 2007 Feng Shang, Lewis Rossman, and James Uber.
**                 All Rights Reserved. See license information in LICENSE.TXT.
**  AUTHORS:       L. Rossman, US EPA - NRMRL
**                 F. Shang, University of Cincinnati
**                 J. Uber, University of Cincinnati
**  VERSION:       1.1.00
**  LAST UPDATE:   2/8/11
******************************************************************************/
#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <string.h>

#include "msxtypes.h"
#include "epanet2.h"

//  External variables
//--------------------
extern MSXproject  MSX;                // MSX project data

//  Local variables
//-----------------
static int   mathError;                // math error flag
static char  mathErrorMsg[1024];       // math error message
static char* elementTxt[] =            // see ObjectType in msxtypes.h
    {"", "pipe", "tank"};
static char* exprTypeTxt[] =           // see ExpressionType in msxtypes.h
    {"", "rate", "formula", "equilibrium"};

//  Exported functions
//--------------------
void   MSXerr_clearMathError(void);
int    MSXerr_mathError(void);
double MSXerr_validate(double x, int index, int element, int exprType);
void   MSXerr_writeMathErrorMsg(void);


//=============================================================================

void MSXerr_clearMathError()
/*
**  Purpose:
**    clears the math error flag.
*/
{
	mathError = 0;
	strcpy(mathErrorMsg, "");
}

//=============================================================================

int  MSXerr_mathError()
/*
**  Purpose:
**    returns the current state of the math error flag.
*/
{
    return mathError;
}

//=============================================================================

void MSXerr_writeMathErrorMsg()
/*
**  Purpose:
**    writes math error message to EPANET report file.
*/
{
	ENwriteline(mathErrorMsg);
	ENwriteline("");
}

//=============================================================================

double  MSXerr_validate(double x, int index, int element, int exprType)
/*
**  Purpose:
**    checks if a number is valid or not.
**
**  Input:
**    x = the number to check
**    index = array index of species or term that x was computed for
**    element = LINK for a pipe element or TANK for a tank element
**    exprType = type of expression that produced x
**
**  Returns:
**    the value of x if it's a valid number or 0 otherwise.
*/
{
	// return x if it's a valid number
	if (x == x) return x;

	// return 0 if the math error flag has previously been set
	// (we only want the first math error identified since others
	//  may have propagated from it)
	if (mathError) return 0.0;

	// construct a math error message
	if ( exprType == TERM )
	{
		sprintf(mathErrorMsg,
		"Ilegal math operation occurred for term:\n  %s",
		MSX.Term[index].id);
	}
	else
	{
		sprintf(mathErrorMsg,
		"Ilegal math operation occurred in %s %s expression for specie:\n  %s",
		elementTxt[element], exprTypeTxt[exprType], MSX.Species[index].id);
	}

	// set the math error flag and return 0
	mathError = 1;
	return 0.0;
}
