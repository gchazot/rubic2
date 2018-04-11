/** \file dmatrix.cpp
  * \brief Contient les définition des fonctions membres de DMatrix
  */

#include "dmatrix.h"
#include "dvect.h"
#include "lvect.h"
#include "dmatrix_utils.h"
#include "constants.h"

#include <cmath>

#include <qstring.h>
#include <QTextStream>

// Constructeur par défaut
DMatrix::DMatrix()
{
	for(short i=0; i<3; i++)
		for(short j=0; j<3; j++)
			mData[i][j] = 0.0;
}

// Constructeur de recopie
DMatrix::DMatrix(const DMatrix& mat)
{
	for(short i=0; i<3; i++)
		for(short j=0; j<3; j++)
			mData[i][j] = mat.mData[i][j];
}

// Constructeur à partir de 3 DVect
// Les vecteurs représentent les LIGNES de la matrice
DMatrix::DMatrix(DVect& v1, DVect& v2, DVect& v3)
{
	for(short i=0; i<3; i++)
	{
		mData[0][i] = v1[i];
		mData[1][i] = v2[i];
		mData[2][i] = v3[i];
	}
}

// Destructeur
DMatrix::~DMatrix()
{
}

// Renvoie l'inverse de la matrice en cours
inline DMatrix DMatrix::inverse(void) const
{
	double det = getDeterminant();
	// On vérifie que la matrice est inversible
	if( det == 0 )
		return *this;
	
	DMatrix tmp(*this);
	DMatrix result;
	
	short i,j;
	
	// Calcul de la matrice des coffacteurs
	for(i=0; i<3; i++)
		for(j=0; j<3; j++)
		{
			result[i][j] = cofacteur(&tmp,i,j);
		}

	// calcul de l'inverse
	return result.transpose() / det;
}

// Renvoie la transposee de la matrice en cours
DMatrix DMatrix::transpose(void) const
{
	DMatrix result;
	short i, j;
	
	for(i=0; i<3; i++)
		for(j=0; j<3; j++)
			result.mData[i][j] = mData[j][i];
	return result;
}

// Renvoie le vecteur contenant les valeurs de la diagonale
inline DVect DMatrix::getDiagonal(void) const
{
	return DVect(mData[0][0], mData[1][1], mData[2][2]);
}

// Diagonalise la matrice et place la matrice diagonale dans
// l'argument diag et la matrice des vecteurs propres dans
// pass.
// renvoie false en cas d'échec de la diagonalisation
inline bool DMatrix::diagonalise(DMatrix& diag, DMatrix& pass) const
{
	DVect diagonale;
	DMatrix temp(*this);
	int nrot;
	if(! jacobi(&temp, &diagonale, &pass, &nrot) )
		return false;

	for(short i=0; i<3; i++)
	{
		for(short j=0; j<3; j++)
			diag[i][j] = 0.0;
		diag[i][i] = diagonale[i];
	}

	return true;
}

// Renvoie le déterminant de la matrice
inline double DMatrix::getDeterminant(void) const
{
	double result = 0.0;
	result += mData[0][0] * mData[1][1] * mData[2][2];
	result += mData[0][1] * mData[1][2] * mData[2][0];
	result += mData[0][2] * mData[1][0] * mData[2][1];
	
	result -= mData[0][2] * mData[1][1] * mData[2][0];
	result -= mData[0][1] * mData[1][0] * mData[2][2];
	result -= mData[0][0] * mData[1][2] * mData[2][1];
	
	return result;
}

// Renvoie la trace de la matrice
inline double DMatrix::getTrace(void) const
{
	return mData[0][0] + mData[1][1] + mData[2][2];
}

// Opérateur qui renvoie la ligne correspondant à l'indice
// Permet l'affectation et la récupération des valeurs
inline double* DMatrix::operator[](short index)
{
	return mData[index];
}

// Opérateur permettant l'addition de deux matrices
inline DMatrix DMatrix::operator+(const DMatrix& mat) const
{
	DMatrix tmp;
	for(short i=0; i<3; i++)
		for(short j=0; j<3; j++)
			tmp.mData[i][j] = mData[i][j] + mat.mData[i][j];
	return (DMatrix&)tmp;
}
// Opérateur permettant la soustraction de deux matrices
inline DMatrix DMatrix::operator-(const DMatrix& mat) const
{
	DMatrix tmp;
	for(short i=0; i<3; i++)
		for(short j=0; j<3; j++)
			tmp.mData[i][j] = mData[i][j] - mat.mData[i][j];
	return (DMatrix&)tmp;
}

// Opérateur permettant la multiplication d'une matrice par un scalaire
inline DMatrix DMatrix::operator*(const double s) const
{
	DMatrix tmp;
	for(short i=0; i<3; i++)
		for(short j=0; j<3; j++)
			tmp.mData[i][j] = mData[i][j] * s;
	return (DMatrix&)tmp;
}

// Opérateur permettant la division d'une matrice par un scalaire
inline DMatrix DMatrix::operator/(const double s) const
{
	DMatrix tmp;
	for(short i=0; i<3; i++)
		for(short j=0; j<3; j++)
			tmp.mData[i][j] = mData[i][j] / s;
	return (DMatrix&)tmp;
}


// Opérateur qui permet de multiplier deux matrices entre elles
inline DMatrix DMatrix::operator*(const DMatrix& mat) const
{
	DMatrix tmp;
	for(short i=0; i<3; i++)
		for(short j=0; j<3; j++)
			for(short p=0; p<3; p++)
				tmp.mData[i][j] += mData[i][p] * mat.mData[p][j];
	return (DMatrix&)tmp;
}

// Opérateur qui renvoie le vecteur résultant du produit d'une
// matrice par un vecteur
inline DVect DMatrix::operator*(DVect& vect) const
{
	DVect res;
	for(short i=0; i<3; i++)
		for(short p=0; p<3; p++)
			res[i] += mData[i][p] * vect[p];
	return res;
}

inline DVect DMatrix::operator*(LVect& vect) const
{
	DVect res;
	for(short i=0; i<3; i++)
		for(short p=0; p<3; p++)
			res[i] += mData[i][p] * vect[p];
	return res;
}

inline DVect DMatrix::operator()(DVect& vect) const
{
	return operator*(vect);
}

inline DVect DMatrix::operator()(LVect& vect) const
{
	return operator*(vect);
}

// Construit une matrice identité
inline DMatrix DMatrix::Identite(void)
{
	DMatrix res;
	for(short i = 0; i<3; i++)
		res.mData[i][i] = 1.0;
	return res;
}

// Construit une matrice de rotation
// Les angles de rotation sont à donner en radians
inline DMatrix DMatrix::Rotation(const double rotX, const double rotY, const double rotZ)
{
	DMatrix matX, matY, matZ;
	
	// Rotation autour de X
	matX.mData[1][1] = cos(rotX);
	matX.mData[2][2] = matX.mData[1][1];
	matX.mData[1][2] = sin(rotX);
	matX.mData[2][1] = -matX.mData[1][2];
	matX.mData[0][0] = 1.0;
	
	// Rotation autour de Y
	matY.mData[0][0] = cos(rotY);
	matY.mData[2][2] = matY.mData[0][0];
	matY.mData[0][2] = sin(rotY);
	matY.mData[2][0] = -matY.mData[0][2];
	matY.mData[1][1] = 1.0;
	
	// Rotation autour de Z
	matZ.mData[0][0] = cos(rotZ);
	matZ.mData[1][1] = matZ.mData[0][0];
	matZ.mData[0][1] = sin(rotZ);
	matZ.mData[1][0] = -matZ.mData[0][1];
	matZ.mData[2][2] = 1.0;
	
	return matX*matY*matZ;
}

// renvoie une chaine de caractères représentant la matrice sous une
// forme affichable à l'écran
QString DMatrix::toString(void) const
{
	QString result;
	QTextStream stream(&result);
	
	stream << "\t[ ";
	stream.precision(PREC_FLOAT_AFF); stream.width(LARG_FLOAT_AFF);
	stream << mData[0][0] << " | ";
	stream.precision(PREC_FLOAT_AFF); stream.width(LARG_FLOAT_AFF);
	stream << mData[0][1] << " | ";
	stream.precision(PREC_FLOAT_AFF); stream.width(LARG_FLOAT_AFF);
	stream << mData[0][2] << " ]" << endl;
	
	stream << "\t[ ";
	stream.precision(PREC_FLOAT_AFF); stream.width(LARG_FLOAT_AFF);
	stream << mData[1][0] << " | ";
	stream.precision(PREC_FLOAT_AFF); stream.width(LARG_FLOAT_AFF);
	stream << mData[1][1] << " | ";
	stream.precision(PREC_FLOAT_AFF); stream.width(LARG_FLOAT_AFF);
	stream << mData[1][2] << " ]" << endl;
	
	stream << "\t[ ";
	stream.precision(PREC_FLOAT_AFF); stream.width(LARG_FLOAT_AFF);
	stream << mData[2][0] << " | ";
	stream.precision(PREC_FLOAT_AFF); stream.width(LARG_FLOAT_AFF);
	stream << mData[2][1] << " | ";
	stream.precision(PREC_FLOAT_AFF); stream.width(LARG_FLOAT_AFF);
	stream << mData[2][2] << " ]" << endl;
	
	stream << "Déterminant : " << getDeterminant() << endl;
	stream << "Trace : " << getTrace();
// 	stream << endl;
	
	stream << flush;
	
	return result;
}

// Opérateur de multiplication d'un scalaire par une matrice.
inline DMatrix operator*(const double s, const DMatrix& mat)
{
	DMatrix tmp;
	for(short i=0; i<3; i++)
		for(short j=0; j<3; j++)
			tmp.mData[i][j] = mat.mData[i][j] * s;
	return (DMatrix&)tmp;
}
