/******************************************************************************
**  MODULE:        MSXTANK.C
**  PROJECT:       EPANET-MSX
**  DESCRIPTION:   Storage tank mixing routines. 
**  COPYRIGHT:     Copyright (C) 2007 Feng Shang, Lewis Rossman, and James Uber.
**                 All Rights Reserved. See license information in LICENSE.TXT.
**  AUTHORS:       L. Rossman, US EPA - NRMRL
**                 F. Shang, University of Cincinnati
**                 J. Uber, University of Cincinnati
**  VERSION:       1.1.00
**  LAST UPDATE:   7/31/07
******************************************************************************/
#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <math.h>

#include "msxtypes.h"

//  External variables
//--------------------
extern MSXproject  MSX;                // MSX project data

//  Imported functions
//--------------------
void  MSXqual_removeSeg(Pseg seg);
Pseg  MSXqual_getFreeSeg(double v, double c[]);
void  MSXqual_addSeg(int k, Pseg seg);
int   MSXqual_isSame(double c1[], double c2[]);
int   MSXchem_equil(int zone, double *c);

//  Exported functions
//--------------------
void   MSXtank_mix1(int i, double vIn, double cIn[], long dt);
void   MSXtank_mix2(int i, double vIn, double cIn[], long dt);
void   MSXtank_mix3(int i, double vIn, double cIn[], long dt);
void   MSXtank_mix4(int i, double vIn, double cIn[], long dt);


//=============================================================================

void  MSXtank_mix1(int i, double vIn, double cIn[], long dt)
/*
**  Purpose:
**    computes new WQ at end of time step in a completely mixed tank
**    (after contents have been reacted).
**                     
**  Input:
**    i   = tank index
**    vin = volume of inflow to tank (ft3)
**    cin = concen. of inflow to tank
**    dt  = current WQ time step     
*/
{
    int    k, m, n;
    double c;
    Pseg   seg;

// --- blend inflow with contents

    n = MSX.Tank[i].node;
    k = MSX.Nobjects[LINK] + i;
    seg = MSX.FirstSeg[k];
    for (m=1; m<=MSX.Nobjects[SPECIES]; m++)
    {
        if ( MSX.Species[m].type != BULK ) continue;
        c = seg->c[m];
        if (MSX.Tank[i].v > 0.0) c += (cIn[m] - c)*vIn/MSX.Tank[i].v;
        else c = cIn[m];
        c = MAX(0.0, c);
        seg->c[m] = c;
        MSX.Tank[i].c[m] = c;
    }

// --- update species equilibrium 

    if ( vIn > 0.0 ) MSXchem_equil(NODE, MSX.Tank[i].c);
}

//=============================================================================

void  MSXtank_mix2(int i, double vIn, double cIn[], long dt)
/*
**   Purpose: 2-compartment tank model                      
**
**   Input:   i = tank index
**            vIn = volume of inflow to tank (ft3)
**            cIn[] = concen. of inflow to tank
**            dt = current WQ time step (sec)
*/
{
    int     k, m, n;
    long    tstep,                     // Actual time step taken
            tstar;                     // Time to fill or drain a zone
    double  qIn,                       // Inflow rate
            qOut,                      // Outflow rate
            qNet;                      // Net flow rate
    double  c, c1, c2;                 // Species concentrations
    Pseg    seg1,                      // Mixing zone segment
            seg2;                      // Ambient zone segment

// --- find inflows & outflows 

    n = MSX.Tank[i].node;
    qNet = MSX.D[n];
    qIn = vIn/(double)dt;
    qOut = qIn - qNet;

// --- get segments for each zone

    k = MSX.Nobjects[LINK] + i;
    seg1 = MSX.FirstSeg[k];
    seg2 = MSX.LastSeg[k];

// --- case of no net volume change

    if ( fabs(qNet) < TINY ) return;

// --- case of net filling (qNet > 0)

    else if (qNet > 0.0)
    {

    // --- case where ambient zone empty & mixing zone filling 

        if (seg2->v <= 0.0)
        {
        // --- time to fill mixing zone
            tstar = (long) ((MSX.Tank[i].vMix - (seg1->v))/qNet);
            tstep = MIN(dt, tstar);

            for (m=1; m<=MSX.Nobjects[SPECIES]; m++)
            {
                if ( MSX.Species[m].type != BULK ) continue;

            // --- new quality in mixing zone
                c = seg1->c[m]; 
                if (seg1->v > 0.0) seg1->c[m] += qIn*tstep*(cIn[m]-c)/(seg1->v);
                else seg1->c[m] = cIn[m];
                seg1->c[m] = MAX(0.0, seg1->c[m]);
                seg2->c[m] = 0.0;
            }

        // --- new volume of mixing zone
            seg1->v += qNet*tstep;

        // --- time during which ambient zone fills
            dt -= tstep;
        }

    // --- case where mixing zone full & ambient zone filling 
        if (dt > 1)
        {
            for (m=1; m<=MSX.Nobjects[SPECIES]; m++)
            {
                if ( MSX.Species[m].type != BULK ) continue;

            // --- new quality in mixing zone
                c1 = seg1->c[m]; 
                seg1->c[m] += qIn * dt * (cIn[m] - c1) / (seg1->v);
                seg1->c[m] = MAX(0.0, seg1->c[m]);

            // --- new quality in ambient zone
		        c2 = seg2->c[m];
                if (seg2->v <= 0.0)
                    seg2->c[m] = seg1->c[m];
                else
                    seg2->c[m] += qNet * dt * ((seg1->c[m]) - c2) / (seg2->v);
                seg2->c[m] = MAX(0.0, seg2->c[m]);
	        }

        // --- new volume of ambient zone
            seg2->v += qNet*dt;
        }
        if ( seg1->v > 0.0 ) MSXchem_equil(NODE, seg1->c);
        if ( seg2->v > 0.0 ) MSXchem_equil(NODE, seg2->c);
    }

// --- case of net emptying (qnet < 0)

    else if ( qNet < 0.0 && seg1->v > 0.0 )
    {

    // --- case where mixing zone full & ambient zone draining

        if ((seg2->v) > 0.0)
        {

        // --- time to drain ambient zone
            tstar = (long)(seg2->v/-qNet);
            tstep = MIN(dt, tstar);

            for (m=1; m<=MSX.Nobjects[SPECIES]; m++)
            {
                if ( MSX.Species[m].type != BULK ) continue;
	            c1 = seg1->c[m];
	            c2 = seg2->c[m];

            // --- new mizing zone quality (affected by both external inflow
            //     and drainage from the ambient zone
                seg1->c[m] += (qIn*cIn[m] - qNet*c2 - qOut*c1)*tstep/(seg1->v);
                seg1->c[m] = MAX(0.0, seg1->c[m]);
	        }

        // --- new ambient zone volume
            seg2->v += qNet*tstep;
            seg2->v = MAX(0.0, seg2->v);

        // --- time during which mixing zone empties
            dt -= tstep;
        }

    // --- case where ambient zone empty & mixing zone draining

        if (dt > 1)
        {
            for (m=1; m<=MSX.Nobjects[SPECIES]; m++)
            {
                if ( MSX.Species[m].type != BULK ) continue;

            // --- new mixing zone quality (affected by external inflow only)
		        c = seg1->c[m];
                seg1->c[m] += qIn*dt*(cIn[m]-c)/(seg1->v);
                seg1->c[m] = MAX(0.0, seg1->c[m]);
                seg2->c[m] = 0.0;
	        }

        // --- new volume of mixing zone
            seg1->v += qNet*dt;
            seg1->v = MAX(0.0, seg1->v);
        }
        if ( seg1->v > 0.0 ) MSXchem_equil(NODE, seg1->c);
    }

// --- use quality of mixed compartment (seg1) to represent quality
//     of tank since this is where outflow begins to flow from

    for (m=1; m<=MSX.Nobjects[SPECIES]; m++) MSX.Tank[i].c[m] = seg1->c[m];
}

//=============================================================================

void  MSXtank_mix3(int i, double vIn, double cIn[], long dt)
/*
**   Purpose: computes concentrations in the segments that form a
**            first-in-first-out (FIFO) tank model.
**                    
**   Input:   i   = tank index
**            vIn = volume of inflow to tank (ft3)
**            cIn = concen. of inflow to tank
**            dt = current WQ time step     
*/
{
   int    k, m, n;
   double vNet, vOut, vSeg, vSum;
   Pseg   seg;

// --- find inflows & outflows

    k = MSX.Nobjects[LINK] + i;
    n = MSX.Tank[i].node;
    vNet = MSX.D[n]*dt;
    vOut = vIn - vNet;

// --- initialize outflow volume & concentration

    vSum = 0.0;
    for (m=1; m<=MSX.Nobjects[SPECIES]; m++) MSX.C1[m] = 0.0;

// --- withdraw flow from first segment

    while (vOut > 0.0)
    {
    // --- get volume of current first segment
        seg = MSX.FirstSeg[k];
        if (seg == NULL) break;
        vSeg = seg->v;
        vSeg = MIN(vSeg, vOut);
        if ( seg == MSX.LastSeg[k] ) vSeg = vOut;

    // --- update mass & volume removed
        vSum += vSeg;
        for (m=1; m<=MSX.Nobjects[SPECIES]; m++)
        {
            MSX.C1[m] += (seg->c[m])*vSeg;
        }

    // --- decrease vOut by volume of first segment
        vOut -= vSeg;

    // --- remove segment if all its volume is consumed
        if (vOut >= 0.0 && vSeg >= seg->v)
        {
            MSX.FirstSeg[k] = seg->prev;
            if ( MSX.FirstSeg[k] == NULL ) MSX.LastSeg[k] = NULL;
            MSXqual_removeSeg(seg);
        }

    // --- otherwise just adjust volume of first segment
        else  seg->v -= vSeg;
    }

// --- use quality from first segment to represent overall
//     quality of tank since this is where outflow flows from

    for (m=1; m<=MSX.Nobjects[SPECIES]; m++)
    {
        if (vSum > 0.0) MSX.Tank[i].c[m] = MSX.C1[m]/vSum;
        else            MSX.Tank[i].c[m] = MSX.FirstSeg[k]->c[m];
    }

// --- add new last segment for new flow entering tank

    if (vIn > 0.0)
    {

    // --- quality is the same, so just add flow volume to last seg
        k = MSX.Nobjects[LINK] + i;
        seg = MSX.LastSeg[k];
        if ( seg != NULL && MSXqual_isSame(seg->c, cIn) ) seg->v += vIn;

     // --- Otherwise add a new seg to tank
        else 
        {
            seg = MSXqual_getFreeSeg(vIn, cIn);
            MSXqual_addSeg(k, seg);
        }
    }
}

//=============================================================================

void  MSXtank_mix4(int i, double vIn, double cIn[], long dt)
/*
**----------------------------------------------------------
**   Input:   i = tank index
**            vIn = volume of inflow to tank (ft3)
**            cIn = concen. of inflow to tank
**            dt = current WQ time step     
**   Output:  none
**   Purpose: Last In-First Out (LIFO) tank model                     
**----------------------------------------------------------
*/
{
   int    k, m, n;
   double vOut, vNet, vSum, vSeg;
   Pseg   seg;

// --- find inflows & outflows

    k = MSX.Nobjects[LINK] + i;
    n = MSX.Tank[i].node;
    vNet = MSX.D[n]*dt;
    vOut = vIn - vNet;

// --- keep track of total volume & mass removed from tank

    vSum = 0.0;
    for (m=1; m<=MSX.Nobjects[SPECIES]; m++) MSX.C1[m] = 0.0;

// --- if tank filling, then create a new last segment

    if ( vNet > 0.0 )
    {

    // --- inflow quality = last segment quality so just expand last segment

        seg = MSX.LastSeg[k];
        if ( seg != NULL && MSXqual_isSame(seg->c, cIn) ) seg->v += vNet;

    // --- otherwise add a new last segment to tank

        else
        {
            seg = MSXqual_getFreeSeg(vNet, cIn);
            MSXqual_addSeg(k, seg);
        }

    // --- quality of tank is that of inflow

        for (m=1; m<=MSX.Nobjects[SPECIES]; m++) MSX.Tank[i].c[m] = cIn[m];

    }

// --- if tank emptying then remove last segments until vNet consumed

    else if (vNet < 0.0)
    {

    // --- keep removing volume from last segments until vNet is removed
        vNet = -vNet;
        while (vNet > 0.0)
        {

        // --- get volume of current last segment
            seg = MSX.LastSeg[k];
            if ( seg == NULL ) break;
            vSeg = seg->v;
            vSeg = MIN(vSeg, vNet);
            if ( seg == MSX.FirstSeg[k] ) vSeg = vNet;

        // --- update mass & volume removed
            vSum += vSeg;
            for (m=1; m<=MSX.Nobjects[SPECIES]; m++)
                MSX.C1[m] += (seg->c[m])*vSeg;

        // --- reduce vNet by volume of last segment
            vNet -= vSeg;

        // --- remove segment if all its volume is used up
            if (vNet >= 0.0 && vSeg >= seg->v)
            {
                MSX.LastSeg[k] = seg->prev;
                if ( MSX.LastSeg[k] == NULL ) MSX.FirstSeg[k] = NULL;
                MSXqual_removeSeg(seg);
            }

        // --- otherwise just reduce volume of last segment
            else
            {
                seg->v -= vSeg;
            }
        }

    // --- tank quality is mixture of flow released and any inflow

        for (m=1; m<=MSX.Nobjects[SPECIES]; m++)
        {
            vSum = vSum + vIn;
            if (vSum > 0.0)
                MSX.Tank[i].c[m] = (MSX.C1[m] + cIn[m]*vIn) / vSum;
        }
    }
}         
