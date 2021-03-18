/******************************************************************************
**  MODULE:        NEWTON.C
**  PROJECT:       EPANET-MSX
**  DESCRIPTION:   Newton-Raphson algorithm used to solve a set of nonlinear
**                 algebraic equations.
**  COPYRIGHT:     Copyright (C) 2007 Feng Shang, Lewis Rossman, and James Uber.
**                 All Rights Reserved. See license information in LICENSE.TXT.
**  AUTHORS:       L. Rossman, US EPA - NRMRL
**                 F. Shang, University of Cincinnati
**                 J. Uber, University of Cincinnati
**  VERSION:       1.1.00
**  LAST UPDATE:   3/1/07
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "msxutils.h"
#include "newton.h"

// Local declarations
//-------------------
static int      Nmax;          // max. number of equations
static int      *Indx;         // permutation vector of row indexes     
static double   *F;            // function & adjustment vector
static double   *W;            // work vector
static double   **J;           // Jacobian matrix

//=============================================================================

int newton_open(int n)
/*
**  Purpose:
**    opens the algebraic solver to handle a system of n equations.
**            
**  Input:
**    n = number of equations 
**
**  Returns:
**    1 if successful, 0 if not.
**
**  Note:
**    All arrays are 1-based so an extra memory location
**    must be allocated for the unused 0-th position.
*/
{
    Nmax    = 0;
    Indx    = NULL;
    F       = NULL;
    W       = NULL;
	Indx	= (int *) calloc(n+1, sizeof(int));
    F   	= (double *) calloc(n+1, sizeof(double));
    W		= (double *) calloc(n+1, sizeof(double));
    J       = createMatrix(n+1, n+1);	
    if ( !Indx || !F || !W || !J ) return 0;
    Nmax = n;
    return 1;
}

//=============================================================================

void newton_close ()
/*
**  Purpose:
**    closes the algebraic solver.
**
**  Input:
**    none 
*/
{
	if (Indx) { free(Indx); Indx = NULL; }
	if (F)    { free(F); F = NULL; }
	if (W)    { free(W); W = NULL; }
	freeMatrix(J);
	J = NULL;
}

//=============================================================================

int newton_solve(double x[], int n, int maxit, int numsig, 
                 void (*func)(double, double*, int, double*))
/*
**  Purpose:
**    uses newton-raphson iterations to solve n nonlinear eqns.
**
**  Input:
**    x[] = solution vector
**    n = number of equations
**    maxit = max. number of iterations allowed
**    numsig = number of significant digits in error
**    func = pointer to the function that returns the function values at x.
**
**  Returns:
**    number of iterations if successful, -1 if Jacobian is singular,
**    -2 if it didn't converge, or -3 if n exceeds allowable size.
**
**  Note:
**    the arguments to the function func are:
**      t = a time value (not used here)
**      x = vector of unknowns being solved for
**      n = number of unknowns
**      f = vector of function values evaluated at x.
*/
{
    int i, k;
	double errx, errmax, cscal, relconvg = pow(10.0, -numsig);

    // --- check that system was sized adequetely

    if ( n > Nmax ) return -3;

    // --- use up to maxit iterations to find a solution

	for (k=1; k<=maxit; k++) 
	{
        // --- evaluate the Jacobian matrix

        jacobian(x, n, F, W, J, func);

        // --- factorize the Jacobian

        if ( !factorize(J, n, W, Indx) ) return -1;

        // --- solve for the updates to x (returned in F)

		for (i=1; i<=n; i++) F[i] = -F[i]; 
        solve(J, n, Indx, F);
		
		// --- update solution x & check for convergence

        errmax = 0.0;
        for (i=1; i<=n; i++)
        {
			cscal = x[i];
            if (cscal < relconvg) cscal = relconvg;
			x[i] += F[i];
            errx = fabs(F[i]/cscal);
            if (errx > errmax) errmax = errx;
        }
		if (errmax <= relconvg) return k;
	}

    // --- return error code if no convergence

	return -2;
}
