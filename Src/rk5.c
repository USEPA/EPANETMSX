/************************************************************************
**  MODULE:        RK5.C
**  PROJECT:       EPANET-MSX
**  DESCRIPTION:   Numerical solution of a system of first order
**                 ordinary differential equations dY/dt = F(t,Y).
**  AUTHOR:        L. Rossman, US EPA - NRMRL
**  VERSION:       1.1.00                                               
**  LAST UPDATE:   3/1/07
**
**  This is an explicit Runge-Kutta method of order (4)5  
**  due to Dormand & Prince (with optional stepsize control).
**  The code was adapted from the DOPRI5 code of E. Hairer
**  and G. Wanner as described in:
**    E. HAIRER, S.P. NORSETT AND G. WANNER, SOLVING ORDINARY
**    DIFFERENTIAL EQUATIONS I. NONSTIFF PROBLEMS. 2ND EDITION.
**    SPRINGER SERIES IN COMPUTATIONAL MATHEMATICS,
**    SPRINGER-VERLAG (1993)               
***********************************************************************/

#include <stdlib.h>
#include <math.h>
#include "rk5.h"

#define fmin(x,y) (((x)<=(y)) ? (x) : (y))     /* minimum of x and y    */
#define fmax(x,y) (((x)>=(y)) ? (x) : (y))     /* maximum of x and y    */

//  Local variables
//-----------------
int      Nmax;          // max. number of equations
int      Itmax;         // max. number of integration steps
int      Adjust;        // use adjustable step size
double   *Ak;           // work arrays
double   *K1;
double   *K2;
double   *K3;
double   *K4;
double   *K5;
double   *K6;
double   *Ynew;         // updated solution
void     (*Report) (double, double*, int);

//=============================================================================

int rk5_open(int n, int itmax, int adjust)
/*
**  Purpose:
**    Opens the RK5 solver to solve system of n equations
**
**  Input:
**    n = number of equtions
**    itmax = maximum iterations allowed
**    adjust = 1 if time step adjustment used, 0 if not
**
**  Returns:
**    1 if successful and 0 if not.
*/
{
    int n1 = n+1;
    Report = NULL;
    Nmax  = 0;
    Itmax = itmax;
    Adjust = adjust;
    Ynew  = (double *) calloc(n1, sizeof(double));
    Ak    = (double *) calloc(6*n1, sizeof(double));
    if ( !Ynew || !Ak ) return 0;
    Nmax = n;
    K1 = (Ak);
    K2 = ((Ak)+(n1));
    K3 = ((Ak)+(2*n1));
    K4 = ((Ak)+(3*n1));
    K5 = ((Ak)+(4*n1));
    K6 = ((Ak)+(5*n1));
    return 1;
}

//=============================================================================

void rk5_close()
/*
**  Purpose:
**    Closes the RK5 solver.
*/
{
    if ( Ynew ) free(Ynew);
    Ynew = NULL;
    if ( Ak ) free(Ak);
    Ak = NULL;
    Nmax = 0;
    Report = NULL;
}

//=============================================================================

int rk5_integrate(double y[], int n, double t, double tnext,
                  double* htry, double atol[], double rtol[],
                  void (*func)(double, double*, int, double*))
/*
**  Purpose:
**    Integrates system of equations dY/dt = F(t,Y) over a
**    given interval.
**
**  Input:
**    y[]    =  values of dependent variables at start of interval
**    n      =  number of dependent variables
**    t      =  value of independent variable at start of interval
**    tnext  =  value of independent variable at end of interval
**    htry   =  initial step size
**    atol[] =  absolute error tolerance on each dependent variable
**    rtol[] =  relative error tolerance on each dependent variable
**    func   =  pointer to function that evaluates dY/dt at given
**              values of t and Y.
**
**  Output:
**    y[]    =  values of dependent variables at end of interval
**    htry   =  last step size used
**
**  Returns:
**    number of function evaluations if successful, -1 if not
**    successful within Itmax iterations or -2 if step size
**    shrinks to 0.
*/ 
{
    double c2=0.20, c3=0.30, c4=0.80, c5=8.0/9.0;
    double a21=0.20, a31=3.0/40.0, a32=9.0/40.0,
           a41=44.0/45.0, a42=-56.0/15.0, a43=32.0/9.0,
           a51=19372.0/6561.0, a52=-25360.0/2187.0, a53=64448.0/6561.0,
           a54=-212.0/729.0, a61=9017.0/3168.0, a62=-355.0/33.0,
           a63=46732.0/5247.0, a64=49.0/176.0, a65=-5103.0/18656.0,
           a71=35.0/384.0, a73=500.0/1113.0, a74=125.0/192.0,
           a75=-2187.0/6784.0, a76=11.0/84.0;
    double e1=71.0/57600.0, e3=-71.0/16695.0, e4=71.0/1920.0,
           e5=-17253.0/339200.0, e6=22.0/525.0, e7=-1.0/40.0;

    double tnew, h, hmax, hnew, ytol, err, sk, fac, fac11 = 1.0;
    int    i;

// --- parameters for step size control

    double UROUND = 2.3e-16;
    double SAFE = 0.90;
    double fac1 = 0.2;
    double fac2 = 10.0;
    double beta = 0.04;
    double facold = 1.e-4;
    double expo1 = 0.2 - beta*0.75;
    double facc1 = 1.0/fac1;
    double facc2 = 1.0/fac2;

// --- various counters

    int    nstep = 1;
    int    nfcn  = 0;
    int    naccpt = 0;
    int    nrejct = 0;
    int    reject = 0;
    int    adjust = Adjust;

// --- initial function evaluation

    func(t, y, n, K1);
    nfcn++;

// --- initial step size
    h = *htry;
    hmax = tnext - t;
    if (h == 0.0)
    {
        adjust = 1;
        h = tnext - t;
        for (i=1; i<=n; i++)
        {
            ytol = atol[i] + rtol[i]*fabs(y[i]);
            if (K1[i] != 0.0) h = fmin(h, (ytol/fabs(K1[i])));
        }
    }
    h = fmax(1.e-8, h);

// --- while not at end of time interval

    while (t < tnext)
    {
    // --- check for zero step size
        if (0.10*fabs(h) <= fabs(t)*UROUND) return -2;

    // --- adjust step size if interval exceeded
        if ((t + 1.01*h - tnext) > 0.0) h = tnext - t;

        tnew = t + c2*h;
        for (i=1; i<=n; i++)
            Ynew[i] = y[i] + h*a21*K1[i];
        func(tnew, Ynew, n, K2);

        tnew = t + c3*h;
        for (i=1; i<=n; i++)
            Ynew[i] = y[i] + h*(a31*K1[i] + a32*K2[i]);
        func(tnew, Ynew, n, K3);

        tnew = t + c4*h;
        for (i=1; i<=n; i++)
            Ynew[i]=y[i] + h*(a41*K1[i] + a42*K2[i] + a43*K3[i]);
        func(tnew, Ynew, n, K4);

        tnew = t + c5*h;
        for (i=1; i<=n; i++)
            Ynew[i] = y[i] + h*(a51*K1[i] + a52*K2[i] + a53*K3[i]+a54*K4[i]);
        func(tnew, Ynew, n, K5);

        tnew = t + h;
        for (i=1; i<=n; i++)
            Ynew[i] = y[i] + h*(a61*K1[i] + a62*K2[i] + 
	                  a63*K3[i] + a64*K4[i] + a65*K5[i]);
        func(tnew, Ynew, n, K6);

        for (i=1; i<=n; i++)
            Ynew[i] = y[i] + h*(a71*K1[i] + a73*K3[i] + 
	                  a74*K4[i] + a75*K5[i] + a76*K6[i]);
        func(tnew, Ynew, n, K2);
        nfcn += 6;

    // --- step size adjustment

        err = 0.0;
        hnew = h;
        if (adjust)
        {
            for (i=1; i<=n; i++)
                K4[i] = (e1*K1[i] + e3*K3[i] + e4*K4[i] + e5*K5[i] +
                         e6*K6[i] + e7*K2[i])*h;
 
            for (i=1; i<=n; i++)
            {
                sk = atol[i] + rtol[i]*fmax(fabs(y[i]), fabs(Ynew[i]));
                sk = K4[i]/sk;
                err = err + (sk*sk);
            }
            err = sqrt(err/n);

            // --- computation of hnew
            fac11 = pow(err, expo1);
            fac = fac11/pow(facold, beta);               // LUND-stabilization
            fac = fmax(facc2, fmin(facc1, (fac/SAFE)));  // must have FAC1 <= HNEW/H <= FAC2
            hnew = h/fac;
        }
  
    // --- step is accepted 

        if( err <= 1.0 )
        {
           facold = fmax(err, 1.0e-4);
            naccpt++;
            for (i=1; i<=n; i++)
            {
                K1[i] = K2[i];
                y[i] = Ynew[i];
            }
            t = t + h;
            if ( adjust && t <= tnext ) *htry = h;
            if (fabs(hnew) > hmax) hnew = hmax; 
            if (reject) hnew = fmin(fabs(hnew), fabs(h));
            reject = 0;
            if (Report) Report(t, y, n);
        } 
  
    // --- step is rejected

        else
        {
            if ( adjust ) hnew = h/fmin(facc1, (fac11/SAFE));
            reject = 1; 
            if (naccpt >= 1) nrejct++;   
        }

    // --- take another step

        h = hnew;
        if ( adjust ) *htry = h;
        nstep++;
        if (nstep >= Itmax) return -1;
    }
    return nfcn;
}
