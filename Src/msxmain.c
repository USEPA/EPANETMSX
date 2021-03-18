/*******************************************************************************
**  MODULE:        MSXMAIN.C
**  PROJECT:       EPANET-MSX
**  DESCRIPTION:   Main module of the EPANET Multi-Species Extension toolkit.
**  COPYRIGHT:     Copyright (C) 2007 Feng Shang, Lewis Rossman, and James Uber.
**                 All Rights Reserved. See license information in LICENSE.TXT.
**  AUTHORS:       L. Rossman, US EPA - NRMRL
**                 F. Shang, University of Cincinnati
**                 J. Uber, University of Cincinnati
**  VERSION:       1.1.00
**  LAST UPDATE:   2/8/11
**
**  EPANET-MSX is an extension of the EPANET program for modeling the fate
**  and transport of multiple interacting chemical species within a water
**  distribution system over an extended period of operation. This module
**  provides a main function for producing a stand-alone console
**  application version of EPANET-MSX. It is not needed when compiling
**  EPANET-MSX into a dynamic link library (DLL) of callable functions.
**
**  To use either the console version or the DLL a user must prepare a
**  regular EPANET input file that describes the pipe network layout
**  and its hydraulic properties as well as a special EPANET-MSX input file
**  that names the chemical species being modeled and specifies the reaction
**  rate and equilbrium expressions that define their chemical behavior. The
**  format of these files is described in the EPANET and EPANET-MSX Users
**  Manuals, respectively.
*******************************************************************************/
#define _CRT_SECURE_NO_DEPRECATE

#include <stdlib.h>
#include <stdio.h>
#include <float.h>

#include "epanet2.h"                   // EPANET toolkit header file
#include "epanetmsx.h"                 // EPANET-MSX toolkit header file

int main(int argc, char *argv[])
/*
**  Purpose:
**    main function for the console version of EPANET-MSX.
**
**  Input:
**    argc = number of command line arguments
**    argv = array of command line arguments.
**
**  Returns:
**    an error code (or 0 for no error).
**
**  Notes:
**    The command line arguments are:
**     - the name of the regular EPANET input data file
**     - the name of the EPANET-MSX input file
**     - the name of a report file that will contain status
**       messages and output results
**     - optionally, the name of an output file that will
**       contain water quality results in binary format.
*/
{
    int    err, done = 1;
    long   t, tleft;
    long   oldHour, newHour;

// --- check command line arguments

    if ( argc < 4 )
    {
        printf("\n Too few command line arguments.\n");
        return 0;
    }

// --- open EPANET file

    printf("\n... EPANET-MSX Version 1.1\n");                                  //1.1.00
    printf("\n  o Processing EPANET input file");
    err = ENopen(argv[1], argv[3], "");
    do
    {
        if (err)
        {
            printf("\n\n... Cannot read EPANET file; error code = %d\n", err);
            ENclose();
            return 0;
        }

    // --- open the MSX input file

        printf("\n  o Processing MSX input file   ");
        err = MSXopen(argv[2]);
        if (err)
        {
            printf("\n\n... Cannot read EPANET-MSX file; error code = %d\n", err);
            break;
        }

    //--- solve hydraulics

        printf("\n  o Computing network hydraulics");
        err = MSXsolveH();
        if (err)
        {
            printf("\n\n... Cannot obtain network hydraulics; error code = %d\n", err);
            break;
        }

    //--- Initialize the multi-species analysis

        printf("\n  o Initializing network water quality");
        err = MSXinit(1);
        if (err)
        {
            printf("\n\n... Cannot initialize EPANET-MSX; error code = %d\n", err);
            break;
        }
        t = 0;
        oldHour = -1;
        newHour = 0;
        printf("\n");

    //--- Run the multi-species analysis at each time step

        do
        {
            if ( oldHour != newHour )
            {
                printf("\r  o Computing water quality at hour %-4d", newHour);
                oldHour = newHour;
            }
            err = MSXstep(&t, &tleft);
            newHour = t / 3600;

        } while (!err && tleft > 0);
        if (err)
        {
            printf("\n\n... EPANET-MSX runtime error; error code = %d\n", err);
            break;
        }
        else 
            printf("\r  o Computing water quality at hour %-4d", t/3600);

    // --- report results

        printf("\n  o Reporting water quality results");
        err = MSXreport();
        if (err)
        {
            printf("\n\n... EPANET-MSX report writer error; error code = %d\n", err);
            break;
        }

    // --- save results to binary file if a file name was provided

        if ( argc >= 5 )
        {
            err = MSXsaveoutfile(argv[4]);
            if ( err > 0 )
            {
                printf("\n\n... Cannot save EPANET-MSX results file; error code = %d\n", err);
                break;
            }
        }

    } while(!done);

//--- Close both the multi-species & EPANET systems

    MSXclose();
    ENclose();
    if ( !err ) printf("\n\n... EPANET-MSX completed successfully.");
    printf("\n");
    return err;
}
