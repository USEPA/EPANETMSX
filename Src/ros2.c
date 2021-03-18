/*******************************************************************************
**  MODULE:        ROS2.C
**  PROJECT:       EPANET-MSX
**  DESCRIPTION:   a second order Rosenbrock 2(1) method for solving stiff sets of
**                 ordinary differential equations.
**  AUTHOR:        L. Rossman, US EPA - NRMRL
**  VERSION:       1.1.00
**  LAST UPDATE:   3/1/07
**
**  This code is based on material presented in:
**    Verwer, J.G., Spee, E.J., Blom, J.G. and Hundsdorfer, W.H.,
**    "A second order Rosenbrock method applied to photochemical dispersion
**    problems", SIAM J. Sci. Comput., 20:1456-1480, July 1999.
*******************************************************************************/

#include <stdlib.h>
#include <math.h>
#include "msxutils.h"
#include "ros2.h"

#define fmin(x,y) (((x)<=(y)) ? (x) : (y))     /* minimum of x and y    */
#define fmax(x,y) (((x)>=(y)) ? (x) : (y))     /* maximum of x and y    */

//  Local variables
//-----------------
static double** A;                     // Jacobian matrix
static double*  K1;                    // Intermediate solutions
static double*  K2;
static double*  Ynew;                  // Updated function values
static int*     Jindx;                 // Jacobian column indexes
static int      Nmax;                  // Max. number of equations
static int      Adjust;                // use adjustable step size


//=============================================================================

int ros2_open(int n, int adjust)
/*
**  Purpose:
**    Opens the ROS2 integrator.
**
**  Input:
**    n = number of equations to be solved
**    adjust = 1 if step size adjustment used, 0 if not
**
**  Returns:
**    1 if successful, 0 if not.
*/
{
    int n1 = n + 1;
    Nmax = 0;
    Adjust = adjust;
    K1    = NULL;
    K2    = NULL;
    Jindx = NULL;
    Ynew  = NULL;
    A     = NULL;
    K1    = (double *) calloc(n1, sizeof(double));
    K2    = (double *) calloc(n1, sizeof(double));
    Jindx = (int *) calloc(n1, sizeof(int));
    Ynew  = (double *) calloc(n1, sizeof(double));
    A = createMatrix(n1, n1);
    if ( !Jindx || !Ynew || !K1 || !K2) return 0;
    if ( !A ) return 0;
    Nmax = n;
    return 1;
}

//=============================================================================

void ros2_close()
/*
**  Purpose:
**    closes the ROS2 integrator.
**
**  Input:
**    none.
*/
{
    if ( Jindx ) { free(Jindx); Jindx = NULL; }
    if ( Ynew ) { free(Ynew); Ynew = NULL; }
    if ( K1 ) { free(K1); K1 = NULL; }
    if ( K2 ) { free(K2); K2 = NULL; }
    freeMatrix(A);
    A = NULL;
}

//=============================================================================
      
int ros2_integrate(double y[], int n, double t, double tnext,
                   double* htry, double atol[], double rtol[],
                   void (*func)(double, double*, int, double*))
/*
**  Purpose:
**    integrates a system of ODEs over a specified time interval.
**
**  Input:
**    y[1..n] = vector of dependent variable values at the start
**              of the integration interval
**    n = number of dependent variables
**    t = time value at the start of the interval
**    tnext = time value at the end of the interval
**    htry = initial step size to be taken
**    atol[1..n] = vector of absolute tolerances on the variables y
**    rtol[1..n] = vector of relative tolerances on the variables y
**    func = name of the function that computes dy/dt for each y
**
**  Output:
**    htry = size of the last full time step taken.
**
**  Returns:
**    the number of times that func() was called, -1 if 
**    the Jacobian is singular, or -2 if the step size
**    shrinks to 0.
**
**  Notes:
**  1. The arguments to the function func() are:
**      t = current time
**      y[1..n] = vector of dependent variable values
**      n = number of dependent variables
**      dfdy[1..n] = vector of derivative values computed.
**
**  2. The arrays used in this function are 1-based, so
**     they must have been sized to n+1 when first created.
*/
{      
    double UROUND = 2.3e-16;
    double g, ghinv, ghinv1, dghinv, ytol;
    double h, hold, hmin, hmax, tplus;
    double ej, err, factor, facmax;
    int    nfcn, njac, naccept, nreject, j;
    int    isReject;
	int    adjust = Adjust;

// --- Initialize counters, etc.

    g = 1.0 + 1.0 / sqrt(2.0);
    ghinv1 = 0.0;
    tplus = t;
    isReject = 0;
    naccept  = 0;
    nreject  = 0;
    nfcn     = 0;
    njac     = 0;

// --- Initial step size

    hmax = tnext - t;
    hmin = 1.e-8;
    h = *htry;
    if ( h == 0.0 )
    {
        func(t, y, n, K1);
        nfcn += 1;
        adjust = 1;
        h = tnext - t;
        for (j=1; j<=n; j++)
        {
            ytol = atol[j] + rtol[j]*fabs(y[j]);
            if (K1[j] != 0.0) h = fmin(h, (ytol/fabs(K1[j])));
        }
    }
    h = fmax(hmin, h);
    h = fmin(hmax, h);

// --- Start the time loop 

    while ( t < tnext )
    {
    // --- check for zero step size

        if (0.10*fabs(h) <= fabs(t)*UROUND) return -2;

    // --- adjust step size if interval exceeded

        tplus = t + h;
        if ( tplus > tnext )
        {
            h = tnext - t;
            tplus = tnext;
        }

    // --- Re-compute the Jacobian if step size accepted

        if ( isReject == 0 )
        {
            jacobian(y, n, K1, K2, A, func);
            njac++;
            nfcn += 2*n;
            ghinv1 = 0.0;
        }

    // --- Update the Jacobian to reflect new step size

        ghinv = -1.0 / (g*h);
        dghinv = ghinv - ghinv1;
        for (j=1; j<=n; j++)
        {
            A[j][j] += dghinv;
        }
        ghinv1 = ghinv;
        if ( !factorize(A, n, K1, Jindx) ) return -1;

    // --- Stage 1 solution

        func(t, y, n, K1);
        nfcn += 1;
        for (j=1; j<=n; j++) K1[j] *= ghinv; 
        solve(A, n, Jindx, K1);

    // --- Stage 2 solution

        for (j=1; j<=n; j++)
        {
            Ynew[j] = y[j] + h*K1[j]; 
        }
        func(t, Ynew, n, K2);
        nfcn += 1;
        for (j=1; j<=n; j++)
        {
            K2[j] = (K2[j] - 2.0*K1[j])*ghinv;
        }
        solve(A, n, Jindx, K2);

    // --- Overall solution

        for (j=1; j<=n; j++)
        {
            Ynew[j] = y[j] + 1.5*h*K1[j] + 0.5*h*K2[j]; 
        }

    // --- Error estimation

        hold = h;
        err = 0.0;
        if ( adjust )
        {
            for (j=1; j<=n; j++)
            {
                ytol = atol[j] + rtol[j]*fabs(Ynew[j]);
	            ej = fabs(Ynew[j] - y[j] - h*K1[j])/ytol;
                err = err + ej*ej; 
            }
            err = sqrt(err/n);
            err = fmax(UROUND, err);

        // --- Choose the step size

            factor = 0.9 / sqrt(err);
            if (isReject) facmax = 1.0;
            else          facmax = 10.0;
            factor = fmin(factor, facmax);
            factor = fmax(factor, 1.0e-1);
            h = factor*h;
            h = fmin(hmax, h);
        }

    // --- Reject/accept the step

        if ( err > 1.0 )
        {
            isReject = 1;
            nreject++;
            h = 0.5*h;
        }
        else
        {
            isReject = 0;
            for (j=1; j<=n; j++)
            {
                y[j] = Ynew[j];
                if ( y[j] <= UROUND ) y[j] = 0.0;
            }
            if ( adjust ) *htry = h;
            t = tplus;    
            naccept++;
        }
        
// --- End of the time loop 

    }
    return nfcn;
}
