/** \file dmatrix_utils.cpp
  * \brief Contient les définitions des fonctions et macros utiles pour DMatrix.
  */

#include "dmatrix_utils.h"
#include "dvect.h"
#include "lvect.h"
#include "dmatrix.h"

#include <cmath>

/// Macro effectuant une rotation pour la fonction jacobi
#define ROTATE(a,i,j,k,l) \
	{ \
		g = (*a)[i][j]; \
		h = (*a)[k][l]; \
		(*a)[i][j] = g - s * (h + g * tau); \
		(*a)[k][l] = h + s * (g - h * tau); \
	}

bool jacobi(DMatrix* a, DVect* d, DMatrix* v, int *nrot)
{
	int j, ip, iq, i;
	double tresh, theta, tau, t, sm, s, h, g, c;
	DVect b;
	DVect z;
	
	// On initialise v à la matrice identité
	for(ip = 0; ip < 3; ip++)
	{
		for(iq = 0; iq < 3; iq++)
			(*v)[ip][iq] = 0.0;
		(*v)[ip][ip] = 1.0;
	}
	
	// On initialise b et d avec la diagonale de a
	for(ip = 0; ip < 3; ip++)
	{
		b[ip] = (*d)[ip] = (*a)[ip][ip];
		z[ip] = 0.0;
	}
	
	*nrot = 0;
	for( i = 0; i < 50; i++)
	{
		// Somme des élements hors-diagonale
		sm = 0.0;
		for(ip=0; ip<2; ip++)
			for(iq=ip+1; iq<3; iq++)
				sm += fabs((*a)[ip][iq]);
		
		// Fin normale de la fonction, qui suppose une convergence
		// quadratique vers l'underflow
		if( sm == 0.0 )	// TODO: A corriger si l'underflow ne renvoie pas 0.0
			return true;
		
		if(i < 3)	// Pour les 3 premières itérations ...
			tresh = 0.2 * sm / 9.0;
		else		// ... Pour les autres
			tresh = 0.0;
		
		for(ip=0; ip<2; ip++)
		{
			for(iq=ip+1; iq<3; iq++)
			{
				g = 100.0 * fabs((*a)[ip][iq]);
				// Après 4 itérations, on passe la rotation si l'élement hors-diagonale est faible
				if(i > 3 && (double)(fabs((*d)[ip])+g) == (double)fabs((*d)[ip])
						&& (double)(fabs((*d)[iq])+g) == (double)fabs((*d)[iq]))
					(*a)[ip][iq] = 0.0;
				else if (fabs((*a)[ip][iq]) > tresh)
				{
					h=(*d)[iq]-(*d)[ip];
					if((double)(fabs(h)+g) == (double)(fabs(h)))
						t = ((*a)[ip][iq]) / h; //  t = 1/(2*theta)
					else
					{
						theta = 0.5 * h / ((*a)[ip][iq]);
						t = 1.0/(fabs(theta)+sqrt(1.0+theta*theta));
						if(theta < 0.0)
							t = -t;
					}
					c = 1.0 / sqrt(1+t*t);
					s = t*c;
					tau = s/(1.0+c);
					h = t * (*a)[ip][iq];
					z[ip] -= h;
					z[iq] += h;
					(*d)[ip] -= h;
					(*d)[iq] += h;
					(*a)[ip][iq] = 0.0;
					
					for(j = 0; j <= ip-1; j++)
						ROTATE(a, j, ip, j, iq)
						
					for(j = ip+1; j<=iq-1; j++)
						ROTATE(a, ip, j, j, iq)
					
					for(j = iq+1; j<3; j++)
						ROTATE(a, ip, j, iq, j)
					
					for(j = 0; j < 3; j++)
						ROTATE(v, j, ip, j, iq)
					
					++(*nrot);
				}
			}
		}
		for(ip = 0; ip < 3; ip++)
		{
			b[ip] += z[ip];
			(*d)[ip] = b[ip];
			z[ip] = 0.0;
		}
	}
	return false;
}

// Calcule le cofacteur correspondant à la position donnée dans la
// matrice donnée
double cofacteur(DMatrix* mat, short i, short j)
{
	double sg = ((i+j)%2) ? -1.0 : 1.0;
	double fac1 = 1.0, fac2 = 1.0;
	
	short k, l, cnt=0;
	for(k=0; k<3; k++)
		for(l=0; l<3; l++)
			if( (k != i) && (l != j) )
			{
				switch(cnt)
				{
				case 0:
				case 3:
					fac1 *= (*mat)[k][l];
					break;
				case 1:
				case 2:
					fac2 *= (*mat)[k][l];
					break;
				}
				cnt++;
			}
	return sg * (fac1 - fac2);
}
