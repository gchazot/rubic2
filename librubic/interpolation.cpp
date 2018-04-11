/** \file interpolation.cpp
  * \brief, contient les définitions des fonctions d'interpolation utilisées pour les splines cubiques
  */
  
#include <cmath>

//	Fonctions d'interpolation en splines cubiques
double P0XX (const double X) { return ((-1.0/3.0)*pow(X,3.0) + (4.0/5.0)*pow(X,2.0) - (7.0/15.0)*X); }
double P1XX (const double X) { return (pow(X,3.0) - (9.0/5.0)*pow(X,2.0) - (1.0/5.0)*X + 1.0); }
double P2XX (const double X) { return (-pow(X,3.0) + (6.0/5.0)*pow(X,2.0) + (4.0/5.0)*X) ; }
double P3XX (const double X) { return ((1.0/3.0)*pow(X,3.0) - (1.0/5.0)*pow(X,2.0) - (2.0/15.0)*X) ; }

double P0YY (const double Y) { return ((-1.0/3.0)*pow(Y,3.0) + (4.0/5.0)*pow(Y,2.0) - (7.0/15.0)*Y); }
double P1YY (const double Y) { return (pow(Y,3.0) - (9.0/5.0)*pow(Y,2.0) - (1.0/5.0)*Y + 1.0); }
double P2YY (const double Y) { return (-pow(Y,3.0) + (6.0/5.0)*pow(Y,2.0) + (4.0/5.0)*Y) ; }
double P3YY (const double Y) { return ((1.0/3.0)*pow(Y,3.0) - (1.0/5.0)*pow(Y,2.0) - (2.0/15.0)*Y); }

double P0ZZ (const double Z) { return ((-1.0/3.0)*pow(Z,3.0) + (4.0/5.0)*pow(Z,2.0) - (7.0/15.0)*Z); }
double P1ZZ (const double Z) { return (pow(Z,3.0) - (9.0/5.0)*pow(Z,2.0) - (1.0/5.0)*Z + 1.0); }
double P2ZZ (const double Z) { return (-pow(Z,3.0) + (6.0/5.0)*pow(Z,2.0) + (4.0/5.0)*Z) ; }
double P3ZZ (const double Z) { return ((1.0/3.0)*pow(Z,3.0) - (1.0/5.0)*pow(Z,2.0) - (2.0/15.0)*Z) ; }

double derX_P0XX (const double X) { return (-pow(X,2.0) + (8.0/5.0)*X - 7.0/15.0) ; }		// dP0XX / dX
double derX_P1XX (const double X) { return (3.0*pow(X,2.0) - (18.0/5.0)*X - 1.0/5.0) ; }	// dP1XX / dX
double derX_P2XX (const double X) { return (-3.0*pow(X,2.0) + (12.0/5.0)*X + 4.0/5.0) ; }	// dP2XX / dX
double derX_P3XX (const double X) { return (pow(X,2.0) - (2.0/5.0)*X - 2.0/15.0) ; }		// dP3XX / dX

double derY_P0YY (const double Y) { return (-pow(Y,2.0) + (8.0/5.0)*Y - 7.0/15.0) ; }		// dP0YY / dY
double derY_P1YY (const double Y) { return (3.0*pow(Y,2.0) - (18.0/5.0)*Y - 1.0/5.0) ; }	// dP1YY / dY
double derY_P2YY (const double Y) { return (-3.0*pow(Y,2.0) + (12.0/5.0)*Y + 4.0/5.0) ; }	// dP2YY / dY
double derY_P3YY (const double Y) { return (pow(Y,2.0) - (2.0/5.0)*Y - 2.0/15.0) ; }		// dP3YY / dY

double derZ_P0ZZ (const double Z) { return (-pow(Z,2.0) + (8.0/5.0)*Z - 7.0/15.0) ; }		// dP0ZZ / dZ
double derZ_P1ZZ (const double Z) { return (3.0*pow(Z,2.0) - (18.0/5.0)*Z - 1.0/5.0) ; }	// dP1ZZ / dZ
double derZ_P2ZZ (const double Z) { return (-3.0*pow(Z,2.0) + (12.0/5.0)*Z + 4.0/5.0) ; }	// dP2ZZ / dZ
double derZ_P3ZZ (const double Z) { return (pow(Z,2.0) - (2.0/5.0)*Z - 2.0/15.0) ; }		// dP3ZZ / dZ

// dPixS / dP0XX
double derP0XX_PixS(unsigned char ***Data, double Y, double Z, long i, long j, long k)
{
	double der = P0ZZ(Z)*(P0YY(Y)*Data[i-1][j-1][k-1] + P1YY(Y)*Data[i-1][j][k-1]+ P2YY(Y)*Data[i-1][j+1][k-1]+ P3YY(Y)*Data[i-1][j+2][k-1])
	+ P1ZZ(Z)*(P0YY(Y)*Data[i-1][j-1][k] + P1YY(Y)*Data[i-1][j][k]+ P2YY(Y)*Data[i-1][j+1][k]+ P3YY(Y)*Data[i-1][j+2][k])
	+ P2ZZ(Z)*(P0YY(Y)*Data[i-1][j-1][k+1] + P1YY(Y)*Data[i-1][j][k+1]+ P2YY(Y)*Data[i-1][j+1][k+1]+ P3YY(Y)*Data[i-1][j+2][k+1])
	+ P3ZZ(Z)*(P0YY(Y)*Data[i-1][j-1][k+2] + P1YY(Y)*Data[i-1][j][k+2]+ P2YY(Y)*Data[i-1][j+1][k+2]+ P3YY(Y)*Data[i-1][j+2][k+2]);
	
	return der;
}

// dPixS / dP1XX
double derP1XX_PixS(unsigned char ***Data, double Y, double Z, long i, long j, long k)
{
	double der = P0ZZ(Z)*(P0YY(Y)*Data[i][j-1][k-1] + P1YY(Y)*Data[i][j][k-1]+ P2YY(Y)*Data[i][j+1][k-1]+ P3YY(Y)*Data[i][j+2][k-1])
	+ P1ZZ(Z)*(P0YY(Y)*Data[i][j-1][k] + P1YY(Y)*Data[i][j][k]+ P2YY(Y)*Data[i][j+1][k]+ P3YY(Y)*Data[i][j+2][k])
	+ P2ZZ(Z)*(P0YY(Y)*Data[i][j-1][k+1] + P1YY(Y)*Data[i][j][k+1]+ P2YY(Y)*Data[i][j+1][k+1]+ P3YY(Y)*Data[i][j+2][k+1])
	+ P3ZZ(Z)*(P0YY(Y)*Data[i][j-1][k+2] + P1YY(Y)*Data[i][j][k+2]+ P2YY(Y)*Data[i][j+1][k+2]+ P3YY(Y)*Data[i][j+2][k+2]);
	
	return der;
}

// dPixS / dP2XX
double derP2XX_PixS(unsigned char ***Data, double Y, double Z, long i, long j, long k)
{
	double der = P0ZZ(Z)*(P0YY(Y)*Data[i+1][j-1][k-1] + P1YY(Y)*Data[i+1][j][k-1]+ P2YY(Y)*Data[i+1][j+1][k-1]+ P3YY(Y)*Data[i+1][j+2][k-1])
	+ P1ZZ(Z)*(P0YY(Y)*Data[i+1][j-1][k] + P1YY(Y)*Data[i+1][j][k]+ P2YY(Y)*Data[i+1][j+1][k]+ P3YY(Y)*Data[i+1][j+2][k])
	+ P2ZZ(Z)*(P0YY(Y)*Data[i+1][j-1][k+1] + P1YY(Y)*Data[i+1][j][k+1]+ P2YY(Y)*Data[i+1][j+1][k+1]+ P3YY(Y)*Data[i+1][j+2][k+1])
	+ P3ZZ(Z)*(P0YY(Y)*Data[i+1][j-1][k+2] + P1YY(Y)*Data[i+1][j][k+2]+ P2YY(Y)*Data[i+1][j+1][k+2]+ P3YY(Y)*Data[i+1][j+2][k+2]);
	
	return der;
}

// dPixS / dP3XX
double derP3XX_PixS(unsigned char ***Data, double Y, double Z, long i, long j, long k)
{
	double der = P0ZZ(Z)*(P0YY(Y)*Data[i+2][j-1][k-1] + P1YY(Y)*Data[i+2][j][k-1]+ P2YY(Y)*Data[i+2][j+1][k-1]+ P3YY(Y)*Data[i+2][j+2][k-1])
	+ P1ZZ(Z)*(P0YY(Y)*Data[i+2][j-1][k] + P1YY(Y)*Data[i+2][j][k]+ P2YY(Y)*Data[i+2][j+1][k]+ P3YY(Y)*Data[i+2][j+2][k])
	+ P2ZZ(Z)*(P0YY(Y)*Data[i+2][j-1][k+1] + P1YY(Y)*Data[i+2][j][k+1]+ P2YY(Y)*Data[i+2][j+1][k+1]+ P3YY(Y)*Data[i+2][j+2][k+1])
	+ P3ZZ(Z)*(P0YY(Y)*Data[i+2][j-1][k+2] + P1YY(Y)*Data[i+2][j][k+2]+ P2YY(Y)*Data[i+2][j+1][k+2]+ P3YY(Y)*Data[i+2][j+2][k+2]);
	
	return der;
}

// dPixS / dP0YY
double derP0YY_PixS(unsigned char ***Data, double X, double Z, long i, long j, long k)
{
	double der = P0ZZ(Z)*(P0XX(X)*Data[i-1][j-1][k-1] + P1XX(X)*Data[i][j-1][k-1]+ P2XX(X)*Data[i+1][j-1][k-1]+ P3XX(X)*Data[i+2][j-1][k-1])
	+ P1ZZ(Z)*(P0XX(X)*Data[i-1][j-1][k] + P1XX(X)*Data[i][j-1][k]+ P2XX(X)*Data[i+1][j-1][k]+ P3XX(X)*Data[i+2][j-1][k])
	+ P2ZZ(Z)*(P0XX(X)*Data[i-1][j-1][k+1] + P1XX(X)*Data[i][j-1][k+1]+ P2XX(X)*Data[i+1][j-1][k+1]+ P3XX(X)*Data[i+2][j-1][k+1])
	+ P3ZZ(Z)*(P0XX(X)*Data[i-1][j-1][k+2] + P1XX(X)*Data[i][j-1][k+2]+ P2XX(X)*Data[i+1][j-1][k+2]+ P3XX(X)*Data[i+2][j-1][k+2]);
	
	return der;
}

// dPixS / dP1YY
double derP1YY_PixS(unsigned char ***Data, double X, double Z, long i, long j, long k)
{
	double der = P0ZZ(Z)*(P0XX(X)*Data[i-1][j][k-1] + P1XX(X)*Data[i][j][k-1]+ P2XX(X)*Data[i+1][j][k-1]+ P3XX(X)*Data[i+2][j][k-1])
	+ P1ZZ(Z)*(P0XX(X)*Data[i-1][j][k] + P1XX(X)*Data[i][j][k]+ P2XX(X)*Data[i+1][j][k]+ P3XX(X)*Data[i+2][j][k])
	+ P2ZZ(Z)*(P0XX(X)*Data[i-1][j][k+1] + P1XX(X)*Data[i][j][k+1]+ P2XX(X)*Data[i+1][j][k+1]+ P3XX(X)*Data[i+2][j][k+1])
	+ P3ZZ(Z)*(P0XX(X)*Data[i-1][j][k+2] + P1XX(X)*Data[i][j][k+2]+ P2XX(X)*Data[i+1][j][k+2]+ P3XX(X)*Data[i+2][j][k+2]);
	
	return der;
}

// dPixS / dP2YY
double derP2YY_PixS(unsigned char ***Data, double X, double Z, long i, long j, long k)
{
	double der = P0ZZ(Z)*(P0XX(X)*Data[i-1][j+1][k-1] + P1XX(X)*Data[i][j+1][k-1]+ P2XX(X)*Data[i+1][j+1][k-1]+ P3XX(X)*Data[i+2][j+1][k-1])
	+ P1ZZ(Z)*(P0XX(X)*Data[i-1][j+1][k] + P1XX(X)*Data[i][j+1][k]+ P2XX(X)*Data[i+1][j+1][k]+ P3XX(X)*Data[i+2][j+1][k])
	+ P2ZZ(Z)*(P0XX(X)*Data[i-1][j+1][k+1] + P1XX(X)*Data[i][j+1][k+1]+ P2XX(X)*Data[i+1][j+1][k+1]+ P3XX(X)*Data[i+2][j+1][k+1])
	+ P3ZZ(Z)*(P0XX(X)*Data[i-1][j+1][k+2] + P1XX(X)*Data[i][j+1][k+2]+ P2XX(X)*Data[i+1][j+1][k+2]+ P3XX(X)*Data[i+2][j+1][k+2]);
	
	return der;
}

// dPixS / dP3YY
double derP3YY_PixS(unsigned char ***Data, double X, double Z, long i, long j, long k)
{
	double der = P0ZZ(Z)*(P0XX(X)*Data[i-1][j+2][k-1] + P1XX(X)*Data[i][j+2][k-1]+ P2XX(X)*Data[i+1][j+2][k-1]+ P3XX(X)*Data[i+2][j+2][k-1])
	+ P1ZZ(Z)*(P0XX(X)*Data[i-1][j+2][k] + P1XX(X)*Data[i][j+2][k]+ P2XX(X)*Data[i+1][j+2][k]+ P3XX(X)*Data[i+2][j+2][k])
	+ P2ZZ(Z)*(P0XX(X)*Data[i-1][j+2][k+1] + P1XX(X)*Data[i][j+2][k+1]+ P2XX(X)*Data[i+1][j+2][k+1]+ P3XX(X)*Data[i+2][j+2][k+1])
	+ P3ZZ(Z)*(P0XX(X)*Data[i-1][j+2][k+2] + P1XX(X)*Data[i][j+2][k+2]+ P2XX(X)*Data[i+1][j+2][k+2]+ P3XX(X)*Data[i+2][j+2][k+2]);
	
	return der;
}

// dPixS / dP0ZZ
double derP0ZZ_PixS(unsigned char ***Data, double X, double Y, long i, long j, long k)
{
	double der = P0YY(Y)*(P0XX(X)*Data[i-1][j-1][k-1] + P1XX(X)*Data[i][j-1][k-1]+ P2XX(X)*Data[i+1][j-1][k-1]+ P3XX(X)*Data[i+2][j-1][k-1])
	+ P1YY(Y)*(P0XX(X)*Data[i-1][j][k-1] + P1XX(X)*Data[i][j][k-1]+ P2XX(X)*Data[i+1][j][k-1]+ P3XX(X)*Data[i+2][j][k-1])
	+ P2YY(Y)*(P0XX(X)*Data[i-1][j+1][k-1] + P1XX(X)*Data[i][j+1][k-1]+ P2XX(X)*Data[i+1][j+1][k-1]+ P3XX(X)*Data[i+2][j+1][k-1])
	+ P3YY(Y)*(P0XX(X)*Data[i-1][j+2][k-1] + P1XX(X)*Data[i][j+2][k-1]+ P2XX(X)*Data[i+1][j+2][k-1]+ P3XX(X)*Data[i+2][j+2][k-1]);
	
	return der;
}

// dPixS / dP1ZZ
double derP1ZZ_PixS(unsigned char ***Data, double X, double Y, long i, long j, long k)
{
	double der = P0YY(Y)*(P0XX(X)*Data[i-1][j-1][k] + P1XX(X)*Data[i][j-1][k]+ P2XX(X)*Data[i+1][j-1][k]+ P3XX(X)*Data[i+2][j-1][k])
	+ P1YY(Y)*(P0XX(X)*Data[i-1][j][k] + P1XX(X)*Data[i][j][k]+ P2XX(X)*Data[i+1][j][k]+ P3XX(X)*Data[i+2][j][k])
	+ P2YY(Y)*(P0XX(X)*Data[i-1][j+1][k] + P1XX(X)*Data[i][j+1][k]+ P2XX(X)*Data[i+1][j+1][k]+ P3XX(X)*Data[i+2][j+1][k])
	+ P3YY(Y)*(P0XX(X)*Data[i-1][j+2][k] + P1XX(X)*Data[i][j+2][k]+ P2XX(X)*Data[i+1][j+2][k]+ P3XX(X)*Data[i+2][j+2][k]);
	
	return der;
}

// dPixS / dP2ZZ
double derP2ZZ_PixS(unsigned char ***Data, double X, double Y, long i, long j, long k)
{
	double der = P0YY(Y)*(P0XX(X)*Data[i-1][j-1][k+1] + P1XX(X)*Data[i][j-1][k+1]+ P2XX(X)*Data[i+1][j-1][k+1]+ P3XX(X)*Data[i+2][j-1][k+1])
	+ P1YY(Y)*(P0XX(X)*Data[i-1][j][k+1] + P1XX(X)*Data[i][j][k+1]+ P2XX(X)*Data[i+1][j][k+1]+ P3XX(X)*Data[i+2][j][k+1])
	+ P2YY(Y)*(P0XX(X)*Data[i-1][j+1][k+1] + P1XX(X)*Data[i][j+1][k+1]+ P2XX(X)*Data[i+1][j+1][k+1]+ P3XX(X)*Data[i+2][j+1][k+1])
	+ P3YY(Y)*(P0XX(X)*Data[i-1][j+2][k+1] + P1XX(X)*Data[i][j+2][k+1]+ P2XX(X)*Data[i+1][j+2][k+1]+ P3XX(X)*Data[i+2][j+2][k+1]);
	
	return der;
}

// dPixS / dP3ZZ
double derP3ZZ_PixS(unsigned char ***Data, double X, double Y, long i, long j, long k)
{
	double der = P0YY(Y)*(P0XX(X)*Data[i-1][j-1][k+2] + P1XX(X)*Data[i][j-1][k+2]+ P2XX(X)*Data[i+1][j-1][k+2]+ P3XX(X)*Data[i+2][j-1][k+2])
	+ P1YY(Y)*(P0XX(X)*Data[i-1][j][k+2] + P1XX(X)*Data[i][j][k+2]+ P2XX(X)*Data[i+1][j][k+2]+ P3XX(X)*Data[i+2][j][k+2])
	+ P2YY(Y)*(P0XX(X)*Data[i-1][j+1][k+2] + P1XX(X)*Data[i][j+1][k+2]+ P2XX(X)*Data[i+1][j+1][k+2]+ P3XX(X)*Data[i+2][j+1][k+2])
	+ P3YY(Y)*(P0XX(X)*Data[i-1][j+2][k+2] + P1XX(X)*Data[i][j+2][k+2]+ P2XX(X)*Data[i+1][j+2][k+2]+ P3XX(X)*Data[i+2][j+2][k+2]);
	
	return der;
}
