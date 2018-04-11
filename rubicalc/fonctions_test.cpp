#include <cstdlib>
#include <ctime>
#include <cmath>
#include "dvect.h"
#include "lvect.h"
#include <iostream>

using namespace std;
#define TAILLE_IMAGE 10

double NG_Ref[TAILLE_IMAGE][TAILLE_IMAGE][TAILLE_IMAGE];
double NG_Def[TAILLE_IMAGE][TAILLE_IMAGE][TAILLE_IMAGE];

//	Fonctions d'interpolation
double P0XX (double X) { return ((-1.0/3.0)*pow(X,3.0) + (4.0/5.0)*pow(X,2.0) - (7.0/15.0)*X); }
double P1XX (double X) { return (pow(X,3.0) - (9.0/5.0)*pow(X,2.0) - (1.0/5.0)*X + 1.0); }
double P2XX (double X) { return (-pow(X,3.0) + (6.0/5.0)*pow(X,2.0) + (4.0/5.0)*X) ; }
double P3XX (double X) { return ((1.0/3.0)*pow(X,3.0) - (1.0/5.0)*pow(X,2.0) - (2.0/15.0)*X) ; }

double P0YY (double Y) { return ((-1.0/3.0)*pow(Y,3.0) + (4.0/5.0)*pow(Y,2.0) - (7.0/15.0)*Y); }
double P1YY (double Y) { return (pow(Y,3.0) - (9.0/5.0)*pow(Y,2.0) - (1.0/5.0)*Y + 1.0); }
double P2YY (double Y) { return (-pow(Y,3.0) + (6.0/5.0)*pow(Y,2.0) + (4.0/5.0)*Y) ; }
double P3YY (double Y) { return ((1.0/3.0)*pow(Y,3.0) - (1.0/5.0)*pow(Y,2.0) - (2.0/15.0)*Y); }

double P0ZZ (double Z) { return ((-1.0/3.0)*pow(Z,3.0) + (4.0/5.0)*pow(Z,2.0) - (7.0/15.0)*Z); }
double P1ZZ (double Z) { return (pow(Z,3.0) - (9.0/5.0)*pow(Z,2.0) - (1.0/5.0)*Z + 1.0); }
double P2ZZ (double Z) { return (-pow(Z,3.0) + (6.0/5.0)*pow(Z,2.0) + (4.0/5.0)*Z) ; }
double P3ZZ (double Z) { return ((1.0/3.0)*pow(Z,3.0) - (1.0/5.0)*pow(Z,2.0) - (2.0/15.0)*Z) ; }


double derX_P0XX (double X) { return (-pow(X,2.0) + (8.0/5.0)*X - 7.0/15.0) ; }		// dP0XX / dX
double derX_P1XX (double X) { return (3.0*pow(X,2.0) - (18.0/5.0)*X - 1.0/5.0) ; }	// dP1XX / dX
double derX_P2XX (double X) { return (-3.0*pow(X,2.0) + (12.0/5.0)*X + 4.0/5.0) ; }	// dP2XX / dX
double derX_P3XX (double X) { return (pow(X,2.0) - (2.0/5.0)*X - 2.0/15.0) ; }		// dP3XX / dX

double derY_P0YY (double Y) { return (-pow(Y,2.0) + (8.0/5.0)*Y - 7.0/15.0) ; }		// dP0YY / dY
double derY_P1YY (double Y) { return (3.0*pow(Y,2.0) - (18.0/5.0)*Y - 1.0/5.0) ; }	// dP1YY / dY
double derY_P2YY (double Y) { return (-3.0*pow(Y,2.0) + (12.0/5.0)*Y + 4.0/5.0) ; }	// dP2YY / dY
double derY_P3YY (double Y) { return (pow(Y,2.0) - (2.0/5.0)*Y - 2.0/15.0) ; }		// dP3YY / dY

double derZ_P0ZZ (double Z) { return (-pow(Z,2.0) + (8.0/5.0)*Z - 7.0/15.0) ; }		// dP0ZZ / dZ
double derZ_P1ZZ (double Z) { return (3.0*pow(Z,2.0) - (18.0/5.0)*Z - 1.0/5.0) ; }	// dP1ZZ / dZ
double derZ_P2ZZ (double Z) { return (-3.0*pow(Z,2.0) + (12.0/5.0)*Z + 4.0/5.0) ; }	// dP2ZZ / dZ
double derZ_P3ZZ (double Z) { return (pow(Z,2.0) - (2.0/5.0)*Z - 2.0/15.0) ; }		// dP3ZZ / dZ

// dPixS / dP0XX
double derP0XX_PixS(double Y, double Z, long i, long j, long k)
{
	double der = P0ZZ(Z)*(P0YY(Y)*NG_Def[i-1][j-1][k-1] + P1YY(Y)*NG_Def[i-1][j][k-1]+ P2YY(Y)*NG_Def[i-1][j+1][k-1]+ P3YY(Y)*NG_Def[i-1][j+2][k-1])
	+ P1ZZ(Z)*(P0YY(Y)*NG_Def[i-1][j-1][k] + P1YY(Y)*NG_Def[i-1][j][k]+ P2YY(Y)*NG_Def[i-1][j+1][k]+ P3YY(Y)*NG_Def[i-1][j+2][k])
	+ P2ZZ(Z)*(P0YY(Y)*NG_Def[i-1][j-1][k+1] + P1YY(Y)*NG_Def[i-1][j][k+1]+ P2YY(Y)*NG_Def[i-1][j+1][k+1]+ P3YY(Y)*NG_Def[i-1][j+2][k+1])
	+ P3ZZ(Z)*(P0YY(Y)*NG_Def[i-1][j-1][k+2] + P1YY(Y)*NG_Def[i-1][j][k+2]+ P2YY(Y)*NG_Def[i-1][j+1][k+2]+ P3YY(Y)*NG_Def[i-1][j+2][k+2]);
	
	return der;
}

// dPixS / dP1XX
double derP1XX_PixS(double Y, double Z, long i, long j, long k)
{
	double der = P0ZZ(Z)*(P0YY(Y)*NG_Def[i][j-1][k-1] + P1YY(Y)*NG_Def[i][j][k-1]+ P2YY(Y)*NG_Def[i][j+1][k-1]+ P3YY(Y)*NG_Def[i][j+2][k-1])
	+ P1ZZ(Z)*(P0YY(Y)*NG_Def[i][j-1][k] + P1YY(Y)*NG_Def[i][j][k]+ P2YY(Y)*NG_Def[i][j+1][k]+ P3YY(Y)*NG_Def[i][j+2][k])
	+ P2ZZ(Z)*(P0YY(Y)*NG_Def[i][j-1][k+1] + P1YY(Y)*NG_Def[i][j][k+1]+ P2YY(Y)*NG_Def[i][j+1][k+1]+ P3YY(Y)*NG_Def[i][j+2][k+1])
	+ P3ZZ(Z)*(P0YY(Y)*NG_Def[i][j-1][k+2] + P1YY(Y)*NG_Def[i][j][k+2]+ P2YY(Y)*NG_Def[i][j+1][k+2]+ P3YY(Y)*NG_Def[i][j+2][k+2]);
	
	return der;
}

// dPixS / dP2XX
double derP2XX_PixS(double Y, double Z, long i, long j, long k)
{
	double der = P0ZZ(Z)*(P0YY(Y)*NG_Def[i+1][j-1][k-1] + P1YY(Y)*NG_Def[i+1][j][k-1]+ P2YY(Y)*NG_Def[i+1][j+1][k-1]+ P3YY(Y)*NG_Def[i+1][j+2][k-1])
	+ P1ZZ(Z)*(P0YY(Y)*NG_Def[i+1][j-1][k] + P1YY(Y)*NG_Def[i+1][j][k]+ P2YY(Y)*NG_Def[i+1][j+1][k]+ P3YY(Y)*NG_Def[i+1][j+2][k])
	+ P2ZZ(Z)*(P0YY(Y)*NG_Def[i+1][j-1][k+1] + P1YY(Y)*NG_Def[i+1][j][k+1]+ P2YY(Y)*NG_Def[i+1][j+1][k+1]+ P3YY(Y)*NG_Def[i+1][j+2][k+1])
	+ P3ZZ(Z)*(P0YY(Y)*NG_Def[i+1][j-1][k+2] + P1YY(Y)*NG_Def[i+1][j][k+2]+ P2YY(Y)*NG_Def[i+1][j+1][k+2]+ P3YY(Y)*NG_Def[i+1][j+2][k+2]);
	
	return der;
}

// dPixS / dP3XX
double derP3XX_PixS(double Y, double Z, long i, long j, long k)
{
	double der = P0ZZ(Z)*(P0YY(Y)*NG_Def[i+2][j-1][k-1] + P1YY(Y)*NG_Def[i+2][j][k-1]+ P2YY(Y)*NG_Def[i+2][j+1][k-1]+ P3YY(Y)*NG_Def[i+2][j+2][k-1])
	+ P1ZZ(Z)*(P0YY(Y)*NG_Def[i+2][j-1][k] + P1YY(Y)*NG_Def[i+2][j][k]+ P2YY(Y)*NG_Def[i+2][j+1][k]+ P3YY(Y)*NG_Def[i+2][j+2][k])
	+ P2ZZ(Z)*(P0YY(Y)*NG_Def[i+2][j-1][k+1] + P1YY(Y)*NG_Def[i+2][j][k+1]+ P2YY(Y)*NG_Def[i+2][j+1][k+1]+ P3YY(Y)*NG_Def[i+2][j+2][k+1])
	+ P3ZZ(Z)*(P0YY(Y)*NG_Def[i+2][j-1][k+2] + P1YY(Y)*NG_Def[i+2][j][k+2]+ P2YY(Y)*NG_Def[i+2][j+1][k+2]+ P3YY(Y)*NG_Def[i+2][j+2][k+2]);
	
	return der;
}

// dPixS / dP0YY
double derP0YY_PixS(double X, double Z, long i, long j, long k)
{
	double der = P0ZZ(Z)*(P0XX(X)*NG_Def[i-1][j-1][k-1] + P1XX(X)*NG_Def[i][j-1][k-1]+ P2XX(X)*NG_Def[i+1][j-1][k-1]+ P3XX(X)*NG_Def[i+2][j-1][k-1])
	+ P1ZZ(Z)*(P0XX(X)*NG_Def[i-1][j-1][k] + P1XX(X)*NG_Def[i][j-1][k]+ P2XX(X)*NG_Def[i+1][j-1][k]+ P3XX(X)*NG_Def[i+2][j-1][k])
	+ P2ZZ(Z)*(P0XX(X)*NG_Def[i-1][j-1][k+1] + P1XX(X)*NG_Def[i][j-1][k+1]+ P2XX(X)*NG_Def[i+1][j-1][k+1]+ P3XX(X)*NG_Def[i+2][j-1][k+1])
	+ P3ZZ(Z)*(P0XX(X)*NG_Def[i-1][j-1][k+2] + P1XX(X)*NG_Def[i][j-1][k+2]+ P2XX(X)*NG_Def[i+1][j-1][k+2]+ P3XX(X)*NG_Def[i+2][j-1][k+2]);
	
	return der;
}

// dPixS / dP1YY
double derP1YY_PixS(double X, double Z, long i, long j, long k)
{
	double der = P0ZZ(Z)*(P0XX(X)*NG_Def[i-1][j][k-1] + P1XX(X)*NG_Def[i][j][k-1]+ P2XX(X)*NG_Def[i+1][j][k-1]+ P3XX(X)*NG_Def[i+2][j][k-1])
	+ P1ZZ(Z)*(P0XX(X)*NG_Def[i-1][j][k] + P1XX(X)*NG_Def[i][j][k]+ P2XX(X)*NG_Def[i+1][j][k]+ P3XX(X)*NG_Def[i+2][j][k])
	+ P2ZZ(Z)*(P0XX(X)*NG_Def[i-1][j][k+1] + P1XX(X)*NG_Def[i][j][k+1]+ P2XX(X)*NG_Def[i+1][j][k+1]+ P3XX(X)*NG_Def[i+2][j][k+1])
	+ P3ZZ(Z)*(P0XX(X)*NG_Def[i-1][j][k+2] + P1XX(X)*NG_Def[i][j][k+2]+ P2XX(X)*NG_Def[i+1][j][k+2]+ P3XX(X)*NG_Def[i+2][j][k+2]);
	
	return der;
}

// dPixS / dP2YY
double derP2YY_PixS(double X, double Z, long i, long j, long k)
{
	double der = P0ZZ(Z)*(P0XX(X)*NG_Def[i-1][j+1][k-1] + P1XX(X)*NG_Def[i][j+1][k-1]+ P2XX(X)*NG_Def[i+1][j+1][k-1]+ P3XX(X)*NG_Def[i+2][j+1][k-1])
	+ P1ZZ(Z)*(P0XX(X)*NG_Def[i-1][j+1][k] + P1XX(X)*NG_Def[i][j+1][k]+ P2XX(X)*NG_Def[i+1][j+1][k]+ P3XX(X)*NG_Def[i+2][j+1][k])
	+ P2ZZ(Z)*(P0XX(X)*NG_Def[i-1][j+1][k+1] + P1XX(X)*NG_Def[i][j+1][k+1]+ P2XX(X)*NG_Def[i+1][j+1][k+1]+ P3XX(X)*NG_Def[i+2][j+1][k+1])
	+ P3ZZ(Z)*(P0XX(X)*NG_Def[i-1][j+1][k+2] + P1XX(X)*NG_Def[i][j+1][k+2]+ P2XX(X)*NG_Def[i+1][j+1][k+2]+ P3XX(X)*NG_Def[i+2][j+1][k+2]);
	
	return der;
}

// dPixS / dP3YY
double derP3YY_PixS(double X, double Z, long i, long j, long k)
{
	double der = P0ZZ(Z)*(P0XX(X)*NG_Def[i-1][j+2][k-1] + P1XX(X)*NG_Def[i][j+2][k-1]+ P2XX(X)*NG_Def[i+1][j+2][k-1]+ P3XX(X)*NG_Def[i+2][j+2][k-1])
	+ P1ZZ(Z)*(P0XX(X)*NG_Def[i-1][j+2][k] + P1XX(X)*NG_Def[i][j+2][k]+ P2XX(X)*NG_Def[i+1][j+2][k]+ P3XX(X)*NG_Def[i+2][j+2][k])
	+ P2ZZ(Z)*(P0XX(X)*NG_Def[i-1][j+2][k+1] + P1XX(X)*NG_Def[i][j+2][k+1]+ P2XX(X)*NG_Def[i+1][j+2][k+1]+ P3XX(X)*NG_Def[i+2][j+2][k+1])
	+ P3ZZ(Z)*(P0XX(X)*NG_Def[i-1][j+2][k+2] + P1XX(X)*NG_Def[i][j+2][k+2]+ P2XX(X)*NG_Def[i+1][j+2][k+2]+ P3XX(X)*NG_Def[i+2][j+2][k+2]);
	
	return der;
}

// dPixS / dP0ZZ
double derP0ZZ_PixS(double X, double Y, long i, long j, long k)
{
	double der = P0YY(Y)*(P0XX(X)*NG_Def[i-1][j-1][k-1] + P1XX(X)*NG_Def[i][j-1][k-1]+ P2XX(X)*NG_Def[i+1][j-1][k-1]+ P3XX(X)*NG_Def[i+2][j-1][k-1])
	+ P1YY(Y)*(P0XX(X)*NG_Def[i-1][j][k-1] + P1XX(X)*NG_Def[i][j][k-1]+ P2XX(X)*NG_Def[i+1][j][k-1]+ P3XX(X)*NG_Def[i+2][j][k-1])
	+ P2YY(Y)*(P0XX(X)*NG_Def[i-1][j+1][k-1] + P1XX(X)*NG_Def[i][j+1][k-1]+ P2XX(X)*NG_Def[i+1][j+1][k-1]+ P3XX(X)*NG_Def[i+2][j+1][k-1])
	+ P3YY(Y)*(P0XX(X)*NG_Def[i-1][j+2][k-1] + P1XX(X)*NG_Def[i][j+2][k-1]+ P2XX(X)*NG_Def[i+1][j+2][k-1]+ P3XX(X)*NG_Def[i+2][j+2][k-1]);
	
	return der;
}

// dPixS / dP1ZZ
double derP1ZZ_PixS(double X, double Y, long i, long j, long k)
{
	double der = P0YY(Y)*(P0XX(X)*NG_Def[i-1][j-1][k] + P1XX(X)*NG_Def[i][j-1][k]+ P2XX(X)*NG_Def[i+1][j-1][k]+ P3XX(X)*NG_Def[i+2][j-1][k])
	+ P1YY(Y)*(P0XX(X)*NG_Def[i-1][j][k] + P1XX(X)*NG_Def[i][j][k]+ P2XX(X)*NG_Def[i+1][j][k]+ P3XX(X)*NG_Def[i+2][j][k])
	+ P2YY(Y)*(P0XX(X)*NG_Def[i-1][j+1][k] + P1XX(X)*NG_Def[i][j+1][k]+ P2XX(X)*NG_Def[i+1][j+1][k]+ P3XX(X)*NG_Def[i+2][j+1][k])
	+ P3YY(Y)*(P0XX(X)*NG_Def[i-1][j+2][k] + P1XX(X)*NG_Def[i][j+2][k]+ P2XX(X)*NG_Def[i+1][j+2][k]+ P3XX(X)*NG_Def[i+2][j+2][k]);
	
	return der;
}

// dPixS / dP2ZZ
double derP2ZZ_PixS(double X, double Y, long i, long j, long k)
{
	double der = P0YY(Y)*(P0XX(X)*NG_Def[i-1][j-1][k+1] + P1XX(X)*NG_Def[i][j-1][k+1]+ P2XX(X)*NG_Def[i+1][j-1][k+1]+ P3XX(X)*NG_Def[i+2][j-1][k+1])
	+ P1YY(Y)*(P0XX(X)*NG_Def[i-1][j][k+1] + P1XX(X)*NG_Def[i][j][k+1]+ P2XX(X)*NG_Def[i+1][j][k+1]+ P3XX(X)*NG_Def[i+2][j][k+1])
	+ P2YY(Y)*(P0XX(X)*NG_Def[i-1][j+1][k+1] + P1XX(X)*NG_Def[i][j+1][k+1]+ P2XX(X)*NG_Def[i+1][j+1][k+1]+ P3XX(X)*NG_Def[i+2][j+1][k+1])
	+ P3YY(Y)*(P0XX(X)*NG_Def[i-1][j+2][k+1] + P1XX(X)*NG_Def[i][j+2][k+1]+ P2XX(X)*NG_Def[i+1][j+2][k+1]+ P3XX(X)*NG_Def[i+2][j+2][k+1]);
	
	return der;
}

// dPixS / dP3ZZ
double derP3ZZ_PixS(double X, double Y, long i, long j, long k)
{
	double der = P0YY(Y)*(P0XX(X)*NG_Def[i-1][j-1][k+2] + P1XX(X)*NG_Def[i][j-1][k+2]+ P2XX(X)*NG_Def[i+1][j-1][k+2]+ P3XX(X)*NG_Def[i+2][j-1][k+2])
	+ P1YY(Y)*(P0XX(X)*NG_Def[i-1][j][k+2] + P1XX(X)*NG_Def[i][j][k+2]+ P2XX(X)*NG_Def[i+1][j][k+2]+ P3XX(X)*NG_Def[i+2][j][k+2])
	+ P2YY(Y)*(P0XX(X)*NG_Def[i-1][j+1][k+2] + P1XX(X)*NG_Def[i][j+1][k+2]+ P2XX(X)*NG_Def[i+1][j+1][k+2]+ P3XX(X)*NG_Def[i+2][j+1][k+2])
	+ P3YY(Y)*(P0XX(X)*NG_Def[i-1][j+2][k+2] + P1XX(X)*NG_Def[i][j+2][k+2]+ P2XX(X)*NG_Def[i+1][j+2][k+2]+ P3XX(X)*NG_Def[i+2][j+2][k+2]);
	
	return der;
}

int Coeff_Correlation(DVect Iv, double u[24], long Lcur, bool deriv, double *correl_num, double *correl_den_def, double *correl_den_ref, double derivCoeff_1[24], double derivCoeff_2[24])
{
// Iv: coordonnées du centre du voxern de l'image non déformée, dont on cherche la position (i.e. le champ de déplacement) dans l'image déformée
// deriv: vaut 0 lors de l'appel si l'on veut juste calculer le coefficient de corrélation, vaut 1 si on veut aussi calculer les dérivées
	int Ipat, Jpat, Kpat;
	double Udef, Vdef, Wdef;
	DVect coord_cur;
	DVect coordvv;
	LVect Dep_000;		// coordonnées entières du point courant dans l'image déformée
	double NG_Def_Vox, NG_Ref_Vox;
	DVect coordvv_Ref;
	LVect Dep_000_Ref;
	LVect coord_cur_Ref;
	double Lcur2, Lcur3;
	double dai_X[24];
	double dai_Y[24];
	double dai_Z[24];
	double derai_PixS[24];
	
	for (int i=0; i<24; i++)
	{
		derivCoeff_1[i] = 0.0;
		derivCoeff_2[i] = 0.0;
	}
	*correl_num = 0.0;
	*correl_den_def = 0.0;
	*correl_den_ref = 0.0;
	
	for(Ipat = 0; Ipat < Lcur; Ipat++)
	{
		for(Jpat = 0; Jpat < Lcur; Jpat++)
		{
			for(Kpat = 0; Kpat < Lcur; Kpat++)
			{
	
				Lcur2 = pow((double)Lcur,2);
				Lcur3 = pow((double)Lcur,3);
				Udef = u[0] + u[3]*Ipat/Lcur + u[6]*Jpat/Lcur + u[9]*Kpat/Lcur + u[12]*(Ipat*Jpat)/Lcur2 + u[15]*(Ipat*Kpat)/Lcur2 + u[18]*(Jpat*Kpat)/Lcur2 + u[21]*(Ipat*Jpat*Kpat)/Lcur3;
				Vdef = u[1] + u[4]*Ipat/Lcur + u[7]*Jpat/Lcur + u[10]*Kpat/Lcur + u[13]*(Ipat*Jpat)/Lcur2 + u[16]*(Ipat*Kpat)/Lcur2 + u[19]*(Jpat*Kpat)/Lcur2 + u[22]*(Ipat*Jpat*Kpat)/Lcur3;
				Wdef = u[2] + u[5]*Ipat/Lcur + u[8]*Jpat/Lcur + u[11]*Kpat/Lcur + u[14]*(Ipat*Jpat)/Lcur2 + u[17]*(Ipat*Kpat)/Lcur2 + u[20]*(Jpat*Kpat)/Lcur2 + u[23]*(Ipat*Jpat*Kpat)/Lcur3;
				
				coordvv.setX(Ipat + Iv.getX() - Lcur/2.0 + Udef);
				coordvv.setY(Jpat + Iv.getY() - Lcur/2.0 + Vdef);
				coordvv.setZ(Kpat + Iv.getZ() - Lcur/2.0 + Wdef);
// 				cout << Ipat + Iv.getX() - Lcur/2.0 + Udef << endl << flush;
// 				cout << "coordvv " << coordvv.getX() << endl << flush;
				Dep_000 = (LVect)(coordvv);
				//cout << "Dep_000 = (" << Dep_000.getX() << ", "<< Dep_000.getY() << ", "<< Dep_000.getZ() << ")" << endl << flush;
	// 			X_000 = (long)(Xvv_Def); 
	// 			Y_000 = (long)(Yvv_Def);
	// 			Z_000 = (long)(Zvv_Def);
				
				
				if ((Dep_000.getX() < 0) || (Dep_000.getY() < 0) || (Dep_000.getZ() < 0))
				{
					// TODO: faire message d'erreur pour sortie des bornes mini
// 					cout << "coucou" << endl << flush;
					return (5);
				}
				
				if ((Dep_000.getX() > TAILLE_IMAGE ) || (Dep_000.getY() > TAILLE_IMAGE ) || (Dep_000.getZ() > TAILLE_IMAGE ))
				{
					// TODO: faire message d'erreur pour sortie des bornes maxi
					cout << "coucou2" << endl << flush;
					return (2);
				}
	
				//	Coordonnées du point courant dans le voxern unitaire
				coord_cur = coordvv - (DVect)Dep_000;
	// 			Xcur = Xvv_Def - (double)X_000;
	// 			Ycur = Yvv_Def - (double)Y_000;
	// 			Zcur = Zvv_Def - (double)Z_000;
				
				long Dep000_X = Dep_000.getX();
				long Dep000_Y = Dep_000.getY();
				long Dep000_Z = Dep_000.getZ();
	
				double coord_cur_X = coord_cur.getX();
				double coord_cur_Y = coord_cur.getY();
				double coord_cur_Z = coord_cur.getZ();
				
				NG_Def_Vox = P0XX(coord_cur_X) * (P0YY(coord_cur_Y) * (P0ZZ(coord_cur_Z) * NG_Def[Dep000_X - 1][Dep000_Y - 1][Dep000_Z - 1] + P1ZZ(coord_cur_Z) * NG_Def[Dep000_X - 1][Dep000_Y - 1][Dep000_Z] + P2ZZ(coord_cur_Z) * NG_Def[Dep000_X - 1][Dep000_Y - 1][Dep000_Z + 1] + P3ZZ(coord_cur_Z) * NG_Def[Dep000_X - 1][Dep000_Y - 1][Dep000_Z + 2])
				+ P1YY(coord_cur_Y) * (P0ZZ(coord_cur_Z) * NG_Def[Dep000_X - 1][Dep000_Y][Dep000_Z - 1] + P1ZZ(coord_cur_Z) * NG_Def[Dep000_X - 1][Dep000_Y][Dep000_Z] + P2ZZ(coord_cur_Z) * NG_Def[Dep000_X - 1][Dep000_Y][Dep000_Z + 1] + P3ZZ(coord_cur_Z) * NG_Def[Dep000_X - 1][Dep000_Y][Dep000_Z + 2])
				+P2YY(coord_cur_Y) * (P0ZZ(coord_cur_Z) * NG_Def[Dep000_X - 1][Dep000_Y + 1][Dep000_Z - 1] + P1ZZ(coord_cur_Z) * NG_Def[Dep000_X - 1][Dep000_Y + 1][Dep000_Z] + P2ZZ(coord_cur_Z) * NG_Def[Dep000_X - 1][Dep000_Y + 1][Dep000_Z + 1] + P3ZZ(coord_cur_Z) * NG_Def[Dep000_X - 1][Dep000_Y + 1][Dep000_Z + 2])
				+ P3YY(coord_cur_Y) * (P0ZZ(coord_cur_Z) * NG_Def[Dep000_X - 1][Dep000_Y + 2][Dep000_Z - 1] + P1ZZ(coord_cur_Z) * NG_Def[Dep000_X - 1][Dep000_Y + 2][Dep000_Z] + P2ZZ(coord_cur_Z) * NG_Def[Dep000_X - 1][Dep000_Y + 2][Dep000_Z + 1] + P3ZZ(coord_cur_Z) * NG_Def[Dep000_X - 1][Dep000_Y + 2][Dep000_Z + 2]))
				
				+ P1XX(coord_cur_X) * (P0YY(coord_cur_Y) * (P0ZZ(coord_cur_Z) * NG_Def[Dep000_X][Dep000_Y - 1][Dep000_Z - 1] + P1ZZ(coord_cur_Z) * NG_Def[Dep000_X][Dep000_Y - 1][Dep000_Z] + P2ZZ(coord_cur_Z) * NG_Def[Dep000_X][Dep000_Y - 1][Dep000_Z + 1] + P3ZZ(coord_cur_Z) * NG_Def[Dep000_X][Dep000_Y - 1][Dep000_Z + 2])
				+ P1YY(coord_cur_Y) * (P0ZZ(coord_cur_Z) * NG_Def[Dep000_X][Dep000_Y][Dep000_Z - 1] + P1ZZ(coord_cur_Z) * NG_Def[Dep000_X][Dep000_Y][Dep000_Z] + P2ZZ(coord_cur_Z) * NG_Def[Dep000_X][Dep000_Y][Dep000_Z + 1] + P3ZZ(coord_cur_Z) * NG_Def[Dep000_X][Dep000_Y][Dep000_Z + 2])
				+P2YY(coord_cur_Y) * (P0ZZ(coord_cur_Z) * NG_Def[Dep000_X][Dep000_Y + 1][Dep000_Z - 1] + P1ZZ(coord_cur_Z) * NG_Def[Dep000_X][Dep000_Y + 1][Dep000_Z] + P2ZZ(coord_cur_Z) * NG_Def[Dep000_X][Dep000_Y + 1][Dep000_Z + 1] + P3ZZ(coord_cur_Z) * NG_Def[Dep000_X][Dep000_Y + 1][Dep000_Z + 2])
				+ P3YY(coord_cur_Y) * (P0ZZ(coord_cur_Z) * NG_Def[Dep000_X][Dep000_Y + 2][Dep000_Z - 1] + P1ZZ(coord_cur_Z) * NG_Def[Dep000_X][Dep000_Y + 2][Dep000_Z] + P2ZZ(coord_cur_Z) * NG_Def[Dep000_X][Dep000_Y + 2][Dep000_Z + 1] + P3ZZ(coord_cur_Z) * NG_Def[Dep000_X][Dep000_Y + 2][Dep000_Z + 2]))
				
				+ P2XX(coord_cur_X) * (P0YY(coord_cur_Y) * (P0ZZ(coord_cur_Z) * NG_Def[Dep000_X + 1][Dep000_Y - 1][Dep000_Z - 1] + P1ZZ(coord_cur_Z) * NG_Def[Dep000_X + 1][Dep000_Y - 1][Dep000_Z] + P2ZZ(coord_cur_Z) * NG_Def[Dep000_X + 1][Dep000_Y - 1][Dep000_Z + 1] + P3ZZ(coord_cur_Z) * NG_Def[Dep000_X + 1][Dep000_Y - 1][Dep000_Z + 2])
				+ P1YY(coord_cur_Y) * (P0ZZ(coord_cur_Z) * NG_Def[Dep000_X + 1][Dep000_Y][Dep000_Z - 1] + P1ZZ(coord_cur_Z) * NG_Def[Dep000_X + 1][Dep000_Y][Dep000_Z] + P2ZZ(coord_cur_Z) * NG_Def[Dep000_X + 1][Dep000_Y][Dep000_Z + 1] + P3ZZ(coord_cur_Z) * NG_Def[Dep000_X + 1][Dep000_Y][Dep000_Z + 2])
				+P2YY(coord_cur_Y) * (P0ZZ(coord_cur_Z) * NG_Def[Dep000_X + 1][Dep000_Y + 1][Dep000_Z - 1] + P1ZZ(coord_cur_Z) * NG_Def[Dep000_X + 1][Dep000_Y + 1][Dep000_Z] + P2ZZ(coord_cur_Z) * NG_Def[Dep000_X + 1][Dep000_Y + 1][Dep000_Z + 1] + P3ZZ(coord_cur_Z) * NG_Def[Dep000_X + 1][Dep000_Y + 1][Dep000_Z + 2])
				+ P3YY(coord_cur_Y) * (P0ZZ(coord_cur_Z) * NG_Def[Dep000_X + 1][Dep000_Y + 2][Dep000_Z - 1] + P1ZZ(coord_cur_Z) * NG_Def[Dep000_X + 1][Dep000_Y + 2][Dep000_Z] + P2ZZ(coord_cur_Z) * NG_Def[Dep000_X + 1][Dep000_Y + 2][Dep000_Z + 1] + P3ZZ(coord_cur_Z) * NG_Def[Dep000_X + 1][Dep000_Y + 2][Dep000_Z + 2]))
				
				+ P3XX(coord_cur_X) * (P0YY(coord_cur_Y) * (P0ZZ(coord_cur_Z) * NG_Def[Dep000_X + 2][Dep000_Y - 1][Dep000_Z - 1] + P1ZZ(coord_cur_Z) * NG_Def[Dep000_X + 2][Dep000_Y - 1][Dep000_Z] + P2ZZ(coord_cur_Z) * NG_Def[Dep000_X + 2][Dep000_Y - 1][Dep000_Z + 1] + P3ZZ(coord_cur_Z) * NG_Def[Dep000_X + 2][Dep000_Y - 1][Dep000_Z + 2])
				+ P1YY(coord_cur_Y) * (P0ZZ(coord_cur_Z) * NG_Def[Dep000_X + 2][Dep000_Y][Dep000_Z - 1] + P1ZZ(coord_cur_Z) * NG_Def[Dep000_X + 2][Dep000_Y][Dep000_Z] + P2ZZ(coord_cur_Z) * NG_Def[Dep000_X + 2][Dep000_Y][Dep000_Z + 1] + P3ZZ(coord_cur_Z) * NG_Def[Dep000_X + 2][Dep000_Y][Dep000_Z + 2])
				+P2YY(coord_cur_Y) * (P0ZZ(coord_cur_Z) * NG_Def[Dep000_X + 2][Dep000_Y + 1][Dep000_Z - 1] + P1ZZ(coord_cur_Z) * NG_Def[Dep000_X + 2][Dep000_Y + 1][Dep000_Z] + P2ZZ(coord_cur_Z) * NG_Def[Dep000_X + 2][Dep000_Y + 1][Dep000_Z + 1] + P3ZZ(coord_cur_Z) * NG_Def[Dep000_X + 2][Dep000_Y + 1][Dep000_Z + 2])
				+ P3YY(coord_cur_Y) * (P0ZZ(coord_cur_Z) * NG_Def[Dep000_X + 2][Dep000_Y + 2][Dep000_Z - 1] + P1ZZ(coord_cur_Z) * NG_Def[Dep000_X + 2][Dep000_Y + 2][Dep000_Z] + P2ZZ(coord_cur_Z) * NG_Def[Dep000_X + 2][Dep000_Y + 2][Dep000_Z + 1] + P3ZZ(coord_cur_Z) * NG_Def[Dep000_X + 2][Dep000_Y + 2][Dep000_Z + 2]));
	
				

				coordvv_Ref.setX(Iv.getX() - Lcur/2. + Ipat); 
				coordvv_Ref.setY(Iv.getY() - Lcur/2. + Jpat);
				coordvv_Ref.setZ(Iv.getZ() - Lcur/2. + Kpat);
				
				Dep_000_Ref = (LVect)(coordvv_Ref);
				
				coord_cur_Ref = coordvv_Ref - (DVect)Dep_000_Ref;
				
				if ((Dep_000_Ref.getX() < 0) || (Dep_000_Ref.getY() < 0) || (Dep_000_Ref.getZ() < 0))
				{
					// TODO: faire message d'erreur pour sortie des bornes mini
					cout << "erreur borne !!!!" << endl << flush ;
					return (3);
				}
				
				if ((Dep_000_Ref.getX() >= TAILLE_IMAGE ) || (Dep_000_Ref.getY() >= TAILLE_IMAGE ) || (Dep_000_Ref.getZ() >= TAILLE_IMAGE ))
				{
					// TODO: faire message d'erreur pour sortie des bornes maxi
					cout << "erreur borne2 !!!!" << endl << flush ;
					return (4);
				}
	
				NG_Ref_Vox = NG_Ref[Dep_000_Ref.getX()][Dep_000_Ref.getY()][Dep_000_Ref.getZ()];
// 				cout << "Ipat " << Ipat << " Jpat " << Jpat << " Kpat " << Kpat << " NG_Def_Vox " << NG_Def_Vox << ", NG_Ref_Vox " << NG_Ref_Vox << endl << flush;
				
				*correl_num += NG_Def_Vox*NG_Ref_Vox;			// Somme des PixS * PixIIdp
				*correl_den_def += NG_Def_Vox*NG_Def_Vox;		// Somme des PixS^2
				*correl_den_ref += NG_Ref_Vox*NG_Ref_Vox;		// Somme des PixIIdp^2
				
				if (deriv == true)
				{
				
// 					dai_X[0] = 1.0;
// 					dai_X[1] = 0.0;
// 					dai_X[2] = 0.0;
// 					dai_X[3] = coord_cur.getX();
// 					dai_X[4] = 0.0;
// 					dai_X[5] = 0.0;
// 					dai_X[6] = coord_cur.getY();
// 					dai_X[7] = 0.0;
// 					dai_X[8] = 0.0;
// 					dai_X[9] = coord_cur.getZ();
// 					dai_X[10] = 0.0;
// 					dai_X[11] = 0.0;
// 					dai_X[12] = coord_cur.getX()*coord_cur.getY();
// 					dai_X[13] = 0.0;
// 					dai_X[14] = 0.0;
// 					dai_X[15] = coord_cur.getX()*coord_cur.getZ();
// 					dai_X[16] = 0.0;
// 					dai_X[17] = 0.0;
// 					dai_X[18] = coord_cur.getY()*coord_cur.getZ();
// 					dai_X[19] = 0.0;
// 					dai_X[20] = 0.0;
// 					dai_X[21] = coord_cur.getX()*coord_cur.getY()*coord_cur.getZ();
// 					dai_X[22] = 0.0;
// 					dai_X[23] = 0.0;
// 					
// 					dai_Y[0] = 0.0;
// 					dai_Y[1] = 1.0;
// 					dai_Y[2] = 0.0;
// 					dai_Y[3] = 0.0;
// 					dai_Y[4] = coord_cur.getX();
// 					dai_Y[5] = 0.0;
// 					dai_Y[6] = 0.0;
// 					dai_Y[7] =  coord_cur.getY();
// 					dai_Y[8] = 0.0;
// 					dai_Y[9] = 0.0;
// 					dai_Y[10] = coord_cur.getZ();
// 					dai_Y[11] = 0.0;
// 					dai_Y[12] = 0.0;
// 					dai_Y[13] = coord_cur.getX()*coord_cur.getY();
// 					dai_Y[14] = 0.0;
// 					dai_Y[15] = 0.0;
// 					dai_Y[16] = coord_cur.getX()*coord_cur.getZ();
// 					dai_Y[17] = 0.0;
// 					dai_Y[18] = 0.0;
// 					dai_Y[19] = coord_cur.getY()*coord_cur.getZ();
// 					dai_Y[20] = 0.0;
// 					dai_Y[21] = 0.0;
// 					dai_Y[22] = coord_cur.getX()*coord_cur.getY()*coord_cur.getZ();
// 					dai_Y[23] = 0.0;
// 					
// 					dai_Z[0] = 0.0;
// 					dai_Z[1] = 0.0;
// 					dai_Z[2] = 1.0;
// 					dai_Z[3] = 0.0;
// 					dai_Z[4] = 0.0;
// 					dai_Z[5] = coord_cur.getX();
// 					dai_Z[6] = 0.0;
// 					dai_Z[7] = 0.0;
// 					dai_Z[8] = coord_cur.getY();
// 					dai_Z[9] = 0.0;
// 					dai_Z[10] = 0.0;
// 					dai_Z[11] = coord_cur.getZ();
// 					dai_Z[12] = 0.0;
// 					dai_Z[13] = 0.0;
// 					dai_Z[14] = coord_cur.getX()*coord_cur.getY();
// 					dai_Z[15] = 0.0;
// 					dai_Z[16] = 0.0;
// 					dai_Z[17] = coord_cur.getX()*coord_cur.getZ();
// 					dai_Z[18] = 0.0;
// 					dai_Z[19] = 0.0;
// 					dai_Z[20] = coord_cur.getY()*coord_cur.getZ();
// 					dai_Z[21] = 0.0;
// 					dai_Z[22] = 0.0;
// 					dai_Z[23] = coord_cur.getX()*coord_cur.getY()*coord_cur.getZ();
// 					
// 					for (int i=0; i < 24; i++)
// 					{
// 						derai_PixS[i] = derP0XX_PixS(coord_cur.getY(), coord_cur.getZ(), Dep_000.getX(), Dep_000.getY(), Dep_000.getZ())*derX_P0XX(coord_cur.getX())*dai_X[i] + derP1XX_PixS(coord_cur.getY(), coord_cur.getZ(), Dep_000.getX(), Dep_000.getY(), Dep_000.getZ())*derX_P1XX(coord_cur.getX())*dai_X[i] + derP2XX_PixS(coord_cur.getY(), coord_cur.getZ(), Dep_000.getX(), Dep_000.getY(), Dep_000.getZ())*derX_P2XX(coord_cur.getX())*dai_X[i] + derP3XX_PixS(coord_cur.getY(), coord_cur.getZ(), Dep_000.getX(), Dep_000.getY(), Dep_000.getZ())*derX_P3XX(coord_cur.getX())*dai_X[i]
// 						
// 						+ derP0YY_PixS(coord_cur.getX(), coord_cur.getZ(), Dep_000.getX(), Dep_000.getY(), Dep_000.getZ())*derY_P0YY(coord_cur.getY())*dai_Y[i] + derP1YY_PixS(coord_cur.getX(), coord_cur.getZ(), Dep_000.getX(), Dep_000.getY(), Dep_000.getZ())*derY_P1YY(coord_cur.getY())*dai_Y[i] + derP2YY_PixS(coord_cur.getX(), coord_cur.getZ(), Dep_000.getX(), Dep_000.getY(), Dep_000.getZ())*derY_P2YY(coord_cur.getY())*dai_Y[i] + derP3YY_PixS(coord_cur.getX(), coord_cur.getZ(), Dep_000.getX(), Dep_000.getY(), Dep_000.getZ())*derY_P3YY(coord_cur.getY())*dai_Y[i]
// 						
// 						+ derP0ZZ_PixS(coord_cur.getX(), coord_cur.getY(), Dep_000.getX(), Dep_000.getY(), Dep_000.getZ())*derZ_P0ZZ(coord_cur.getZ())*dai_Z[i] + derP1ZZ_PixS(coord_cur.getX(), coord_cur.getY(), Dep_000.getX(), Dep_000.getY(), Dep_000.getZ())*derZ_P1ZZ(coord_cur.getZ())*dai_Z[i] + derP2ZZ_PixS(coord_cur.getX(), coord_cur.getY(), Dep_000.getX(), Dep_000.getY(), Dep_000.getZ())*derZ_P2ZZ(coord_cur.getZ())*dai_Z[i] + derP3ZZ_PixS(coord_cur.getX(), coord_cur.getY(), Dep_000.getX(), Dep_000.getY(), Dep_000.getZ())*derZ_P3ZZ(coord_cur.getZ())*dai_Z[i];
// 					}
					
					for (int i=0; i<24; i++)
					{
						derivCoeff_1[i] += derai_PixS[i] * NG_Ref_Vox;		
	// 					cout << "derivCoeff1 " << *derivCoeff_1 << endl << flush;
						derivCoeff_2[i] += 2.0 * NG_Def_Vox * derai_PixS[i];	
					}
				}
				
			}
		}
	}
return (1);
}



//Calcul coefficient de corrélation et dérivées

int fonction_test (DVect Iv, double u[24], long Lcur)
{
	double correl_num, correl_den_def, correl_den_ref, derivCoeff_1[24], derivCoeff_2[24];
	double coeff_correl;
	double deriv_au_coeff, deriv_bu_coeff, deriv_cu_coeff, deriv_du_coeff, deriv_eu_coeff, deriv_fu_coeff, deriv_gu_coeff, deriv_hu_coeff;
	double deriv_av_coeff, deriv_bv_coeff, deriv_cv_coeff, deriv_dv_coeff, deriv_ev_coeff, deriv_fv_coeff, deriv_gv_coeff, deriv_hv_coeff;
	double deriv_aw_coeff, deriv_bw_coeff, deriv_cw_coeff, deriv_dw_coeff, deriv_ew_coeff, deriv_fw_coeff, deriv_gw_coeff, deriv_hw_coeff;
	
	bool deriv = true;
	
	correl_num = 0.0;
	correl_den_def = 0.0;
	correl_den_ref = 0.0;
	
	
	
	if (Coeff_Correlation(Iv, u, Lcur, deriv, &correl_num, &correl_den_def, &correl_den_ref, derivCoeff_1, derivCoeff_2) == 1)	
	{
		coeff_correl = 1.0 - correl_num / pow((correl_den_def * correl_den_ref),0.5);
// 		cout << "correl num " << correl_num << " correl den def " << correl_den_def << " correl den ref " << correl_den_def << endl << flush;
		cout << "coefficient de corrélation = " << coeff_correl << endl;

	}
	else
	cout << "erreur calcul corrélation" << endl << flush;
	
	if (deriv)
	{
	
	deriv_au_coeff = -(derivCoeff_1[3] * pow(correl_den_def*correl_den_ref,0.5) - correl_num * 0.5 * (1.0/(pow(correl_den_def*correl_den_ref,0.5))) * derivCoeff_2[3] * correl_den_ref) / (correl_den_def * correl_den_ref);
	deriv_bu_coeff = -(derivCoeff_1[6] * pow(correl_den_def*correl_den_ref,0.5) - correl_num*0.5 * (1.0/(pow(correl_den_def*correl_den_ref,0.5))) * derivCoeff_2[6] * correl_den_ref) / (correl_den_def * correl_den_ref);
	deriv_cu_coeff -(derivCoeff_1[9] * pow(correl_den_def*correl_den_ref,0.5) - correl_num*0.5 * (1.0/(pow(correl_den_def*correl_den_ref,0.5))) * derivCoeff_2[9] * correl_den_ref) / (correl_den_def * correl_den_ref);
	deriv_du_coeff = -(derivCoeff_1[12] * pow(correl_den_def*correl_den_ref,0.5) - correl_num*0.5 * (1.0/(pow(correl_den_def*correl_den_ref,0.5))) * derivCoeff_2[12] * correl_den_ref) / (correl_den_def * correl_den_ref);
	deriv_eu_coeff = -(derivCoeff_1[15] * pow(correl_den_def*correl_den_ref,0.5) - correl_num*0.5 * (1.0/(pow(correl_den_def*correl_den_ref,0.5))) * derivCoeff_2[15] * correl_den_ref) / (correl_den_def * correl_den_ref);
	deriv_fu_coeff = -(derivCoeff_1[18] * pow(correl_den_def*correl_den_ref,0.5) - correl_num*0.5 * (1.0/(pow(correl_den_def*correl_den_ref,0.5))) * derivCoeff_2[18] * correl_den_ref) / (correl_den_def * correl_den_ref);
	deriv_gu_coeff = -(derivCoeff_1[21] * pow(correl_den_def*correl_den_ref,0.5) - correl_num*0.5 * (1.0/(pow(correl_den_def*correl_den_ref,0.5))) * derivCoeff_2[21] * correl_den_ref) / (correl_den_def * correl_den_ref);
	deriv_hu_coeff = -(derivCoeff_1[0] * pow(correl_den_def*correl_den_ref,0.5) - correl_num * 0.5 * (1.0 / (pow(correl_den_def*correl_den_ref,0.5)) ) * derivCoeff_2[0] * correl_den_ref) / (correl_den_def * correl_den_ref);
// 	deriv_hu_coeff = -(derivCoeff_1[0] * pow(correl_den_def*correl_den_ref,0.5) - correl_num * pow((derivCoeff_2[0] * correl_den_ref),0.5)) / (correl_den_def * correl_den_ref);
	
	deriv_av_coeff = -(derivCoeff_1[4] * pow(correl_den_def*correl_den_ref,0.5) - correl_num*0.5 * (1.0/(pow(correl_den_def*correl_den_ref,0.5))) * derivCoeff_2[4] * correl_den_ref) / (correl_den_def * correl_den_ref);
	deriv_bv_coeff = -(derivCoeff_1[7] * pow(correl_den_def*correl_den_ref,0.5) - correl_num*0.5 * (1.0/(pow(correl_den_def*correl_den_ref,0.5))) * derivCoeff_2[7] * correl_den_ref) / (correl_den_def * correl_den_ref);
	deriv_cv_coeff = -(derivCoeff_1[10] * pow(correl_den_def*correl_den_ref,0.5) - correl_num*0.5 * (1.0/(pow(correl_den_def*correl_den_ref,0.5))) * derivCoeff_2[10] * correl_den_ref) / (correl_den_def * correl_den_ref);
	deriv_dv_coeff = -(derivCoeff_1[13] * pow(correl_den_def*correl_den_ref,0.5) - correl_num*0.5 * (1.0/(pow(correl_den_def*correl_den_ref,0.5))) * derivCoeff_2[13] * correl_den_ref) / (correl_den_def * correl_den_ref);
	deriv_ev_coeff = -(derivCoeff_1[16] * pow(correl_den_def*correl_den_ref,0.5) - correl_num*0.5 * (1.0/(pow(correl_den_def*correl_den_ref,0.5))) * derivCoeff_2[16] * correl_den_ref) / (correl_den_def * correl_den_ref);
	deriv_fv_coeff = -(derivCoeff_1[19] * pow(correl_den_def*correl_den_ref,0.5) - correl_num*0.5 * (1.0/(pow(correl_den_def*correl_den_ref,0.5))) * derivCoeff_2[19] * correl_den_ref) / (correl_den_def * correl_den_ref);
	deriv_gv_coeff = -(derivCoeff_1[22] * pow(correl_den_def*correl_den_ref,0.5) - correl_num*0.5 * (1.0/(pow(correl_den_def*correl_den_ref,0.5))) * derivCoeff_2[22] * correl_den_ref) / (correl_den_def * correl_den_ref);
	deriv_hv_coeff = -(derivCoeff_1[1] * pow(correl_den_def*correl_den_ref,0.5) - correl_num*0.5 * (1.0/(pow(correl_den_def*correl_den_ref,0.5))) * derivCoeff_2[1] * correl_den_ref) / (correl_den_def * correl_den_ref);
	
	deriv_aw_coeff = -(derivCoeff_1[5] * pow(correl_den_def*correl_den_ref,0.5) - correl_num*0.5 * (1.0/(pow(correl_den_def*correl_den_ref,0.5))) * derivCoeff_2[5] * correl_den_ref) / (correl_den_def * correl_den_ref);
	deriv_bw_coeff = -(derivCoeff_1[8] * pow(correl_den_def*correl_den_ref,0.5) - correl_num*0.5 * (1.0/(pow(correl_den_def*correl_den_ref,0.5))) * derivCoeff_2[8] * correl_den_ref) / (correl_den_def * correl_den_ref);
	deriv_cw_coeff = -(derivCoeff_1[11] * pow(correl_den_def*correl_den_ref,0.5) - correl_num*0.5 * (1.0/(pow(correl_den_def*correl_den_ref,0.5))) * derivCoeff_2[11] * correl_den_ref) / (correl_den_def * correl_den_ref);
	deriv_dw_coeff = -(derivCoeff_1[14] * pow(correl_den_def*correl_den_ref,0.5) - correl_num*0.5 * (1.0/(pow(correl_den_def*correl_den_ref,0.5))) * derivCoeff_2[14] * correl_den_ref) / (correl_den_def * correl_den_ref);
	deriv_ew_coeff = -(derivCoeff_1[17] * pow(correl_den_def*correl_den_ref,0.5) - correl_num*0.5 * (1.0/(pow(correl_den_def*correl_den_ref,0.5))) * derivCoeff_2[17] * correl_den_ref) / (correl_den_def * correl_den_ref);
	deriv_fw_coeff = -(derivCoeff_1[20] * pow(correl_den_def*correl_den_ref,0.5) - correl_num*0.5 * (1.0/(pow(correl_den_def*correl_den_ref,0.5))) * derivCoeff_2[20] * correl_den_ref) / (correl_den_def * correl_den_ref);
	deriv_gw_coeff = -(derivCoeff_1[23] * pow(correl_den_def*correl_den_ref,0.5) - correl_num*0.5 * (1.0/(pow(correl_den_def*correl_den_ref,0.5))) * derivCoeff_2[23] * correl_den_ref) / (correl_den_def * correl_den_ref);
	deriv_hw_coeff = -(derivCoeff_1[2] * pow(correl_den_def*correl_den_ref,0.5) - correl_num*0.5 * (1.0/(pow(correl_den_def*correl_den_ref,0.5))) * derivCoeff_2[2] * correl_den_ref) / (correl_den_def * correl_den_ref);
	
	
	
	// Test dérivation
	double utest_p[24];
	double utest_m[24];
	double coeff_correl_test_p;
	double coeff_correl_test_m;
	double deriv_coeff_test[24];
	for (int i=0; i < 24; i++)
	{
		utest_p[i] = u[i];
		utest_m[i] = u[i];
	}
	
	// Dérivation par différences finies par rapport à chacune des variables
	
	for (int i=0; i<24; i++)
	{
		utest_p[i] += 1e-4;
		utest_m[i] -= 1e-4;
		
		Coeff_Correlation(Iv, utest_p, Lcur, deriv, &correl_num, &correl_den_def, &correl_den_ref, derivCoeff_1, derivCoeff_2);
		coeff_correl_test_p = 1.0 - correl_num / pow((correl_den_def * correl_den_ref),0.5);
// 		cout << "coeff plus " << coeff_correl_test_p << endl << flush;
		
		Coeff_Correlation(Iv, utest_m, Lcur, deriv, &correl_num, &correl_den_def, &correl_den_ref, derivCoeff_1, derivCoeff_2);
		coeff_correl_test_m = 1.0 - correl_num / pow((correl_den_def * correl_den_ref),0.5);
// 		cout << "coeff moins " << coeff_correl_test_m << endl << flush;
		
		deriv_coeff_test[i] = (coeff_correl_test_p - coeff_correl_test_m) / (2.0*1e-4);
		
		utest_p[i] = u[i];
		utest_m[i] = u[i];
	}
	
	cout << "deriv hu avec diff finies " << deriv_coeff_test[0] << endl << flush;
	cout << " deriv hu " << deriv_hu_coeff << endl << flush;
	
// 	double deriv_hu_coeff_test;
// 	for (int i=0; i < 24; i++)
// 	{
// 		utest_p[i] = u[i];
// 		utest_m[i] = u[i];
// 	}
// 	
// 	utest_p[0] += 1.0;
// 	utest_m[0] -= 1.0;
// 	
// 	Coeff_Correlation(Iv, utest_p, Lcur, deriv, &correl_num, &correl_den_def, &correl_den_ref, &derivCoeff_1, &derivCoeff_2, &derivCoeff_3, &derivCoeff_4);
// 	coeff_correl_test_p = 1.0 - correl_num / pow((correl_den_def * correl_den_ref),0.5);
// 	cout << "coeff plus " << coeff_correl_test_p << endl << flush;
// 	
// 	Coeff_Correlation(Iv, utest_m, Lcur, deriv, &correl_num, &correl_den_def, &correl_den_ref, &derivCoeff_1, &derivCoeff_2, &derivCoeff_3, &derivCoeff_4);
// 	coeff_correl_test_m = 1.0 - correl_num / pow((correl_den_def * correl_den_ref),0.5);
// 	cout << "coeff moins " << coeff_correl_test_m << endl << flush;
// 	
// 	deriv_hu_coeff_test = (coeff_correl_test_p - coeff_correl_test_m) / (2.0*1.0);
// 	
// 	cout << "deriv hu avec diff finies " << deriv_hu_coeff_test << endl << flush;
// 	cout << " deriv hu " << deriv_hu_coeff << endl << flush;
	
	
// 	cout << " deriv au " << deriv_au_coeff << endl << flush;
// 	cout << " deriv bu " << deriv_bu_coeff << endl << flush;
// 	cout << " deriv cu " << deriv_cu_coeff << endl << flush;
// 	cout << " deriv du " << deriv_du_coeff << endl << flush;
// 	cout << " deriv eu " << deriv_eu_coeff << endl << flush;
// 	cout << " deriv fu " << deriv_fu_coeff << endl << flush;
// 	cout << " deriv gu " << deriv_gu_coeff << endl << flush;
// 	cout << " deriv hu " << deriv_hu_coeff << endl << flush;
// 	cout << " deriv av " << deriv_av_coeff << endl << flush;
// 	cout << " deriv bv " << deriv_bv_coeff << endl << flush;
// 	cout << " deriv cv " << deriv_cv_coeff << endl << flush;
// 	cout << " deriv dv " << deriv_dv_coeff << endl << flush;
// 	cout << " deriv ev " << deriv_ev_coeff << endl << flush;
// 	cout << " deriv fv " << deriv_fv_coeff << endl << flush;
// 	cout << " deriv gv " << deriv_gv_coeff << endl << flush;
// 	cout << " deriv hv " << deriv_hv_coeff << endl << flush;
// 	cout << " deriv aw " << deriv_aw_coeff << endl << flush;
// 	cout << " deriv bw " << deriv_bw_coeff << endl << flush;
// 	cout << " deriv cw " << deriv_cw_coeff << endl << flush;
// 	cout << " deriv dw " << deriv_dw_coeff << endl << flush;
// 	cout << " deriv ew " << deriv_ew_coeff << endl << flush;
// 	cout << " deriv fw " << deriv_fw_coeff << endl << flush;
// 	cout << " deriv gw " << deriv_gw_coeff << endl << flush;
// 	cout << " deriv hw " << deriv_hw_coeff << endl << flush;
	}
	
// 	for (int l = 0; l < 24; l++)
// 	{
// 		u[l] = 0.0;
// 	}
return (1);
}




int main(int, char**)
{
	long i, j, k;
	unsigned char valeur;
	long l, m, n, o;
	long Lcur;
	DVect Iv;		// 
	
cout << "entrée dans le main" << endl << flush;

	// Initialisation du générateur de nombres aléatoires
	srand(time(0));

	for(i=0; i<TAILLE_IMAGE; i++)
		for(j=0; j<TAILLE_IMAGE; j++)
			for(k=0; k<TAILLE_IMAGE; k++)
			{
				// Valeur de 1 à 254 (0xfe)
				valeur = rand() % 0xfd + 1;
				
				NG_Ref[i][j][k] = valeur;
			}
	
	for(i=0; i<TAILLE_IMAGE; i++)
		for(j=0; j<TAILLE_IMAGE; j++)
			for(k=0; k<TAILLE_IMAGE; k++)
			{
				NG_Def[i][j][k] = NG_Ref[i+1][j][k];
			}
	
	cout << "images définies" << endl << flush;
			
	double u[24];
	
	//Initialisation du champ de déformation
	for (l = 0; l < 24; l	++)
	{
		u[l] = 0.0;
	}
// 	u[0] = 1.0;
// 	u[1] = 1.0;
// 	u[2] = 1.0;
	Lcur = 4;
	
	// TODO: Placer le code de test ici !! :-)
	
// 	Iv.setX(1.0);
// 	Iv.setY(1.0);
// 	Iv.setZ(1.0);
	
	for (m = 1; m < (TAILLE_IMAGE-1); m++)
	{
		Iv.setX(m);
		for (n = 1; n < (TAILLE_IMAGE-1); n++)
		{
			Iv.setY(n);
			for (o = 1; o < (TAILLE_IMAGE-1); o++)
			{
			cout << "point (" << m << ", " << n << ", " << o << ")" << endl << flush;
				Iv.setZ(o);
				fonction_test (Iv, u, Lcur);
			}
		}
	}
}
