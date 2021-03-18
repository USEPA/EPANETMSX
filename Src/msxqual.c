/******************************************************************************
**  MODULE:        MSXQUAL.C
**  PROJECT:       EPANET-MSX
**  DESCRIPTION:   Water quality routing routines.
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
#include <malloc.h>
#include <math.h>

#include "msxtypes.h"
#include "mempool.h"
#include "msxutils.h"

// Macros to identify upstream & downstream nodes of a link
// under the current flow and to compute link volume
//
#define   UP_NODE(x)   ( (FlowDir[(x)]=='+') ? MSX.Link[(x)].n1 : MSX.Link[(x)].n2 )
#define   DOWN_NODE(x) ( (FlowDir[(x)]=='+') ? MSX.Link[(x)].n2 : MSX.Link[(x)].n1 )
#define   LINKVOL(k)   ( 0.785398*MSX.Link[(k)].len*SQR(MSX.Link[(k)].diam) )

//  External variables
//--------------------
extern MSXproject  MSX;                // MSX project data

//  Local variables
//-----------------
static Pseg           FreeSeg;         // pointer to unused pipe segment
static Pseg           *NewSeg;         // new segment added to each pipe
static char           *FlowDir;        // flow direction for each pipe
static double         *VolIn;          // inflow flow volume to each node
static double         **MassIn;        // mass inflow of each species to each node
static double         **X;             // work matrix
static char           HasWallSpecies;  // wall species indicator
static char           OutOfMemory;     // out of memory indicator
static alloc_handle_t *QualPool;       // memory pool

//  Imported functions
//--------------------
int    MSXchem_open(void);
void   MSXchem_close(void);
int    MSXchem_react(long dt);
int    MSXchem_equil(int zone, double *c);

void   MSXtank_mix1(int i, double vIn, double cIn[], long dt);
void   MSXtank_mix2(int i, double vIn, double cIn[], long dt);
void   MSXtank_mix3(int i, double vIn, double cIn[], long dt);
void   MSXtank_mix4(int i, double vIn, double cIn[], long dt);

int    MSXout_open(void);
int    MSXout_saveResults(void);
int    MSXout_saveFinalResults(void);

void   MSXerr_clearMathError(void);                                            //1.1.00
int    MSXerr_mathError(void);                                                 //1.1.00
char*  MSXerr_writeMathErrorMsg(void);                                         //1.1.00

//  Exported functions
//--------------------
int    MSXqual_open(void);
int    MSXqual_init(void);
int    MSXqual_step(long *t, long *tleft);
int    MSXqual_close(void);
double MSXqual_getNodeQual(int j, int m);
double MSXqual_getLinkQual(int k, int m);
int    MSXqual_isSame(double c1[], double c2[]);
void   MSXqual_removeSeg(Pseg seg);
Pseg   MSXqual_getFreeSeg(double v, double c[]);
void   MSXqual_addSeg(int k, Pseg seg);

//  Local functions
//-----------------
static int    getHydVars(void);
static int    transport(long tstep);
static void   initSegs(void);
static void   reorientSegs(void);
static void   advectSegs(long dt);
static void   getNewSegWallQual(int k, long dt, Pseg seg);
static void   shiftSegWallQual(int k, long dt);
static void   accumulate(long dt);
static void   getIncidentConcen(void);
static void   updateNodes(long dt);
static void   sourceInput(long dt);
static void   addSource(int n, Psource source, double v, long dt);
static void   release(long dt);
static double getSourceQual(Psource source);
static void   removeAllSegs(int k);


//=============================================================================

int  MSXqual_open()
/*
**   Purpose:
**     opens the WQ routing system.
**
**   Returns:
**     an error code (0 if no errors).
*/
{
    int errcode = 0;
    int n;

// --- set flags

    MSX.QualityOpened = FALSE;
    MSX.Saveflag = 0;
    OutOfMemory = FALSE;
    HasWallSpecies = FALSE;

// --- initialize array pointers to null

    MSX.C1 = NULL;
    MSX.FirstSeg = NULL;
    MSX.LastSeg = NULL;
    X = NULL;
    NewSeg = NULL;
    FlowDir = NULL;
    VolIn = NULL;
    MassIn = NULL;

// --- open the chemistry system

    errcode = MSXchem_open();
    if ( errcode > 0 ) return errcode;

// --- allocate a memory pool for pipe segments

    QualPool = AllocInit();
    if ( QualPool == NULL ) return ERR_MEMORY;

// --- allocate memory used for species concentrations

    X = createMatrix(MSX.Nobjects[NODE]+1, MSX.Nobjects[SPECIES]+1);
    MSX.C1 = (double *) calloc(MSX.Nobjects[SPECIES]+1, sizeof(double));

// --- allocate memory used for pointers to the first, last,
//     and new WQ segments in each link and tank

    n = MSX.Nobjects[LINK] + MSX.Nobjects[TANK] + 1;
    MSX.FirstSeg = (Pseg *) calloc(n, sizeof(Pseg));
    MSX.LastSeg  = (Pseg *) calloc(n, sizeof(Pseg));
    NewSeg = (Pseg *) calloc(n, sizeof(Pseg));

// --- allocate memory used for flow direction in each link

    FlowDir  = (char *) calloc(n, sizeof(char));

// --- allocate memory used to accumulate mass and volume
//     inflows to each node

    n        = MSX.Nobjects[NODE] + 1;
    VolIn    = (double *) calloc(n, sizeof(double));
    MassIn   = createMatrix(n, MSX.Nobjects[SPECIES]+1);

// --- check for successful memory allocation

    CALL(errcode, MEMCHECK(X));
    CALL(errcode, MEMCHECK(MSX.C1));
    CALL(errcode, MEMCHECK(MSX.FirstSeg));
    CALL(errcode, MEMCHECK(MSX.LastSeg));
    CALL(errcode, MEMCHECK(NewSeg));
    CALL(errcode, MEMCHECK(FlowDir));
    CALL(errcode, MEMCHECK(VolIn));
    CALL(errcode, MEMCHECK(MassIn));

// --- check if wall species are present

    for (n=1; n<=MSX.Nobjects[SPECIES]; n++)
    {
        if ( MSX.Species[n].type == WALL ) HasWallSpecies = TRUE;
    }
    if ( !errcode ) MSX.QualityOpened = TRUE;
    return(errcode);
}

//=============================================================================

int  MSXqual_init()
/*
**  Purpose:
**     re-initializes the WQ routing system.
**
**  Input:
**    none.
**
**  Returns:
**    an error code (or 0 if no errors).
*/
{
    int i, n, m;
    int errcode = 0;

// --- initialize node concentrations, tank volumes, & source mass flows

    for (i=1; i<=MSX.Nobjects[NODE]; i++)
    {
        for (m=1; m<=MSX.Nobjects[SPECIES]; m++)
            MSX.Node[i].c[m] = MSX.Node[i].c0[m];
    }
    for (i=1; i<=MSX.Nobjects[TANK]; i++)
    {
        MSX.Tank[i].hstep = 0.0;
        MSX.Tank[i].v = MSX.Tank[i].v0;
        n = MSX.Tank[i].node;
        for (m=1; m<=MSX.Nobjects[SPECIES]; m++)
            MSX.Tank[i].c[m] = MSX.Node[n].c0[m];
    }
    for (i=1; i<=MSX.Nobjects[PATTERN]; i++)
    {
        MSX.Pattern[i].interval = 0;
        MSX.Pattern[i].current = MSX.Pattern[i].first;
    }

// --- copy expression constants to vector MSX.K[]                             //1.1.00

    for (i=1; i<=MSX.Nobjects[CONSTANT]; i++)
    {
        MSX.K[i] = MSX.Const[i].value;
    }

// --- check if a separate WQ report is required

    MSX.Rptflag = 0;
    n = 0;
    for (i=1; i<=MSX.Nobjects[NODE]; i++) n += MSX.Node[i].rpt;
    for (i=1; i<=MSX.Nobjects[LINK]; i++) n += MSX.Link[i].rpt;
    if ( n > 0 )
    {
        n = 0;
        for (m=1; m<=MSX.Nobjects[SPECIES]; m++) n += MSX.Species[m].rpt;
    }
    if ( n > 0 ) MSX.Rptflag = 1;
    if ( MSX.Rptflag ) MSX.Saveflag = 1;

// --- reset memory pool

    AllocSetPool(QualPool);
    FreeSeg = NULL;
    AllocReset();

// --- re-position hydraulics file

    fseek(MSX.HydFile.file, MSX.HydOffset, SEEK_SET);

// --- set elapsed times to zero

    MSX.Htime = 0;                         //Hydraulic solution time
    MSX.Qtime = 0;                         //Quality routing time
    MSX.Rtime = MSX.Rstart;                //Reporting time
    MSX.Nperiods = 0;                      //Number fo reporting periods

// --- open binary output file if results are to be saved

    if ( MSX.Saveflag ) errcode = MSXout_open();
    return errcode;
}

//=============================================================================

int MSXqual_step(long *t, long *tleft)
/*
**  Purpose:
**    updates WQ conditions over a single WQ time step.
**
**  Input:
**    none.
**
**  Output:
**    *t = current simulation time (sec)
**    *tleft = time left in simulation (sec)
**
**  Returns:
**    an error code:
**      0 = no error
**      501 = memory error
**      307 = can't read hydraulics file
**      513 = can't integrate reaction rates
*/
{
    long dt, hstep, tstep;
    int  errcode = 0;

// --- set the shared memory pool to the water quality pool
//     and the overall time step to nominal WQ time step

    AllocSetPool(QualPool);
    tstep = MSX.Qstep;

// --- repeat until the end of the time step

    do
    {
    // --- find the time until the next hydraulic event occurs
        dt = tstep;
        hstep = MSX.Htime - MSX.Qtime;

    // --- check if next hydraulic event occurs within the current time step

        if (hstep <= dt)
        {

        // --- reduce current time step to end at next hydraulic event
            dt = hstep;

        // --- route WQ over this time step
            if ( dt > 0 ) CALL(errcode, transport(dt));
            MSX.Qtime += dt;

        // --- retrieve new hydraulic solution
            if ( MSX.Qtime == MSX.Htime ) CALL(errcode, getHydVars());

        // --- report results if its time to do so
            if (MSX.Saveflag && MSX.Qtime == MSX.Rtime)
            {
                CALL(errcode, MSXout_saveResults());
                MSX.Rtime += MSX.Rstep;
                MSX.Nperiods++;
            }
        }

    // --- otherwise just route WQ over the current time step

        else
        {
            CALL(errcode, transport(dt));
            MSX.Qtime += dt;
        }

    // --- reduce overall time step by the size of the current time step

        tstep -= dt;
        if (OutOfMemory) errcode = ERR_MEMORY;
    } while (!errcode && tstep > 0);

// --- update the current time into the simulation and the amount remaining

    *t = MSX.Qtime;
    *tleft = MSX.Dur - MSX.Qtime;

// --- if there's no time remaining, then save the final records to output file

    if ( *tleft <= 0 && MSX.Saveflag )
    {
        CALL(errcode, MSXout_saveFinalResults());
    }
    return errcode;
}

//=============================================================================

double  MSXqual_getNodeQual(int j, int m)
/*
**   Purpose:
**     retrieves WQ for species m at node n.
**
**   Input:
**     j = node index
**     m = species index.
**
**   Returns:
**     WQ value of node.
*/
{
    int k;

// --- return 0 for WALL species

    if ( MSX.Species[m].type == WALL ) return 0.0;

// --- if node is a tank, return its internal concentration

    k = MSX.Node[j].tank;
    if (k > 0 && MSX.Tank[k].a > 0.0)
    {
        return MSX.Tank[k].c[m];
    }

// --- otherwise return node's concentration (which includes
//     any contribution from external sources)

    return MSX.Node[j].c[m];
}

//=============================================================================

double  MSXqual_getLinkQual(int k, int m)
/*
**   Purpose:
**     computes average quality in link k.
**
**   Input:
**     k = link index
**     m = species index.
**
**   Returns:
**     WQ value of link.
*/
{
    double  vsum = 0.0,
            msum = 0.0;
    Pseg    seg;

    seg = MSX.FirstSeg[k];
    while (seg != NULL)
    {
        vsum += seg->v;
        msum += (seg->c[m])*(seg->v);
        seg = seg->prev;
    }
    if (vsum > 0.0) return(msum/vsum);
    else
    {
        return (MSXqual_getNodeQual(MSX.Link[k].n1, m) +
                MSXqual_getNodeQual(MSX.Link[k].n2, m)) / 2.0;
    }
}

//=============================================================================

int MSXqual_close()
/*
**   Purpose:
**     closes the WQ routing system.
**
**   Input:
**     none.
**
**   Returns:
**     error code (0 if no error).
*/
{
    int errcode = 0;
    if ( !MSX.ProjectOpened ) return 0;
    MSXchem_close();
    FREE(MSX.C1);
    FREE(MSX.FirstSeg);
    FREE(MSX.LastSeg);
    FREE(NewSeg);
    FREE(FlowDir);
    FREE(VolIn);
    freeMatrix(MassIn);
    freeMatrix(X);
    if ( QualPool)
    {
        AllocSetPool(QualPool);
        AllocFreePool();
    }
    MSX.QualityOpened = FALSE;
    return errcode;
}

//=============================================================================

int  MSXqual_isSame(double c1[], double c2[])
/*
**   Purpose:
**     checks if two sets of concentrations are the same
**
**   Input:
**     c1[] = first set of species concentrations
**     c2[] = second set of species concentrations
**
**   Returns:
**     1 if the concentrations are all within a specific tolerance of each
**     other or 0 if they are not.
*/
{
    int m;
    for (m=1; m<=MSX.Nobjects[SPECIES]; m++)
    {
        if ( fabs(c1[m] - c2[m]) >= MSX.Species[m].aTol ) return 0;
    }
    return 1;
}

//=============================================================================

int  getHydVars()
/*
**   Purpose:
**     retrieves hydraulic solution and time step for next hydraulic event
**     from a hydraulics file.
**
**   Input:
**     none.
**
**   Returns:
**     error code (0 if no error).
**
**   NOTE:
**     A hydraulic solution consists of the current time
**     (hydtime), nodal demands (D) and heads (H), link
**     flows (Q), and link status values and settings (which are not used).
*/
{
    int  errcode = 0;
    long hydtime, hydstep;
    INT4 n;

// --- read hydraulic time, demands, heads, and flows from the file

    if (fread(&n, sizeof(INT4), 1, MSX.HydFile.file) < 1)
        return ERR_READ_HYD_FILE;
    hydtime = (long)n;
    n = MSX.Nobjects[NODE];
    if (fread(MSX.D+1, sizeof(REAL4), n, MSX.HydFile.file) < (unsigned)n)
        return ERR_READ_HYD_FILE;
    if (fread(MSX.H+1, sizeof(REAL4), n, MSX.HydFile.file) < (unsigned)n)
        return ERR_READ_HYD_FILE;
    n = MSX.Nobjects[LINK];
    if (fread(MSX.Q+1, sizeof(REAL4), n, MSX.HydFile.file) < (unsigned)n)
        return ERR_READ_HYD_FILE;

// --- skip over link status and settings

    fseek(MSX.HydFile.file, 2*n*sizeof(REAL4), SEEK_CUR);

// --- read time step until next hydraulic event

    if (fread(&n, sizeof(INT4), 1, MSX.HydFile.file) < 1)
        return ERR_READ_HYD_FILE;
    hydstep = (long)n;

// --- update elapsed time until next hydraulic event

    MSX.Htime = hydtime + hydstep;

// --- initialize pipe segments (at time 0) or else re-orient segments
//     to accommodate any flow reversals

    if (MSX.Qtime < MSX.Dur)
    {
        if (MSX.Qtime == 0) initSegs();
        else reorientSegs();
    }
    return errcode;
}

//=============================================================================

int  transport(long tstep)
/*
**  Purpose:
**    transports constituent mass through pipe network
**    under a period of constant hydraulic conditions.
**
**  Input:
**    tstep = length of current time step (sec).
**
**  Returns:
**    an error code or 0 if no error.
*/
{
    long qtime, dt;
    int  errcode = 0;

// --- repeat until time step is exhausted

    MSXerr_clearMathError();                // clear math error flag           //1.1.00
    qtime = 0;
    while (!OutOfMemory &&
           !errcode &&
           qtime < tstep)
    {                                       // Qstep is nominal quality time step
        dt = MIN(MSX.Qstep, tstep-qtime);   // get actual time step
        qtime += dt;                        // update amount of input tstep taken
        errcode = MSXchem_react(dt);        // react species in each pipe & tank
        if ( errcode ) return errcode;
        advectSegs(dt);                     // advect segments in each pipe
        accumulate(dt);                     // accumulate all inflows at nodes
        updateNodes(dt);                    // update nodal quality
        sourceInput(dt);                    // compute nodal inputs from sources
        release(dt);                        // release new outflows from nodes
		if (MSXerr_mathError())             // check for any math error        //1.1.00
		{
			MSXerr_writeMathErrorMsg();
			errcode = ERR_ILLEGAL_MATH;
		}
   }
   return errcode;
}

//=============================================================================

void  initSegs()
/*
**   Purpose:
**     initializes water quality in pipe segments.
**
**   Input:
**     none.
*/
{
    int     j, k, m;
    double  v;

// --- examine each link

    for (k=1; k<=MSX.Nobjects[LINK]; k++)
    {
    // --- establish flow direction

        FlowDir[k] = '+';
        if (MSX.Q[k] < 0.) FlowDir[k] = '-';

    // --- start with no segments

        MSX.LastSeg[k] = NULL;
        MSX.FirstSeg[k] = NULL;
        NewSeg[k] = NULL;

    // --- use quality of downstream node for BULK species
    //     if no initial link quality supplied

        j = DOWN_NODE(k);
        for (m=1; m<=MSX.Nobjects[SPECIES]; m++)
        {
            if ( MSX.Link[k].c0[m] != MISSING )
                MSX.C1[m] = MSX.Link[k].c0[m];
            else if ( MSX.Species[m].type == BULK )
                MSX.C1[m] = MSX.Node[j].c0[m];
            else MSX.C1[m] = 0.0;
        }

    // --- fill link with a single segment of this quality

        v = LINKVOL(k);
        if ( v > 0.0 ) MSXqual_addSeg(k, MSXqual_getFreeSeg(v, MSX.C1));
    }

// --- initialize segments in tanks

    for (j=1; j<=MSX.Nobjects[TANK]; j++)
    {
    // --- skip reservoirs

        if ( MSX.Tank[j].a == 0.0 ) continue;

    // --- tank segment pointers are stored after those for links

        k = MSX.Tank[j].node;
        for (m=1; m<=MSX.Nobjects[SPECIES]; m++)
            MSX.C1[m] = MSX.Node[k].c0[m];
        k = MSX.Nobjects[LINK] + j;
        MSX.LastSeg[k] = NULL;
        MSX.FirstSeg[k] = NULL;

    // --- add 2 segments for 2-compartment model

        if (MSX.Tank[j].mixModel == MIX2)
        {
            v = MAX(0, MSX.Tank[j].v - MSX.Tank[j].vMix);
            MSXqual_addSeg(k, MSXqual_getFreeSeg(v, MSX.C1));
            v = MSX.Tank[j].v - v;
            MSXqual_addSeg(k, MSXqual_getFreeSeg(v, MSX.C1));
        }

    // --- add one segment for all other models

        else
        {
            v = MSX.Tank[j].v;
            MSXqual_addSeg(k, MSXqual_getFreeSeg(v, MSX.C1));
        }
    }
}

//=============================================================================

void  reorientSegs()
/*
**   Purpose:
**     re-orients pipe segments (if flow reverses).
**
**   Input:
**     none.
*/
{
    int    k;
    char   newdir;
    Pseg   seg, pseg, nseg;

// --- examine each link

    for (k=1; k<=MSX.Nobjects[LINK]; k++)
    {
    // --- find new flow direction

        newdir = '+';
        if (MSX.Q[k] == 0.0)     newdir = FlowDir[k];
        else if (MSX.Q[k] < 0.0) newdir = '-';

    // --- if direction changes, then reverse the order of segments
    //     (first to last) and save new direction

        if (newdir != FlowDir[k])
        {
            seg = MSX.FirstSeg[k];
            MSX.FirstSeg[k] = MSX.LastSeg[k];
            MSX.LastSeg[k] = seg;
            pseg = NULL;
            while (seg != NULL)
            {
                nseg = seg->prev;
                seg->prev = pseg;
                seg->next = nseg;
                pseg = seg;
                seg = nseg;
            }
            FlowDir[k] = newdir;
        }
    }
}

//=============================================================================

void advectSegs(long dt)
/*
**   Purpose:
**     advects WQ segments within each pipe.
**
**   Input:
**     dt = current WQ time step (sec).
*/
{
    int k, m;

// --- examine each link

    for (k=1; k<=MSX.Nobjects[LINK]; k++)
    {
    // --- zero out WQ in new segment to be added at entrance of link

        for (m=1; m<=MSX.Nobjects[SPECIES]; m++) MSX.C1[m] = 0.0;

    // --- get a free segment to add to entrance of link

        NewSeg[k] = MSXqual_getFreeSeg(0.0, MSX.C1);

    // --- skip zero-length links (pumps & valves) & no-flow links

        if ( NewSeg[k] == NULL ||
             MSX.Link[(k)].len == 0.0 || MSX.Q[k] == 0.0 ) continue;

    // --- find conc. of wall species in new segment to be added
    //     and adjust conc. of wall species to reflect shifted
    //     positions of existing segments

        if ( HasWallSpecies )
        {
            getNewSegWallQual(k, dt, NewSeg[k]);
            shiftSegWallQual(k, dt);
        }
    }
}

//=============================================================================

void getNewSegWallQual(int k, long dt, Pseg newseg)
/*
**  Purpose:
**     computes wall species concentrations for a new WQ segment that
**     enters a pipe from its upstream node.
**
**  Input:
**    k = link index
**    dt = current WQ time step (sec)
**    newseg = pointer to a new, unused WQ segment
**
**  Output:
**    newseg->c[] = wall species concentrations in the new WQ segment
*/
{
    Pseg  seg;
    int   m;
    double v, vin, vsum, vadded, vleft;

// --- get volume of inflow to link

    if ( newseg == NULL ) return;
    v = LINKVOL(k);
	vin = ABS(MSX.Q[k])*dt;
    if (vin > v) vin = v;

// --- start at last (most upstream) existing WQ segment

	seg = MSX.LastSeg[k];
	vsum = 0.0;
    vleft = vin;
    for (m = 1; m <= MSX.Nobjects[SPECIES]; m++)
    {
        if ( MSX.Species[m].type == WALL ) newseg->c[m] = 0.0;
    }

// --- repeat while some inflow volume still remains

    while ( vleft > 0.0 && seg != NULL )
    {

    // --- find volume added by this segment

        vadded = seg->v;
        if ( vadded > vleft ) vadded = vleft;

    // --- update total volume added and inflow volume remaining

        vsum += vadded;
        vleft -= vadded;

    // --- add wall species mass contributed by this segment to new segment

        for (m = 1; m <= MSX.Nobjects[SPECIES]; m++)
        {
            if ( MSX.Species[m].type == WALL ) newseg->c[m] += vadded*seg->c[m];
        }

    // --- move to next downstream WQ segment

        seg = seg->next;
    }

// --- convert mass of wall species in new segment to concentration

    if ( vsum > 0.0 )
    {
        for (m = 1; m <= MSX.Nobjects[SPECIES]; m++)
        {
            if ( MSX.Species[m].type == WALL ) newseg->c[m] /= vsum;
        }
    }
}

//=============================================================================

void shiftSegWallQual(int k, long dt)
/*
**  Purpose:
**    recomputes wall species concentrations in segments that remain
**    within a pipe after flow is advected over current time step.
**
**  Input:
**    k = link index
**    dt = current WQ time step (sec)
*/
{
    Pseg  seg1, seg2;
    int   m;
    double v, vin, vstart, vend, vcur, vsum;

// --- find volume of water displaced in pipe

    v = LINKVOL(k);
	vin = ABS(MSX.Q[k])*dt;
    if (vin > v) vin = v;

// --- set future start position (measured by pipe volume) of original last segment

    vstart = vin;

// --- examine each segment, from upstream to downstream

    for( seg1 = MSX.LastSeg[k]; seg1 != NULL; seg1 = seg1->next )
    {
    // --- initialize a "mixture" WQ

        for (m = 1; m <= MSX.Nobjects[SPECIES]; m++) MSX.C1[m] = 0.0;

    // --- find the future end position of this segment

        vend = vstart + seg1->v;
        if (vend > v) vend = v;
        vcur = vstart;
        vsum = 0;

    // --- find volume taken up by the segment after it moves down the pipe

        for (seg2 = MSX.LastSeg[k]; seg2 != NULL; seg2 = seg2->next)
        {
            if ( seg2->v == 0.0 ) continue;
            vsum += seg2->v;
            if ( vsum >= vstart && vsum <= vend )
            {
                for (m = 1; m <= MSX.Nobjects[SPECIES]; m++)
                {
                    if ( MSX.Species[m].type == WALL )
                        MSX.C1[m] += (vsum - vcur) * seg2->c[m];
                }
                vcur = vsum;
            }
            if ( vsum >= vend ) break;
        }

    // --- update the wall species concentrations in the segment

        for (m = 1; m <= MSX.Nobjects[SPECIES]; m++)
        {
            if ( MSX.Species[m].type != WALL ) continue;
            if (seg2 != NULL) MSX.C1[m] += (vend - vcur) * seg2->c[m];
            seg1->c[m] = MSX.C1[m] / (vend - vstart);
            if ( seg1->c[m] < 0.0 ) seg1->c[m] = 0.0;
        }

    // --- re-start at the current end location

        vstart = vend;
        if ( vstart >= v ) break;
    }
}

//=============================================================================

void accumulate(long dt)
/*
**  Purpose:
**    accumulates mass inflow at downstream node of each link.
**
**  Input:
**    dt = current WQ time step (sec).
*/
{
    int    i, j, k, m, n;
    double cseg, v, vseg;
    Pseg   seg;

// --- compute average conc. of segments incident on each node
//     (for use if there is no transport through the node)

    getIncidentConcen();

// --- reset cumlulative inflow to each node to zero

    memset(VolIn, 0, (MSX.Nobjects[NODE]+1)*sizeof(double));
    n = (MSX.Nobjects[NODE]+1)*(MSX.Nobjects[SPECIES]+1);
    memset(MassIn[0], 0, n*sizeof(double));

// --- move mass from first segment of each link into link's downstream node

    for (k=1; k<=MSX.Nobjects[LINK]; k++)
    {
        i = UP_NODE(k);               // upstream node
        j = DOWN_NODE(k);             // downstream node
        v = ABS(MSX.Q[k])*dt;         // flow volume

    // --- if link volume < flow volume, then transport upstream node's
    //     quality to downstream node and remove all link segments

        if (LINKVOL(k) < v)
        {
            VolIn[j] += v;
            seg = MSX.FirstSeg[k];
            for (m=1; m<=MSX.Nobjects[SPECIES]; m++)
            {
                if ( MSX.Species[m].type != BULK ) continue;
                cseg = MSX.Node[i].c[m];
                if (seg != NULL) cseg = seg->c[m];
                MassIn[j][m] += v*cseg;
             }
             removeAllSegs(k);
        }

    // --- otherwise remove flow volume from leading segments
    //     and accumulate flow mass at downstream node

        else while (v > 0.0)
        {
        // --- identify leading segment in pipe

            seg = MSX.FirstSeg[k];
            if (seg == NULL) break;

        // --- volume transported from this segment is
        //     minimum of remaining flow volume & segment volume
        //     (unless leading segment is also last segment)

            vseg = seg->v;
            vseg = MIN(vseg, v);
            if (seg == MSX.LastSeg[k]) vseg = v;

        // --- update volume & mass entering downstream node

            for (m=1; m<=MSX.Nobjects[SPECIES]; m++)
            {
                if ( MSX.Species[m].type != BULK ) continue;
                cseg = seg->c[m];
                MassIn[j][m] += vseg*cseg;
            }
            VolIn[j] += vseg;

        // --- reduce flow volume by amount transported

            v -= vseg;

        // --- if all of segment's volume was transferred, then
        //     replace leading segment with the one behind it
        //     (Note that the current seg is recycled for later use.)

            if (v >= 0.0 && vseg >= seg->v)
            {
                MSX.FirstSeg[k] = seg->prev;
                if (MSX.FirstSeg[k] == NULL) MSX.LastSeg[k] = NULL;
                MSXqual_removeSeg(seg);
            }

        // --- otherwise reduce segment's volume

            else
            {
                seg->v -= vseg;
            }

       } // End while

    } // Next link
}

//=============================================================================

void getIncidentConcen()
/*
**  Purpose:
**    determines average WQ for bulk species in link end segments that are
**    incident on each node.
**
**  Input:
**    none
*/
{
    int j, k, m, n;

// --- zero-out memory used to store accumulated totals

    memset(VolIn, 0, (MSX.Nobjects[NODE]+1)*sizeof(double));
    n = (MSX.Nobjects[NODE]+1)*(MSX.Nobjects[SPECIES]+1);
    memset(MassIn[0], 0, n*sizeof(double));
    memset(X[0], 0, n*sizeof(double));

// --- examine each link

    for (k=1; k<=MSX.Nobjects[LINK]; k++)
    {
        j = DOWN_NODE(k);             // downstream node
        if (MSX.FirstSeg[k] != NULL)  // accumulate concentrations
        {
            for (m=1; m<=MSX.Nobjects[SPECIES]; m++)
            {
                if ( MSX.Species[m].type == BULK )
                  MassIn[j][m] += MSX.FirstSeg[k]->c[m];
            }
            VolIn[j]++;
        }
        j = UP_NODE(k);              // upstream node
        if (MSX.LastSeg[k] != NULL)  // accumulate concentrations
        {
            for (m=1; m<=MSX.Nobjects[SPECIES]; m++)
            {
                if ( MSX.Species[m].type == BULK )
                    MassIn[j][m] += MSX.LastSeg[k]->c[m];
            }
            VolIn[j]++;
        }
    }

// --- compute avg. incident concen. at each node

    for (k=1; k<=MSX.Nobjects[NODE]; k++)
    {
        if (VolIn[k] > 0.0)
        {
            for (m=1; m<=MSX.Nobjects[SPECIES]; m++)
                X[k][m] = MassIn[k][m]/VolIn[k];
        }
    }
}

//=============================================================================

void updateNodes(long dt)
/*
**  Purpose:
**    updates the concentration at each node to the mixture
**    concentration of the accumulated inflow from connecting pipes.
**
**  Input:
**    dt = current WQ time step (sec)
**
**  Note:
**    Does not account for source flow effects. X[i][] contains
**    average concen. of segments adjacent to node i, used in case
**    there was no inflow into node i.
*/
{
    int i, j, m;

// --- examine each node

    for (i=1; i<=MSX.Nobjects[NODE]; i++)
    {
    // --- node is a junction

        j = MSX.Node[i].tank;
        if (j <= 0)
        {
        // --- add any external inflow (i.e., negative demand)
        //     to total inflow volume

            if (MSX.D[i] < 0.0) VolIn[i] -= MSX.D[i]*dt;

        // --- if inflow volume is non-zero, then compute the mixture
        //     concentration resulting at the node

            if (VolIn[i] > 0.0)
            {
                for (m=1; m<=MSX.Nobjects[SPECIES]; m++)
                    MSX.Node[i].c[m] = MassIn[i][m]/VolIn[i];
            }

        // --- otherwise use the avg. of the concentrations in the
        //     links incident on the node

            else
            {
                for (m=1; m<=MSX.Nobjects[SPECIES]; m++)
                    MSX.Node[i].c[m] = X[i][m];
            }

        // --- compute new equilibrium mixture

            MSXchem_equil(NODE, MSX.Node[i].c);
        }

    // --- node is a tank or reservoir

        else
        {
        // --- use initial quality for reservoirs

            if (MSX.Tank[j].a == 0.0)
            {
                for (m=1; m<=MSX.Nobjects[SPECIES]; m++)
                    MSX.Node[i].c[m] = MSX.Node[i].c0[m];
            }

        // --- otherwise update tank WQ based on mixing model

            else
            {
                if (VolIn[i] > 0.0)
                {
                    for (m=1; m<=MSX.Nobjects[SPECIES]; m++)
                    {
                        MSX.C1[m] = MassIn[i][m]/VolIn[i];
                    }
                }
                else for (m=1; m<=MSX.Nobjects[SPECIES]; m++) MSX.C1[m] = 0.0;
                switch(MSX.Tank[j].mixModel)
                {
                    case MIX1: MSXtank_mix1(j, VolIn[i], MSX.C1, dt);
                               break;
                    case MIX2: MSXtank_mix2(j, VolIn[i], MSX.C1, dt);
                               break;
                    case FIFO: MSXtank_mix3(j, VolIn[i], MSX.C1, dt);
                               break;
                    case LIFO: MSXtank_mix4(j, VolIn[i], MSX.C1, dt);
                               break;
                }
                for (m=1; m<=MSX.Nobjects[SPECIES]; m++)
                    MSX.Node[i].c[m] = MSX.Tank[j].c[m];
                MSX.Tank[j].v += MSX.D[i]*dt;
            }
        }
    }
}

//=============================================================================

void sourceInput(long dt)
/*
**  Purpose:
**    computes contribution (if any) of mass additions from WQ
**    sources at each node.
**
**  Input:
**    dt = current WQ time step (sec)
*/
{
    int     n;
    double  qout, qcutoff, volout;
    Psource source;

// --- establish a flow cutoff which indicates no outflow from a node

    qcutoff = 10.0*TINY;

// --- consider each node

    for (n=1; n<=MSX.Nobjects[NODE]; n++)
    {
    // --- skip node if no WQ source

        source = MSX.Node[n].sources;
        if (source == NULL) continue;

    // --- find total flow volume leaving node

        if (MSX.Node[n].tank == 0) volout = VolIn[n];  // Junctions
        else volout = VolIn[n] - MSX.D[n]*dt;          // Tanks
        qout = volout / (double) dt;

    // --- evaluate source input only if node outflow > cutoff flow

        if (qout <= qcutoff) continue;

    // --- add contribution of each source species

        while (source)
        {
            addSource(n, source, volout, dt);
            source = source->next;
        }

    // --- compute a new chemical equilibrium at the source node

        MSXchem_equil(NODE, MSX.Node[n].c);
    }
}

//=============================================================================

void addSource(int n, Psource source, double volout, long dt)
/*
**  Purpose:
**    updates concentration of particular species leaving a node
**    that receives external source input.
**
**  Input:
**    n = index of source node
**    source = pointer to WQ source data
**    volout = volume of water leaving node during current time step
*/
{
    int     m;
    double  massadded, s;

// --- only analyze bulk species

    m = source->species;
    massadded = 0.0;
    if (source->c0 > 0.0 && MSX.Species[m].type == BULK)
    {

    // --- mass added depends on type of source

        s = getSourceQual(source);
        switch(source->type)
        {
        // Concen. Source:
        // Mass added = source concen. * -(demand)

          case CONCEN:

          // Only add source mass if demand is negative

              if (MSX.D[n] < 0.0) massadded = -s*MSX.D[n]*dt;

          // If node is a tank then set concen. to 0.
          // (It will be re-set to true value later on)

              if (MSX.Node[n].tank > 0) MSX.Node[n].c[m] = 0.0;
              break;

        // Mass Inflow Booster Source:

          case MASS:
              massadded = s*dt/LperFT3;
              break;

        // Setpoint Booster Source:
        // Mass added is difference between source
        // & node concen. times outflow volume

          case SETPOINT:
              if (s > MSX.Node[n].c[m])
                  massadded = (s - MSX.Node[n].c[m])*volout;
              break;

        // Flow-Paced Booster Source:
        // Mass added = source concen. times outflow volume

          case FLOWPACED:
              massadded = s*volout;
              break;
        }

    // --- adjust nodal concentration to reflect source addition

        MSX.Node[n].c[m] += massadded/volout;
    }
}

//=============================================================================

void release(long dt)
/*
**  Purpose:
**    releases outflow from nodes into incident links.
**
**  Input:
**    dt = current WQ time step
*/
{
    int    k, n, m;
    int    useNewSeg;
    double q, v;
    Pseg   seg;

// --- examine each link

    for (k=1; k<=MSX.Nobjects[LINK]; k++)
    {
    // --- ignore links with no flow

        if (MSX.Q[k] == 0.0)
        {
            MSXqual_removeSeg(NewSeg[k]);
            continue;
        }

    // --- find flow volume released to link from upstream node
    //     (NOTE: Flow volume is allowed to be > link volume.)

        n = UP_NODE(k);
        q = ABS(MSX.Q[k]);
        v = q*dt;

    // --- place bulk WQ at upstream node in new segment identified for link

        for (m=1; m<=MSX.Nobjects[SPECIES]; m++)
        {
            if ( MSX.Species[m].type == BULK )
                NewSeg[k]->c[m] = MSX.Node[n].c[m];
        }

    // --- if link has no last segment, then we must add a new one

        useNewSeg = 0;
        seg = MSX.LastSeg[k];
        if ( seg == NULL ) useNewSeg = 1;

    // --- otherwise check if quality in last segment
    //     differs from that of the new segment

        else if ( !MSXqual_isSame(seg->c, NewSeg[k]->c) ) useNewSeg = 1;

    // --- quality of last seg & new seg are close;
    //     simply increase volume of last seg

        if ( useNewSeg == 0 )
        {
            seg->v += v;
            MSXqual_removeSeg(NewSeg[k]);
        }

    // --- otherwise add the new seg to the end of the link

        else
        {
            NewSeg[k]->v = v;
            MSXqual_addSeg(k, NewSeg[k]);
        }

    }   //next link
}

//=============================================================================

double  getSourceQual(Psource source)
/*
**   Input:   j = source index
**   Output:  returns source WQ value
**   Purpose: determines source concentration in current time period
*/
{
    int    i;
    long   k;
    double c, f = 1.0;

// --- get source concentration (or mass flow) in original units
    c = source->c0;

// --- convert mass flow rate from min. to sec.
    if (source->type == MASS) c /= 60.0;

// --- apply time pattern if assigned
    i = source->pat;
    if (i == 0) return(c);
    k = ((MSX.Qtime + MSX.Pstart) / MSX.Pstep) % MSX.Pattern[i].length;
    if (k != MSX.Pattern[i].interval)
    {
        if ( k < MSX.Pattern[i].interval )
        {
            MSX.Pattern[i].current = MSX.Pattern[i].first;
            MSX.Pattern[i].interval = 0;
        }
        while (MSX.Pattern[i].current && MSX.Pattern[i].interval < k)
        {
             MSX.Pattern[i].current = MSX.Pattern[i].current->next;
             MSX.Pattern[i].interval++;
        }
    }
    if (MSX.Pattern[i].current) f = MSX.Pattern[i].current->value;
    return c*f;
}

//=============================================================================

void  removeAllSegs(int k)
/*
**   Purpose:
**     removes all segments in a pipe link.
**
**   Input:
**     k = link index.
*/
{
    Pseg seg;
    seg = MSX.FirstSeg[k];
    while (seg != NULL)
    {
        MSX.FirstSeg[k] = seg->prev;
        MSXqual_removeSeg(seg);
        seg = MSX.FirstSeg[k];
    }
    MSX.LastSeg[k] = NULL;
}

//=============================================================================

void MSXqual_removeSeg(Pseg seg)
/*
**   Purpose:
**     places a WQ segment back into the memory pool of segments.
**
**   Input:
**     seg = pointer to a WQ segment.
*/
{
    if ( seg == NULL ) return;
    seg->prev = FreeSeg;
    seg->next = NULL;
    FreeSeg = seg;
}

//=============================================================================

Pseg MSXqual_getFreeSeg(double v, double c[])
/*
**   Purpose:
**     retrieves an unused water quality volume segment from the memory pool.
**
**   Input:
**     v = segment volume (ft3)
**     c[] = segment quality
**
**   Returns:
**     a pointer to an unused water quality segment.
*/
{
    Pseg seg;
    int  m;

// --- try using the last discarded segment if one is available

    if (FreeSeg != NULL)
    {
        seg = FreeSeg;
        FreeSeg = seg->prev;
    }

// --- otherwise create a new segment from the memory pool

    else
    {
        seg = (struct Sseg *) Alloc(sizeof(struct Sseg));
        if (seg == NULL)
        {
            OutOfMemory = TRUE;
            return NULL;
        }
        seg->c = (double *) Alloc((MSX.Nobjects[SPECIES]+1)*sizeof(double));
        if ( seg->c == NULL )
        {
            OutOfMemory = TRUE;
            return NULL;
        }
    }

// --- assign volume, WQ, & integration time step to the new segment

    seg->v = v;
    for (m=1; m<=MSX.Nobjects[SPECIES]; m++) seg->c[m] = c[m];
    seg->hstep = 0.0;
    return seg;
}

//=============================================================================

void  MSXqual_addSeg(int k, Pseg seg)
/*
**   Purpose:
**     adds a new segment to the upstream end of a link.
**
**   Input:
**     k = link index
**     seg = pointer to a free WQ segment.
*/

{
    seg->prev = NULL;
    seg->next = NULL;
    if (MSX.FirstSeg[k] == NULL) MSX.FirstSeg[k] = seg;
    if (MSX.LastSeg[k] != NULL)
    {
        MSX.LastSeg[k]->prev = seg;
        seg->next = MSX.LastSeg[k];
    }
    MSX.LastSeg[k] = seg;
}
