//  EPANETMSX.C  -- Command line implementation of EPANET-MSX

#include <stdlib.h>
#include <stdio.h>
#include "epanet2.h"                   // EPANET toolkit header file
#include "epanetmsx.h"                 // EPANET-MSX toolkit header file

int main(int argc, char* argv[])
/*
**  Purpose:
**    runs a multi-species EPANET analysis
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
**     - the name of a regular EPANET input data file
**     - the name of a EPANET-MSX input file
**     - the name of a report file that will contain status
**       messages and output results
**     - optionally, the name of an output file that will
**       contain water quality results in binary format.
*/
{
    int    err, done = 1;
    long   t, tleft;
    long   oldHour, newHour;

    // --- Check command line arguments

    if (argc < 4) {
        printf("\n Too few command line arguments.\n");
        return 0;
    }
    // --- Open the EPANET file

    printf("\n... EPANET-MSX Version 1.0\n");
    printf("\n  o Processing EPANET input file");
    err = ENopen(argv[1], argv[3], "");
    if (err) {
        printf("\n\n... Cannot read EPANET file; error code = %d\n", err);
        ENclose();
        return 0;
    }
    // ---  Begin an error detection loop

    do {

        // --- Open the MSX input file

        printf("\n  o Processing MSX input file");
        err = MSXopen(argv[2]);
        if (err) {
            printf("\n\n... Cannot read MSX file; error code = %d\n", err);
            break;
        }

        // --- Solve hydraulics

        printf("\n  o Computing network hydraulics");
        err = MSXsolveH();
        if (err) {
            printf(
                "\n\n... Cannot obtain network hydraulics; error code = %d\n", err);
            break;
        }

        // --- Initialize the multi-species analysis

        printf("\n  o Initializing network water quality");
        err = MSXinit(1);
        if (err) {
            printf(
                "\n\n... Cannot initialize EPANET-MSX; error code = %d\n", err);
            break;
        }
        t = 0;
        oldHour = -1;
        newHour = 0;
        printf("\n");

        // --- Repeat for each time step

        do {

            // --- Report current progress
            if (oldHour != newHour) {
                printf("\r  o Computing water quality at hour %-4d", newHour);
                oldHour = newHour;
            }

            // --- Compute water quality
            err = MSXstep(&t, &tleft);
            newHour = t / 3600;

        } while (!err && tleft > 0);
        // --- Report any runtime error

        if (err) {
            printf("\n\n... EPANET-MSX runtime error; error code = %d\n", err);
            break;
        }
        else
            printf("\r  o Computing water quality at hour %-4d", t / 3600);

        // --- Report results

        printf("\n  o Reporting water quality results");
        err = MSXreport();
        if (err) {
            printf(
                "\n\n... MSX report writer error; error code = %d\n", err);
            break;
        }

        // --- Save results to binary file if a file name was provided

        if (argc >= 5) {
            err = MSXsaveoutfile(argv[4]);
            if (err > 0) {
                printf(
                    "\n\n... Cannot save MSX results file; error code = %d\n", err);
                break;
            }
        }

        // --- End of error detection loop

    } while (!done);

    // --- Close both the multi-species & EPANET systems

    MSXclose();
    ENclose();
    if (!err) printf("\n\n... EPANET-MSX completed successfully.");
    printf("\n");
    return err;
}
