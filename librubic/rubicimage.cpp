/** \file rubicimage.cpp
  * \brief Contient les définitions des fonctions membres de RubicImage
  */

#include "rubicimage.h"
#include "dvect.h"
#include "lvect.h"
#include "dvect24.h"

#include <cstdlib>
#include <iostream>
using namespace std;

// Constructeur avec création de l'image selon les dimensions séparées
RubicImage::RubicImage(long xSize, long ySize, long zSize)
{
	mXSize = xSize;
	mYSize = ySize;
	mZSize = zSize;
	mValid = true;
	
	mNGMin = 1u;
	mNGMax = 255u;
	
	mData = new unsigned char**[xSize];
	if(mData == NULL)
	{
		mXSize = 0;
		mYSize = 0;
		mZSize = 0;
		mValid = false;
		return;
	}
	
	for(long i=0; i<mXSize; i++)
	{
		mData[i] = new unsigned char*[mYSize];
		if(mData[i] == NULL)
		{
			mValid = false;
			return;
		}
		for(long j=0; j<mYSize; j++)
		{
			mData[i][j] = new unsigned char[mZSize];
			if(mData[i][j] == NULL)
			{
				mValid = false;
				return;
			}
			for(long k=0; k<mZSize; k++)
			{
				mData[i][j][k] = 0u;
			}
		}
	}
}

// Constructeur avec création de l'image selon les dimensions en LVect
RubicImage::RubicImage(LVect& size)
{
	mXSize = size.getX();
	mYSize = size.getY();
	mZSize = size.getZ();
	mValid = true;
	
	mNGMin = 1u;
	mNGMax = 255u;
	
	mData = new unsigned char**[mXSize];
	if(mData == NULL)
	{
		mXSize = 0;
		mYSize = 0;
		mZSize = 0;
		mValid = false;
		return;
	}
	
	for(long i=0; i<mXSize; i++)
	{
		mData[i] = new unsigned char*[mYSize];
		if(mData[i] == NULL)
		{
			mValid = false;
			return;
		}
		for(long j=0; j<mYSize; j++)
		{
			mData[i][j] = new unsigned char[mZSize];
			if(mData[i][j] == NULL)
			{
				mValid = false;
				return;
			}
			for(long k=0; k<mZSize; k++)
			{
				mData[i][j][k] = 0u;
			}
		}
	}
}

// Destructeur
RubicImage::~RubicImage()
{
	if(mData != NULL)
	{
		for(long i=0; i<mXSize; i++)
		{
			if(mData[i] != NULL)
			{
				for(long j=0; j<mYSize; j++)
				{
					if(mData[i][j] != NULL)
					{
						delete [] mData[i][j];
					}
				}
				delete [] mData[i];
			}
		}
		delete [] mData;
	}
}

bool RubicImage::isValid(void)
{
	return mValid;
}

// Gestion de la taille de l'image
long RubicImage::getXSize(void) const
{
	return mXSize;
}

long RubicImage::getYSize(void) const
{
	return mYSize;
}

long RubicImage::getZSize(void) const
{
	return mZSize;
}

LVect RubicImage::getSize(void) const
{
	return LVect(mXSize, mYSize, mZSize);
}

// Modification du niveau de gris minimal à prendre en compte pour l'interpolation
void RubicImage::setNGMin(const unsigned char val)
{
	mNGMin = val;
}

// Modification du niveau de gris maximal à prendre en compte pour l'interpolation
void RubicImage::setNGMax(const unsigned char val)
{
	mNGMax = val;
}
	
// Gestion des données dans l'image
inline unsigned char RubicImage::getData(long xpos, long ypos, long zpos) const
{
// 	unsigned long result = 0;
	if( mValid && (xpos < mXSize) && (ypos < mYSize) && (zpos < mZSize) 
		&& (xpos >= 0) && (ypos >=0) && (zpos >= 0) )
	{
// 		result += (unsigned long)mData[xpos][ypos][zpos][0];
// 		result += (unsigned long)mData[xpos][ypos][zpos][1];
// 		result += (unsigned long)mData[xpos][ypos][zpos][2];
// 		if( (result % 3) != 2 )
// 			result = result / 3;
// 		else
// 			result = result / 3 + 1;
		return mData[xpos][ypos][zpos];
	}
	else
		return 0u;
// 	return (unsigned char)result;
}

inline unsigned char RubicImage::getData(LVect& pos) const
{
	return getData( pos.getX(), pos.getY(), pos.getZ() );
}

inline void RubicImage::setData(unsigned char data, long xpos, long ypos, long zpos)
{
	if( mValid && (xpos <= mXSize) && (ypos <= mYSize) && (zpos <= mZSize)
		&& (xpos >= 0) && (ypos >=0) && (zpos >= 0) )
	{
// 		mData[xpos][ypos][zpos][0] = data;
// 		mData[xpos][ypos][zpos][1] = data;
// 		mData[xpos][ypos][zpos][2] = data;
		mData[xpos][ypos][zpos] = data;
	}
}

inline void RubicImage::setData(unsigned char data, LVect& pos)
{
	setData( data, pos.getX(), pos.getY(), pos.getZ() );
}

// Calcule la valeur interpolée au point donné en paramètre
// La valeur est arrondie à l'entier le plus proche
inline double RubicImage::interpoleLineaire(double xpos, double ypos, double zpos)
{
	if( !mValid )
		return 0.0;

	// Partie entière des coordonnées
	long xinf = (long) xpos;
	long yinf = (long) ypos;
	long zinf = (long) zpos;

	// Coordonnées suivantes
	long xsup = xinf + 1;
	long ysup = yinf + 1;
	long zsup = zinf + 1;
	
	// On vérifie que l'on reste dans l'image
	if( (xinf < 0) || (yinf < 0) || (zinf < 0)
		|| (xsup > mXSize-1) || (ysup > mYSize-1) || (zsup > mZSize-1) )
	{
// 		cout << "L'interpolation sort de l'image" << endl;
		return 0.0;
	}

	// On vérifie que toutes les valeurs d'interpolation sont valides
	if( (getData( xinf, yinf, zinf) < mNGMin) || (getData( xinf, yinf, zinf) > mNGMax)
		|| (getData( xsup, yinf, zinf) < mNGMin) || (getData( xsup, yinf, zinf) > mNGMax)
		|| (getData( xinf, ysup, zinf) < mNGMin) || (getData( xinf, ysup, zinf) > mNGMax)
		|| (getData( xinf, yinf, zsup) < mNGMin) || (getData( xinf, yinf, zsup) > mNGMax)
		|| (getData( xsup, ysup, zinf) < mNGMin) || (getData( xsup, ysup, zinf) > mNGMax)
		|| (getData( xsup, yinf, zsup) < mNGMin) || (getData( xsup, yinf, zsup) > mNGMax)
		|| (getData( xinf, ysup, zsup) < mNGMin) || (getData( xinf, ysup, zsup) > mNGMax)
		|| (getData( xsup, ysup, zsup) < mNGMin) || (getData( xsup, ysup, zsup) > mNGMax)
		)
	{
// 		cout << "Au moins une valeur d'interpolation est invalide" << endl;
		return 0.0;
	}
	
	// Parties fractionaires des coordonnées
	double xfrac = xpos - (double) xinf;
	double yfrac = ypos - (double) yinf;
	double zfrac = zpos - (double) zinf;
	
	// Résultat de l'interpolation
	double result;
	
	// Coefficients d'interpolation
	double Alpha1, Alpha2, Alpha3;
	double Beta1, Beta2, Beta3;
	double Gamma, Lambda;
	
	// Calcul des coefficients d'interpolation
	Alpha1 =  (double)getData( xsup, yinf, zinf) - (double)getData( xinf, yinf, zinf);
	Alpha2 =  (double)getData( xinf, ysup, zinf) - (double)getData( xinf, yinf, zinf);
	Alpha3 =  (double)getData( xinf, yinf, zsup) - (double)getData( xinf, yinf, zinf);
	
	Beta1  =  (double)getData( xsup, ysup, zinf) + (double)getData( xinf, yinf, zinf)
		- (double)getData( xsup, yinf, zinf) - (double)getData( xinf, ysup, zinf);
	Beta2 =   (double)getData( xsup, yinf, zsup) + (double)getData( xinf, yinf, zinf)
		- (double)getData( xsup, yinf, zinf) - (double)getData( xinf, yinf, zsup);
	Beta3 =   (double)getData( xinf, ysup, zsup) + (double)getData( xinf, yinf, zinf)
		- (double)getData( xinf, ysup, zinf) - (double)getData( xinf, yinf, zsup);
	
	Gamma =   (double)getData( xsup, ysup, zsup) + (double)getData( xsup, yinf, zinf)
		+ (double)getData( xinf, ysup, zinf) + (double)getData( xinf, yinf, zsup)
		- (double)getData( xinf, yinf, zinf) - (double)getData( xsup, ysup, zinf)
		- (double)getData( xsup, yinf, zsup) - (double)getData( xinf, ysup, zsup);
		
	Lambda =  (double)getData( xinf, yinf, zinf);
	
	// Calcul de la valeur interpolée
	result = Alpha1 * xfrac + Alpha2 * yfrac + Alpha3 * zfrac + Beta1 * xfrac * yfrac 
	+ Beta2 * xfrac * zfrac + Beta3 * yfrac * zfrac + Gamma * xfrac * yfrac * zfrac + Lambda;
	
	if( result <= 0.0 )
	{
// 		cout << "interpolation négative" << endl;
		return 0.0;
	}
	if( result >= 255.0 )
	{
// 		cout << "interpolation > à 255" << endl;
		return 255.0;
	}

	return result;
}

inline double RubicImage::interpoleLineaire(DVect& pos)
{
	return interpoleLineaire(pos.getX(), pos.getY(), pos.getZ());
}

//	Déclaration des Fonctions d'interpolation en splines cubiques
// 	Voir interpolation.cpp pour les définitions.
double P0XX (const double X);
double P1XX (const double X);
double P2XX (const double X);
double P3XX (const double X);

double P0YY (const double Y);
double P1YY (const double Y);
double P2YY (const double Y);
double P3YY (const double Y);

double P0ZZ (const double Z);
double P1ZZ (const double Z);
double P2ZZ (const double Z);
double P3ZZ (const double Z);

double derX_P0XX (const double X);	// dP0XX / dX
double derX_P1XX (const double X);	// dP1XX / dX
double derX_P2XX (const double X);	// dP2XX / dX
double derX_P3XX (const double X);	// dP3XX / dX

double derY_P0YY (const double Y);	// dP0YY / dY
double derY_P1YY (const double Y);	// dP1YY / dY
double derY_P2YY (const double Y);	// dP2YY / dY
double derY_P3YY (const double Y);	// dP3YY / dY

double derZ_P0ZZ (const double Z);	// dP0ZZ / dZ
double derZ_P1ZZ (const double Z);	// dP1ZZ / dZ
double derZ_P2ZZ (const double Z);	// dP2ZZ / dZ
double derZ_P3ZZ (const double Z);	// dP3ZZ / dZ


double derP0XX_PixS(unsigned char ***mData, double Y, double Z, long i, long j, long k);	// dPixS / dP0XX
double derP1XX_PixS(unsigned char ***mData, double Y, double Z, long i, long j, long k);	// dPixS / dP1XX
double derP2XX_PixS(unsigned char ***mData, double Y, double Z, long i, long j, long k);	// dPixS / dP2XX
double derP3XX_PixS(unsigned char ***mData, double Y, double Z, long i, long j, long k);	// dPixS / dP3XX

double derP0YY_PixS(unsigned char ***mData, double Y, double Z, long i, long j, long k);	// dPixS / dP0YY
double derP1YY_PixS(unsigned char ***mData, double Y, double Z, long i, long j, long k);	// dPixS / dP1YY
double derP2YY_PixS(unsigned char ***mData, double Y, double Z, long i, long j, long k);	// dPixS / dP2YY
double derP3YY_PixS(unsigned char ***mData, double Y, double Z, long i, long j, long k);	// dPixS / dP3YY

double derP0ZZ_PixS(unsigned char ***mData, double Y, double Z, long i, long j, long k);	// dPixS / dP0ZZ
double derP1ZZ_PixS(unsigned char ***mData, double Y, double Z, long i, long j, long k);	// dPixS / dP1ZZ
double derP2ZZ_PixS(unsigned char ***mData, double Y, double Z, long i, long j, long k);	// dPixS / dP2ZZ
double derP3ZZ_PixS(unsigned char ***mData, double Y, double Z, long i, long j, long k);	// dPixS / dP3ZZ

inline double RubicImage::interpoleCubique(double xpos, double ypos, double zpos)
{
	if( !mValid )
	{
		cout << "Problème, image non valide" << endl;
		return 0.0;
	}
	
	DVect coordvv(xpos, ypos, zpos);
	LVect Dep_000 = (LVect)(coordvv);
	DVect coord_cur = coordvv - (DVect)Dep_000;
	
	long Dep000_X = Dep_000.getX();
	long Dep000_Y = Dep_000.getY();
	long Dep000_Z = Dep_000.getZ();

	double coord_cur_X = coord_cur.getX();
	double coord_cur_Y = coord_cur.getY();
	double coord_cur_Z = coord_cur.getZ();
	
	// On vérifie que l'on reste dans l'image
	if(	(Dep_000.getMin() < 1) || (Dep_000.getX() >= mXSize-2)
		|| (Dep_000.getY() >= mYSize-2) || (Dep_000.getZ() >= mZSize-2) )
	{
// 		cout << "L'interpolation sort de l'image" << endl;
		return 0.0;
	}

	// On vérifie que toutes les valeurs d'interpolation sont valides
	for(short i = -1; i <= 2; i++)
		for(short j = -1; j <= 2; j++)
			for(short k = -1; k <= 2; k++)
				if( (mData[Dep000_X + i][Dep000_Y + j][Dep000_Z + j] < mNGMin)
				 || (mData[Dep000_X + i][Dep000_Y + j][Dep000_Z + j] > mNGMax) )
				 {
// 				 	cout << "Une valeur d'interpolation est invalide (";
// 				 	cout << (int)mNGMin << "/" << (int)mNGMax << ")" << endl;
				 	return 0.0;
				 }

	
	// Calcul de la valeur interpolée
	double result = P0XX(coord_cur_X) * (P0YY(coord_cur_Y) * (P0ZZ(coord_cur_Z) * mData[Dep000_X - 1][Dep000_Y - 1][Dep000_Z - 1] + P1ZZ(coord_cur_Z) * mData[Dep000_X - 1][Dep000_Y - 1][Dep000_Z] + P2ZZ(coord_cur_Z) * mData[Dep000_X - 1][Dep000_Y - 1][Dep000_Z + 1] + P3ZZ(coord_cur_Z) * mData[Dep000_X - 1][Dep000_Y - 1][Dep000_Z + 2])
	+ P1YY(coord_cur_Y) * (P0ZZ(coord_cur_Z) * mData[Dep000_X - 1][Dep000_Y][Dep000_Z - 1] + P1ZZ(coord_cur_Z) * mData[Dep000_X - 1][Dep000_Y][Dep000_Z] + P2ZZ(coord_cur_Z) * mData[Dep000_X - 1][Dep000_Y][Dep000_Z + 1] + P3ZZ(coord_cur_Z) * mData[Dep000_X - 1][Dep000_Y][Dep000_Z + 2])
	+P2YY(coord_cur_Y) * (P0ZZ(coord_cur_Z) * mData[Dep000_X - 1][Dep000_Y + 1][Dep000_Z - 1] + P1ZZ(coord_cur_Z) * mData[Dep000_X - 1][Dep000_Y + 1][Dep000_Z] + P2ZZ(coord_cur_Z) * mData[Dep000_X - 1][Dep000_Y + 1][Dep000_Z + 1] + P3ZZ(coord_cur_Z) * mData[Dep000_X - 1][Dep000_Y + 1][Dep000_Z + 2])
	+ P3YY(coord_cur_Y) * (P0ZZ(coord_cur_Z) * mData[Dep000_X - 1][Dep000_Y + 2][Dep000_Z - 1] + P1ZZ(coord_cur_Z) * mData[Dep000_X - 1][Dep000_Y + 2][Dep000_Z] + P2ZZ(coord_cur_Z) * mData[Dep000_X - 1][Dep000_Y + 2][Dep000_Z + 1] + P3ZZ(coord_cur_Z) * mData[Dep000_X - 1][Dep000_Y + 2][Dep000_Z + 2]))
	
	+ P1XX(coord_cur_X) * (P0YY(coord_cur_Y) * (P0ZZ(coord_cur_Z) * mData[Dep000_X][Dep000_Y - 1][Dep000_Z - 1] + P1ZZ(coord_cur_Z) * mData[Dep000_X][Dep000_Y - 1][Dep000_Z] + P2ZZ(coord_cur_Z) * mData[Dep000_X][Dep000_Y - 1][Dep000_Z + 1] + P3ZZ(coord_cur_Z) * mData[Dep000_X][Dep000_Y - 1][Dep000_Z + 2])
	+ P1YY(coord_cur_Y) * (P0ZZ(coord_cur_Z) * mData[Dep000_X][Dep000_Y][Dep000_Z - 1] + P1ZZ(coord_cur_Z) * mData[Dep000_X][Dep000_Y][Dep000_Z] + P2ZZ(coord_cur_Z) * mData[Dep000_X][Dep000_Y][Dep000_Z + 1] + P3ZZ(coord_cur_Z) * mData[Dep000_X][Dep000_Y][Dep000_Z + 2])
	+P2YY(coord_cur_Y) * (P0ZZ(coord_cur_Z) * mData[Dep000_X][Dep000_Y + 1][Dep000_Z - 1] + P1ZZ(coord_cur_Z) * mData[Dep000_X][Dep000_Y + 1][Dep000_Z] + P2ZZ(coord_cur_Z) * mData[Dep000_X][Dep000_Y + 1][Dep000_Z + 1] + P3ZZ(coord_cur_Z) * mData[Dep000_X][Dep000_Y + 1][Dep000_Z + 2])
	+ P3YY(coord_cur_Y) * (P0ZZ(coord_cur_Z) * mData[Dep000_X][Dep000_Y + 2][Dep000_Z - 1] + P1ZZ(coord_cur_Z) * mData[Dep000_X][Dep000_Y + 2][Dep000_Z] + P2ZZ(coord_cur_Z) * mData[Dep000_X][Dep000_Y + 2][Dep000_Z + 1] + P3ZZ(coord_cur_Z) * mData[Dep000_X][Dep000_Y + 2][Dep000_Z + 2]))
	
	+ P2XX(coord_cur_X) * (P0YY(coord_cur_Y) * (P0ZZ(coord_cur_Z) * mData[Dep000_X + 1][Dep000_Y - 1][Dep000_Z - 1] + P1ZZ(coord_cur_Z) * mData[Dep000_X + 1][Dep000_Y - 1][Dep000_Z] + P2ZZ(coord_cur_Z) * mData[Dep000_X + 1][Dep000_Y - 1][Dep000_Z + 1] + P3ZZ(coord_cur_Z) * mData[Dep000_X + 1][Dep000_Y - 1][Dep000_Z + 2])
	+ P1YY(coord_cur_Y) * (P0ZZ(coord_cur_Z) * mData[Dep000_X + 1][Dep000_Y][Dep000_Z - 1] + P1ZZ(coord_cur_Z) * mData[Dep000_X + 1][Dep000_Y][Dep000_Z] + P2ZZ(coord_cur_Z) * mData[Dep000_X + 1][Dep000_Y][Dep000_Z + 1] + P3ZZ(coord_cur_Z) * mData[Dep000_X + 1][Dep000_Y][Dep000_Z + 2])
	+P2YY(coord_cur_Y) * (P0ZZ(coord_cur_Z) * mData[Dep000_X + 1][Dep000_Y + 1][Dep000_Z - 1] + P1ZZ(coord_cur_Z) * mData[Dep000_X + 1][Dep000_Y + 1][Dep000_Z] + P2ZZ(coord_cur_Z) * mData[Dep000_X + 1][Dep000_Y + 1][Dep000_Z + 1] + P3ZZ(coord_cur_Z) * mData[Dep000_X + 1][Dep000_Y + 1][Dep000_Z + 2])
	+ P3YY(coord_cur_Y) * (P0ZZ(coord_cur_Z) * mData[Dep000_X + 1][Dep000_Y + 2][Dep000_Z - 1] + P1ZZ(coord_cur_Z) * mData[Dep000_X + 1][Dep000_Y + 2][Dep000_Z] + P2ZZ(coord_cur_Z) * mData[Dep000_X + 1][Dep000_Y + 2][Dep000_Z + 1] + P3ZZ(coord_cur_Z) * mData[Dep000_X + 1][Dep000_Y + 2][Dep000_Z + 2]))
	
	+ P3XX(coord_cur_X) * (P0YY(coord_cur_Y) * (P0ZZ(coord_cur_Z) * mData[Dep000_X + 2][Dep000_Y - 1][Dep000_Z - 1] + P1ZZ(coord_cur_Z) * mData[Dep000_X + 2][Dep000_Y	- 1][Dep000_Z] + P2ZZ(coord_cur_Z) * mData[Dep000_X + 2][Dep000_Y - 1][Dep000_Z + 1] + P3ZZ(coord_cur_Z) * mData[Dep000_X + 2][Dep000_Y - 1][Dep000_Z + 2])
	+ P1YY(coord_cur_Y) * (P0ZZ(coord_cur_Z) * mData[Dep000_X + 2][Dep000_Y][Dep000_Z - 1] + P1ZZ(coord_cur_Z) * mData[Dep000_X + 2][Dep000_Y][Dep000_Z] + P2ZZ(coord_cur_Z) * mData[Dep000_X + 2][Dep000_Y][Dep000_Z + 1] + P3ZZ(coord_cur_Z) * mData[Dep000_X + 2][Dep000_Y][Dep000_Z + 2])
	+P2YY(coord_cur_Y) * (P0ZZ(coord_cur_Z) * mData[Dep000_X + 2][Dep000_Y + 1][Dep000_Z - 1] + P1ZZ(coord_cur_Z) * mData[Dep000_X + 2][Dep000_Y + 1][Dep000_Z] + P2ZZ(coord_cur_Z) * mData[Dep000_X + 2][Dep000_Y + 1][Dep000_Z + 1] + P3ZZ(coord_cur_Z) * mData[Dep000_X + 2][Dep000_Y + 1][Dep000_Z + 2])
	+ P3YY(coord_cur_Y) * (P0ZZ(coord_cur_Z) * mData[Dep000_X + 2][Dep000_Y + 2][Dep000_Z - 1] + P1ZZ(coord_cur_Z) * mData[Dep000_X + 2][Dep000_Y + 2][Dep000_Z] + P2ZZ(coord_cur_Z) * mData[Dep000_X + 2][Dep000_Y + 2][Dep000_Z + 1] + P3ZZ(coord_cur_Z) * mData[Dep000_X + 2][Dep000_Y + 2][Dep000_Z + 2]));
	
	// On vérifie que l'interpolation n'est pas absurde
	if( result < 0.0 )
	{
// 		cout << "interpolation négative" << endl;
		return 0.0;
	}
	if( result > 255.0 )
	{
// 		cout << "interpolation > à 255" << endl;
		return 255.0;
	}
	
// 	cout << "interpolation OK" << endl;
	return result;
}

inline double RubicImage::interpoleCubique(DVect& pos)
{
	return interpoleCubique(pos.getX(), pos.getY(), pos.getZ());
}

inline bool RubicImage::deriveCubique(double xpos, double ypos, double zpos, double Ip, double Jp, double Kp, double Lcur, DVect24* result)
{
	if( !mValid )
		return false;
	
	DVect24 dai_X;
	DVect24 dai_Y;
	DVect24 dai_Z;

	DVect coordvv(xpos, ypos, zpos);
	LVect Dep_000 = (LVect)(coordvv);
	DVect coord_cur = coordvv - (DVect)Dep_000;
	
	long Dep000_X = Dep_000.getX();
	long Dep000_Y = Dep_000.getY();
	long Dep000_Z = Dep_000.getZ();

	double coord_cur_X = coord_cur.getX();
	double coord_cur_Y = coord_cur.getY();
	double coord_cur_Z = coord_cur.getZ();
	
	// On vérifie que l'on nous a fourni un DVect24 valide
	if(result == NULL)
		return false;
		
	// On vérifie que l'on reste dans l'image
	if(	(Dep_000.getMin() < 1) || (Dep000_X >= mXSize-2)
		|| (Dep000_Y >= mYSize-2) || (Dep000_Z >= mZSize-2) )
	{
// 		cout << "L'interpolation sort de l'image" << endl;
		return false;
	}

	// On vérifie que toutes les valeurs d'interpolation sont valides
	for(short i = -1; i <= 2; i++)
		for(short j = -1; j <= 2; j++)
			for(short k = -1; k <= 2; k++)
				if( (mData[Dep000_X + i][Dep000_Y + j][Dep000_Z + j] < mNGMin)
				 || (mData[Dep000_X + i][Dep000_Y + j][Dep000_Z + j] > mNGMax) )
				 	return false;
				 	

// 	dai_X[0] = 1.0;
// 	dai_X[1] = 0.0;
// 	dai_X[2] = 0.0;
// 	dai_X[3] = coord_cur_X;
// 	dai_X[4] = 0.0;
// 	dai_X[5] = 0.0;
// 	dai_X[6] = coord_cur_Y;
// 	dai_X[7] = 0.0;
// 	dai_X[8] = 0.0;
// 	dai_X[9] = coord_cur_Z;
// 	dai_X[10] = 0.0;
// 	dai_X[11] = 0.0;
// 	dai_X[12] = coord_cur_X*coord_cur_Y;
// 	dai_X[13] = 0.0;
// 	dai_X[14] = 0.0;
// 	dai_X[15] = coord_cur_X*coord_cur_Z;
// 	dai_X[16] = 0.0;
// 	dai_X[17] = 0.0;
// 	dai_X[18] = coord_cur_Y*coord_cur_Z;
// 	dai_X[19] = 0.0;
// 	dai_X[20] = 0.0;
// 	dai_X[21] = coord_cur_X*coord_cur_Y*coord_cur_Z;
// 	dai_X[22] = 0.0;
// 	dai_X[23] = 0.0;
// 	
// 	dai_Y[0] = 0.0;
// 	dai_Y[1] = 1.0;
// 	dai_Y[2] = 0.0;
// 	dai_Y[3] = 0.0;
// 	dai_Y[4] = coord_cur_X;
// 	dai_Y[5] = 0.0;
// 	dai_Y[6] = 0.0;
// 	dai_Y[7] =  coord_cur_Y;
// 	dai_Y[8] = 0.0;
// 	dai_Y[9] = 0.0;
// 	dai_Y[10] = coord_cur_Z;
// 	dai_Y[11] = 0.0;
// 	dai_Y[12] = 0.0;
// 	dai_Y[13] = coord_cur_X*coord_cur_Y;
// 	dai_Y[14] = 0.0;
// 	dai_Y[15] = 0.0;
// 	dai_Y[16] = coord_cur_X*coord_cur_Z;
// 	dai_Y[17] = 0.0;
// 	dai_Y[18] = 0.0;
// 	dai_Y[19] = coord_cur_Y*coord_cur_Z;
// 	dai_Y[20] = 0.0;
// 	dai_Y[21] = 0.0;
// 	dai_Y[22] = coord_cur_X*coord_cur_Y*coord_cur_Z;
// 	dai_Y[23] = 0.0;
// 	
// 	dai_Z[0] = 0.0;
// 	dai_Z[1] = 0.0;
// 	dai_Z[2] = 1.0;
// 	dai_Z[3] = 0.0;
// 	dai_Z[4] = 0.0;
// 	dai_Z[5] = coord_cur_X;
// 	dai_Z[6] = 0.0;
// 	dai_Z[7] = 0.0;
// 	dai_Z[8] = coord_cur_Y;
// 	dai_Z[9] = 0.0;
// 	dai_Z[10] = 0.0;
// 	dai_Z[11] = coord_cur_Z;
// 	dai_Z[12] = 0.0;
// 	dai_Z[13] = 0.0;
// 	dai_Z[14] = coord_cur_X*coord_cur_Y;
// 	dai_Z[15] = 0.0;
// 	dai_Z[16] = 0.0;
// 	dai_Z[17] = coord_cur_X*coord_cur_Z;
// 	dai_Z[18] = 0.0;
// 	dai_Z[19] = 0.0;
// 	dai_Z[20] = coord_cur_Y*coord_cur_Z;
// 	dai_Z[21] = 0.0;
// 	dai_Z[22] = 0.0;
// 	dai_Z[23] = coord_cur_X*coord_cur_Y*coord_cur_Z;


	dai_X[0] = 1.0;
	dai_X[1] = 0.0;
	dai_X[2] = 0.0;
	dai_X[3] = Ip/Lcur;
	dai_X[4] = 0.0;
	dai_X[5] = 0.0;
	dai_X[6] = Jp/Lcur;
	dai_X[7] = 0.0;
	dai_X[8] = 0.0;
	dai_X[9] = Kp/Lcur;
	dai_X[10] = 0.0;
	dai_X[11] = 0.0;
	dai_X[12] = Ip/Lcur*Jp/Lcur;
	dai_X[13] = 0.0;
	dai_X[14] = 0.0;
	dai_X[15] = Ip/Lcur*Kp/Lcur;
	dai_X[16] = 0.0;
	dai_X[17] = 0.0;
	dai_X[18] = Jp/Lcur*Kp/Lcur;
	dai_X[19] = 0.0;
	dai_X[20] = 0.0;
	dai_X[21] = Ip/Lcur*Jp/Lcur*Kp/Lcur;
	dai_X[22] = 0.0;
	dai_X[23] = 0.0;
	
	dai_Y[0] = 0.0;
	dai_Y[1] = 1.0;
	dai_Y[2] = 0.0;
	dai_Y[3] = 0.0;
	dai_Y[4] = Ip/Lcur;
	dai_Y[5] = 0.0;
	dai_Y[6] = 0.0;
	dai_Y[7] =  Jp/Lcur;
	dai_Y[8] = 0.0;
	dai_Y[9] = 0.0;
	dai_Y[10] = Kp/Lcur;
	dai_Y[11] = 0.0;
	dai_Y[12] = 0.0;
	dai_Y[13] = Ip/Lcur*Jp/Lcur;
	dai_Y[14] = 0.0;
	dai_Y[15] = 0.0;
	dai_Y[16] = Ip/Lcur*Kp/Lcur;
	dai_Y[17] = 0.0;
	dai_Y[18] = 0.0;
	dai_Y[19] = Jp/Lcur*Kp/Lcur;
	dai_Y[20] = 0.0;
	dai_Y[21] = 0.0;
	dai_Y[22] = Ip/Lcur*Jp/Lcur*Kp/Lcur;
	dai_Y[23] = 0.0;
	
	dai_Z[0] = 0.0;
	dai_Z[1] = 0.0;
	dai_Z[2] = 1.0;
	dai_Z[3] = 0.0;
	dai_Z[4] = 0.0;
	dai_Z[5] = Ip/Lcur;
	dai_Z[6] = 0.0;
	dai_Z[7] = 0.0;
	dai_Z[8] = Jp/Lcur;
	dai_Z[9] = 0.0;
	dai_Z[10] = 0.0;
	dai_Z[11] = Kp/Lcur;
	dai_Z[12] = 0.0;
	dai_Z[13] = 0.0;
	dai_Z[14] = Ip/Lcur*Jp/Lcur;
	dai_Z[15] = 0.0;
	dai_Z[16] = 0.0;
	dai_Z[17] = Ip/Lcur*Kp/Lcur;
	dai_Z[18] = 0.0;
	dai_Z[19] = 0.0;
	dai_Z[20] = Jp/Lcur*Kp/Lcur;
	dai_Z[21] = 0.0;
	dai_Z[22] = 0.0;
	dai_Z[23] = Ip/Lcur*Jp/Lcur*Kp/Lcur;
	
	for (int i=0; i < 24; i++)
	{
		(*result)[i] = derP0XX_PixS(mData, coord_cur_Y, coord_cur_Z, Dep000_X, Dep000_Y, Dep000_Z) * derX_P0XX(coord_cur_X) * dai_X[i] + derP1XX_PixS(mData, coord_cur_Y, coord_cur_Z, Dep000_X, Dep000_Y, Dep000_Z) * derX_P1XX(coord_cur_X) * dai_X[i] + derP2XX_PixS(mData, coord_cur_Y, coord_cur_Z, Dep000_X, Dep000_Y, Dep000_Z) * derX_P2XX(coord_cur_X) * dai_X[i] + derP3XX_PixS(mData, coord_cur_Y, coord_cur_Z, Dep000_X, Dep000_Y, Dep000_Z) * derX_P3XX(coord_cur_X) * dai_X[i]
		
		+ derP0YY_PixS(mData, coord_cur_X, coord_cur_Z, Dep000_X, Dep000_Y, Dep000_Z) * derY_P0YY(coord_cur_Y) * dai_Y[i] + derP1YY_PixS(mData, coord_cur_X, coord_cur_Z, Dep000_X, Dep000_Y, Dep000_Z) * derY_P1YY(coord_cur_Y) * dai_Y[i] + derP2YY_PixS(mData, coord_cur_X, coord_cur_Z, Dep000_X, Dep000_Y, Dep000_Z) * derY_P2YY(coord_cur_Y) * dai_Y[i] + derP3YY_PixS(mData, coord_cur_X, coord_cur_Z, Dep000_X, Dep000_Y, Dep000_Z) * derY_P3YY(coord_cur_Y) * dai_Y[i]
		
		+ derP0ZZ_PixS(mData, coord_cur_X, coord_cur_Y, Dep000_X, Dep000_Y, Dep000_Z) * derZ_P0ZZ(coord_cur_Z) * dai_Z[i] + derP1ZZ_PixS(mData, coord_cur_X, coord_cur_Y, Dep000_X, Dep000_Y, Dep000_Z) * derZ_P1ZZ(coord_cur_Z) * dai_Z[i] + derP2ZZ_PixS(mData, coord_cur_X, coord_cur_Y, Dep000_X, Dep000_Y, Dep000_Z) * derZ_P2ZZ(coord_cur_Z) * dai_Z[i] + derP3ZZ_PixS(mData, coord_cur_X, coord_cur_Y, Dep000_X, Dep000_Y, Dep000_Z) * derZ_P3ZZ(coord_cur_Z) * dai_Z[i];
	}
	return true;
}

inline bool RubicImage::deriveCubique(DVect& pos, double Ip, double Jp, double Kp, double Lcur, DVect24* result)
{
	return deriveCubique(pos.getX(), pos.getY(), pos.getZ(), Ip, Jp, Kp, Lcur, result);
}

inline unsigned char** RubicImage::operator[](long index)
{
	return mData[index];
}

inline unsigned char RubicImage::operator[](LVect& index)
{
	return mData[index.getX()][index.getY()][index.getZ()];
}

inline unsigned char arrondi(const double& valeur)
{
	if( valeur <= 0.0 )
		return 0u;
	if( valeur >= 255.0 )
		return 255u;
		
	unsigned char entier = (unsigned char) valeur;
	
	double fraction = valeur - (double) entier;
	
	if( fraction >= 0.5 )
		return entier + 1u;
	else
		return entier;
}
