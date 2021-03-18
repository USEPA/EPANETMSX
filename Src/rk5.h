/************************************************************************
**  MODULE:        RK5.H
**  PROJECT:       EPANET-MSX
**  DESCRIPTION:   Header file for the ODE solver contained in RK5.C.
**  AUTHOR:        L. Rossman, US EPA - NRMRL
**  VERSION:       1.1.00                                               
**  LAST UPDATE:   3/1/07
***********************************************************************/

// Opens the ODE solver system
int  rk5_open(int n, int itmax, int adjust);

// Closes the ODE solver system
void rk5_close(void);

// Applies the solver to integrate a specific system of ODEs
int  rk5_integrate(double y[], int n, double t, double tnext,
                   double* htry, double atol[], double rtol[],
                   void (*func)(double, double*, int, double*));
