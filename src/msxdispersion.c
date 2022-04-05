/******************************************************************************
**  MODULE:        MSXDISPERSION.C
**  PROJECT:       EPANET-MSX
**  DESCRIPTION:   Dispersion solver for Lagrangian ADR
******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "msxtypes.h"
#include "dispersion.h"

//  External variables
//--------------------
extern MSXproject  MSX;                // MSX project data

void dispersion_pipe(int m, long tstep)
{

	double cons;
	double ldispersion;
	double flowrate, velocity, area;
	int nseg;
	double diam;
	double vd, vu, vself, asquare, dh, frictionfactor;
	double reynolds=0, shearvelocity=0;
	//	double aaa, ccc;
	Pseg seg;

	double elpt;
	double d0 = 1.292e-8;   //molecular diffusivity 1.292e-8 ft^2/s   1.2e-9 m^2/s
	
	d0 = MSX.Dispersion.md[m];

	for (int k = 1; k <= MSX.Nobjects[LINK]; k++)
	{
		velocity = 0;
		if (MSX.FirstSeg[k] == NULL)
			continue;
		diam = MSX.Link[k].diam;
		// Compute Reynolds No.
		flowrate = (MSX.S[k] <= CLOSED) ? 0.0 : MSX.Q[k];
		area = PI * diam * diam / 4.0;         // pipe area
		if (area > 0.0 && MSX.Link[k].len > 0.0 && ABS(flowrate) > 0.0)
		{
			velocity = fabs(flowrate) / area;              // flow velocity
			reynolds = velocity * diam / MSX.Dispersion.viscosity;     // Reynolds number  //#define   VISCOS    1.1E-5     // Kinematic viscosity of water // @ 20 deg C (sq ft/sec)
			dh = ABS(MSX.H[MSX.Link[k].n1] - MSX.H[MSX.Link[k].n2]);
			if (dh > 0.00001)
				frictionfactor = 39.725 * dh * pow(diam, 5) / (MSX.Link[k].len * SQR(flowrate));
			else
				frictionfactor = 0.0;
			shearvelocity = velocity * sqrt(frictionfactor / 8);

			if (d0 < 0)
			{
				ldispersion = MSX.Dispersion.ld[m];
			}
			else if (reynolds > 2000) //Basha 2007
			{
				ldispersion = 0.5 * diam * shearvelocity * (10.1 + 577 * pow(reynolds / 1000.0, -2.2));

			}
			else   //Basha 2007
			{
				ldispersion = SQR(0.5 * diam * velocity) / (48 * d0);
				elpt = MSX.Link[k].len / velocity;
				ldispersion = ldispersion * (1 - exp(-12.425 * d0 * elpt / SQR(0.5 * diam)));
				ldispersion += d0;
				//		ldispersion = pow(velocity * diam / 2.0, 2) / (48 * d0);//Taylor
			}
		//	ldispersion = 147.25/10.0;
		}
		else
		{
			ldispersion = 0;
		}

		if (ldispersion < 0.0)
		{
			ldispersion = 0;
			MSX.Dispersion.pipeDispersionCoeff[k] = 0;
			continue;
		}

		double domi;

		if (velocity > 0)
			domi = ldispersion / (velocity * velocity * tstep);
		else
			domi = 1000;
		if (domi >= 0.000 && ldispersion >= 1E-4)
		{
			MSX.Dispersion.pipeDispersionCoeff[k] = ldispersion;
		}
		else
		{
			MSX.Dispersion.pipeDispersionCoeff[k] = 0;
			continue;
		}
		asquare = area*area;
		seg = MSX.FirstSeg[k];   //downstream
		cons = 2.0 * ldispersion * asquare * tstep;
		vd = 0.0;
		MSX.Dispersion.bl[0] = 1.0;
		MSX.Dispersion.cl[0] = 0.0;
		MSX.Dispersion.rl[0] = 0.0;
		nseg = 0;

		while (seg != NULL)
		{
			nseg++;
			vself = seg->v;
			MSX.Dispersion.rl[nseg] = seg->c[m];
			seg = seg->prev;
			if (seg)
				vu = seg->v;
			else
				vu = 0.0;
			MSX.Dispersion.al[nseg] = -cons / (vself * vself + vself * vd);
			MSX.Dispersion.cl[nseg] = -cons / (vself * vself + vself * vu);
			MSX.Dispersion.bl[nseg] = 1 - MSX.Dispersion.al[nseg] - MSX.Dispersion.cl[nseg];

			vd = vself;
		}
		if (nseg == 0)
			continue;
		if (nseg > MAXSEGMENTS)
			printf("too many segs\n");

		MSX.Dispersion.al[nseg + 1] = 0.0;
		MSX.Dispersion.bl[nseg + 1] = 1.0;
		MSX.Dispersion.rl[nseg + 1] = 0.0;

		if (reynolds >= 0)
			tridiag(nseg + 2, MSX.Dispersion.al, MSX.Dispersion.bl, MSX.Dispersion.cl, MSX.Dispersion.rl, MSX.Dispersion.sol); //nseg+2 <= 1000 here 
		else
		{
			for (int p = 0; p <= nseg + 1; p++)
				MSX.Dispersion.sol[p] = MSX.Dispersion.rl[p];
		}

		seg = MSX.FirstSeg[k];   //downstream segment
		int segindex = 1;
		while (seg != NULL)
		{
			seg->hresponse = MSX.Dispersion.sol[segindex];
			seg = seg->prev;
			segindex++;
		}
		/*clear initial condition*/
		for (int p = 1; p < nseg + 1; p++)
			MSX.Dispersion.rl[p] = 0.0;

		/*downstream unit boundary condition*/
		MSX.Dispersion.rl[0] = 1.0;
		if (reynolds >= 0)
			tridiag(nseg + 2, MSX.Dispersion.al, MSX.Dispersion.bl, MSX.Dispersion.cl, MSX.Dispersion.rl, MSX.Dispersion.sol); 
		else
		{
			for (int p = 0; p <= nseg + 1; p++)
				MSX.Dispersion.sol[p] = MSX.Dispersion.rl[p];
		}

		seg = MSX.FirstSeg[k];   //downstream
		segindex = 1;
		while (seg != NULL)
		{
			seg->dresponse = MSX.Dispersion.sol[segindex];
			seg = seg->prev;
			segindex++;
		}

		/*upstream unit boundary condition*/
		MSX.Dispersion.rl[0] = 0.0;
		MSX.Dispersion.rl[nseg + 1] = 1.0;
		if (reynolds >= 0)
			tridiag(nseg + 2, MSX.Dispersion.al, MSX.Dispersion.bl, MSX.Dispersion.cl, MSX.Dispersion.rl, MSX.Dispersion.sol); //nseg+2 <= 100 here 
		else
		{
			for (int p = 0; p <= nseg + 1; p++)
				MSX.Dispersion.sol[p] = MSX.Dispersion.rl[p];
		}

		seg = MSX.FirstSeg[k];   //downstream
		segindex = 1;
		while (seg != NULL)
		{
			seg->uresponse = MSX.Dispersion.sol[segindex];
			seg = seg->prev;
			segindex++;
		}
	}
}

void solve_nodequal(int m, long tstep)
{

	Pseg firstseg;
	Pseg lastseg;
	Psource source;
	int n1, n2;
	double diam, area, asquare;
	double vsegdown, vsegup, newqual, diffqual;
	//	double dispersion =  147.25;
	double ldispersion;
	double coelastseg, coefirstseg;
	double nodequaldiff;
	int njuncs = MSX.Nobjects[NODE] - MSX.Nobjects[TANK];
	int found = 0;


	//let's take a look of the matrix

	memset(MSX.Dispersion.Aii, 0, (MSX.Nobjects[NODE] + 1) * sizeof(double));
	memset(MSX.Dispersion.Aij, 0, (MSX.Dispersion.Ncoeffs + 1) * sizeof(double));
	memset(MSX.Dispersion.F, 0, (MSX.Nobjects[NODE] + 1) * sizeof(double));
	for (int k = 1; k <= MSX.Nobjects[LINK]; k++)
	{
		ldispersion = MSX.Dispersion.pipeDispersionCoeff[k];
		if (ldispersion <= 0)
			continue;
		n1 = MSX.Link[k].n1;  //upstream
		n2 = MSX.Link[k].n2;  //downstream
		if (MSX.FlowDir[k] < 0)
		{
			n1 = MSX.Link[k].n2;  //upstream
			n2 = MSX.Link[k].n1;  //downstream
		}
		diam = MSX.Link[k].diam;
		area = 0.25 * PI * diam * diam;
		asquare = area * area;
		firstseg = MSX.FirstSeg[k];   //downstream
		lastseg =  MSX.LastSeg[k];		//upstream	


		if (firstseg == NULL)
			continue;

		coefirstseg = ldispersion * asquare / firstseg->v;   //dispersion should be pipe by pipe
		coelastseg = ldispersion * asquare / lastseg->v;   //dispersion should be pipe by pipe
		MSX.Dispersion.Aij[MSX.Dispersion.Ndx[k]] -= coefirstseg * firstseg->uresponse; //coefirstseg*firstseg->greenu = coelastseg*lastseg->greend 

		found = 0;
		source = MSX.Node[n2].sources;
		while(source != NULL)
		{
			if (source->species == m)
			{
				found = 1;
				break;
			}
			else
				source = source->next;
		}
		
		if (n2 <= njuncs)
		{
			if (source == NULL || source->c0 <= 0.0)
			{
				MSX.Dispersion.Aii[MSX.Dispersion.Row[n2]] += coefirstseg * (1.0 - firstseg->dresponse);

				MSX.Dispersion.F[MSX.Dispersion.Row[n2]] += coefirstseg * firstseg->hresponse;
			}
			else
			{
				MSX.Dispersion.Aij[MSX.Dispersion.Ndx[k]] = 0;
				MSX.Dispersion.F[MSX.Dispersion.Row[n1]] += coelastseg * MSX.LastSeg[k]->dresponse * MSX.Node[n2].c[m];
			}
		}
		else
		{
			MSX.Dispersion.F[MSX.Dispersion.Row[n1]] += coelastseg * MSX.LastSeg[k]->dresponse * MSX.Node[n2].c[m];
		}

			
		found = 0;
		source = MSX.Node[n1].sources;
		while (source != NULL)
		{
			if (source->species == m)
			{
				found = 1;
				break;
			}
			else
				source = source->next;
		}

		if (n1 <= njuncs)
		{
			source = MSX.Node[n1].sources;
			if (source == NULL || source->c0 <= 0.0)
			{
				MSX.Dispersion.Aii[MSX.Dispersion.Row[n1]] += coelastseg * (1.0 - lastseg->uresponse);
				if (isnan(MSX.Dispersion.Aii[MSX.Dispersion.Row[n1]]))
					printf("NAN\n");
				MSX.Dispersion.F[MSX.Dispersion.Row[n1]] += coelastseg * lastseg->hresponse;
			}
			else
			{
				MSX.Dispersion.Aij[MSX.Dispersion.Ndx[k]] = 0;   //sure
				MSX.Dispersion.F[MSX.Dispersion.Row[n2]] += coefirstseg * firstseg->uresponse * MSX.Node[n1].c[m];
			}

		}
		else
		{
			MSX.Dispersion.F[MSX.Dispersion.Row[n2]] += coefirstseg * firstseg->uresponse * MSX.Node[n1].c[m];
		}
	}
	for (int i = 1; i <= njuncs; i++)
	{
		if (MSX.Dispersion.Aii[MSX.Dispersion.Row[i]] == 0.0)		//no dispersion around the junction at all
		{
			MSX.Dispersion.Aii[MSX.Dispersion.Row[i]] = 1.0;

			MSX.Dispersion.F[MSX.Dispersion.Row[i]] = 1.0 * MSX.Node[i].c[m];
		}
	}

	int errcode = linsolve(njuncs, MSX.Dispersion.Aii, MSX.Dispersion.Aij, MSX.Dispersion.F);

	for (int i = 1; i <= njuncs; i++)
	{
		MSX.Node[i].c[m] = MSX.Dispersion.F[MSX.Dispersion.Row[i]];

	}

}


void   segqual_update(int m, long tstep)
{

	Pseg seg;
	Psource source;
	int n1, n2;
	double mass1 = 0;
	double mass2 = 0;
	double dispersedin = 0;
	double ldispersion;
	double area;
	int njuncs = MSX.Nobjects[NODE] - MSX.Nobjects[TANK];
	for (int k = 1; k <= MSX.Nobjects[LINK]; k++)
	{
		ldispersion = MSX.Dispersion.pipeDispersionCoeff[k];
		if (ldispersion <= 0.0)
			continue;
		area = 0.25 * PI * MSX.Link[k].diam * MSX.Link[k].diam;
		n1 = MSX.Link[k].n1;
		n2 = MSX.Link[k].n2;
		if (MSX.FlowDir[k] < 0.0)
		{
			n1 = MSX.Link[k].n2;
			n2 = MSX.Link[k].n1;
		}
		seg = MSX.FirstSeg[k];
		while (seg != NULL)   //update segment concentration based on new up/down node quality
		{
			mass1 += seg->c[m] * seg->v;
			seg->c[m] = seg->hresponse + MSX.Node[n2].c[m] * seg->dresponse + MSX.Node[n1].c[m] * seg->uresponse;

			mass2 += seg->c[m] * seg->v;
			seg = seg->prev;
		}
		if (MSX.FirstSeg[k] == NULL)
			continue;

		source = MSX.Node[n2].sources;
		while (source != NULL)
		{
			if (source->species == m)
				break;
			source = source->next;
		}

		if (source != NULL && source->c0 > 0)
		{
			dispersedin += 2.0 * ldispersion * tstep * area*area * (MSX.Node[n2].c[m] - MSX.FirstSeg[k]->c[m]) / MSX.FirstSeg[k]->v;
		}
		else if (n2 > njuncs)  //we assume constant tank and reservoir concentration during dispersion which may not be true for small tanks
		{
			dispersedin += 2.0 * ldispersion * tstep *area*area * (MSX.Node[n2].c[m] - MSX.FirstSeg[k]->c[m]) / MSX.FirstSeg[k]->v;
		}

		source = MSX.Node[n1].sources;
		while (source != NULL)
		{
			if (source->species == m)
				break;
			source = source->next;
		}
		if (source != NULL && source->c0 > 0)
		{
			dispersedin += 2.0 * ldispersion * tstep * area * area * (MSX.Node[n1].c[m] - MSX.LastSeg[k]->c[m]) / MSX.LastSeg[k]->v;
		}
		else if (n1 > njuncs)
		{
			dispersedin += 2.0 * ldispersion * tstep * area * area * (MSX.Node[n1].c[m] - MSX.LastSeg[k]->c[m]) / MSX.LastSeg[k]->v;
		}
	}
	MSX.MassBalance.indisperse[m] += dispersedin* LperFT3;

}








// Solve tri-daigonal system of eqns. using Thomas' algorithm

void tridiag(int n, double *a, double *b, double *c, double *r, double *y)
{
	int j;
	double bet;

	bet = b[0];
	y[0] = r[0] / bet;
	for (j = 1; j < n; j++)
	{
		MSX.Dispersion.gam[j] = c[j - 1] / bet;
		bet = b[j] - a[j] * MSX.Dispersion.gam[j];
		y[j] = (r[j] - a[j] * y[j - 1]) / bet;
	}
	for (j = n - 2; j >= 0; j--)
	{
		y[j] -= MSX.Dispersion.gam[j + 1] * y[j + 1];
	}
}
