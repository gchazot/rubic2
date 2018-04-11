/** \file dvect.cpp
  * \brief Contient les définitions des fonctions membres de DVect
  */

#include "dvect.h"
#include "lvect.h"
#include "dmatrix.h"
#include "constants.h"

#include <cmath>

#include <qstring.h>
#include <QTextStream>

// Constructeur par défaut
inline DVect::DVect()
{
	mX = mY = mZ = 0.0;
}

// Constructeur avec initialisation des composantes
inline DVect::DVect(const double x, const double y, const double z)
{
	mX = x;
	mY = y;
	mZ = z;
}

// Constructeur de recopie
inline DVect::DVect(const DVect& v)
{
	mX = v.mX;
	mY = v.mY;
	mZ = v.mZ;
}

// Constructeur d'initialisation par un pointeur
inline DVect::DVect(const DVect* v)
{
	mX = v->mX;
	mY = v->mY;
	mZ = v->mZ;
}

// Constructeur de conversion depuis le type LVect. La conversion
// long -> double suit la même règle que pour les types de base
inline DVect::DVect(const LVect v)
{
	mX = (double)(v.mX);
	mY = (double)(v.mY);
	mZ = (double)(v.mZ);
}

// Destructeur
DVect::~DVect()
{
	// Ne fait rien
}

/*****************************************************************************
****           Manipulation des composantes par fonctions membres         ****
*****************************************************************************/
// Renvoie la composante x
inline double DVect::getX(void) const
{
	return mX;
}
// Renvoie la composante y
inline double DVect::getY(void) const
{
	return mY;
}
// Renvoie la composante y
inline double DVect::getZ(void) const
{
	return mZ;
}
// Définit la composante x
inline void DVect::setX(const double x)
{
	mX = x;
}
// Définit la composante y
inline void DVect::setY(const double y)
{
	mY = y;
}
// Définit la composante z
inline void DVect::setZ(const double z)
{
	mZ = z;
}

/*****************************************************************************
****            Définition des opérateurs valables pour DVect             ****
*****************************************************************************/
// Addition avec un autre DVect: addition composante à composante
inline DVect DVect::operator+(const DVect& v) const
{
	return DVect(mX + v.mX, mY + v.mY, mZ + v.mZ);
}

// Soustraction d'un autre DVect: chaque composante du membre de
// droite est soustraite à la composante correspondante du membre
// de droite
inline DVect DVect::operator-(const DVect& v) const
{
	return DVect(mX - v.mX, mY - v.mY, mZ - v.mZ);
}

// Multiplication par un autre DVect: c'est le PRODUIT SCALAIRE qui
// est effectué: x1*x2 + y1*y2 + z1*z2
inline double DVect::operator*(const DVect& v) const
{
	return ( (mX * v.mX) + (mY * v.mY) + (mZ * v.mZ) );
}

// Addition avec un scalaire: le scalaire est ajouté à chacune des 
// composantes du DVect
inline DVect DVect::operator+(const double s) const
{
	return DVect(mX + s, mY + s, mZ + s);
}

// Soustraction avec un scalaire: le scalaire est soustrait à chacune
// des composantes du DVect
inline DVect DVect::operator-(const double s) const
{
	return DVect(mX - s, mY - s, mZ - s);
}

// Multiplication par un scalaire: chaque compsante est multipliée par
// le scalaire
inline DVect DVect::operator*(const double s) const
{
	return DVect(mX * s, mY * s, mZ * s);
}

// Division par un scalaire: chaque composante est divisée par le
// scalaire
inline DVect DVect::operator/(const double s) const
{
	return DVect(mX / s, mY / s, mZ / s);
}

// Opposé du vecteur: remplace chaque composante Xi par son opposé -Xi
inline DVect DVect::operator-(void) const
{
	return DVect( -mX, -mY, -mZ);
}

/////////////// Opérateurs avec affectation /////////////////
// Addition avec un autre DVect: addition composante à composante
inline void DVect::operator+=(const DVect& v)
{
	mX += v.mX;
	mY += v.mY;
	mZ += v.mZ;
}

// Soustraction d'un autre DVect: chaque composante du membre de
// droite est soustraite à la composante correspondante du membre
// de droite
inline void DVect::operator-=(const DVect& v)
{
	mX -= v.mX;
	mY -= v.mY;
	mZ -= v.mZ;
}

// Addition avec un scalaire: le scalaire est ajouté à chacune des 
// composantes du DVect
inline void DVect::operator+=(const double s)
{
	mX += s;
	mY += s;
	mZ += s;
}

// Soustraction d'un scalaire: le scalaire est soustrait à chacune des
// composantes du DVect
inline void DVect::operator-=(const double s)
{
	mX -= s;
	mY -= s;
	mZ -= s;
}

// Multiplication par un scalaire: chaque compsante est multipliée par
// le scalaire
inline void DVect::operator*=(const double s)
{
	mX *= s;
	mY *= s;
	mZ *= s;
}

// Division par un scalaire: chaque composante est divisée par le
// scalaire
inline void DVect::operator/=(const double s)
{
	mX /= s;
	mY /= s;
	mZ /= s;
}

// Opérateur d'affectation d'un autre DVect
// Permet de faire a = b; sans appel au constructeur
inline void DVect::operator=(const DVect& v)
{
	mX = v.mX;
	mY = v.mY;
	mZ = v.mZ;
}

// Opérateur d'affectation d'un tableau de 3 double
// Permet de faire a = {x, y, z)
inline void DVect::operator=(const double tab[3])
{
	mX = tab[0];
	mY = tab[1];
	mZ = tab[2];
}

// Récupération d'une référence à une composante par son numéro
// Permet d'affecter une valeur à une composante; (ex: vect[i] = 250)
// Ou de récupérer la valeur d'une composante; (ex: double Xa = vect[0])
inline double& DVect::operator[](short index)
{
	return (double &)mData[index];
}

// Opérateur qui renvoie le vecteur résultant du produit d'un
// vecteur par une matrice
inline DVect DVect::operator*(DMatrix& mat) const
{
	DVect res;
	for(short i=0; i<3; i++)
		for(short p=0; p<3; p++)
			res[i] += mData[p] * mat[p][i];
	return res;
}

/////////////// Opérateurs de comparaison  ////////////////
// Oppérateur d'égalité.: renvoie true si les deux vecteurs sont
// strictement identiques
inline bool DVect::operator==(const DVect& v) const
{
	return ( (v.mX == mX) && (v.mY == mY) && (v.mZ == mZ) );
}

/*****************************************************************************
****             Définition des fonctions utiles pour DVect               ****
*****************************************************************************/
// Calcul de la norme du vecteur : sqrt(x² + y² + z²)
inline double DVect::getNorme(void) const
{
	return sqrt(mX*mX + mY*mY + mZ*mZ);
}

// Calcul du CARRE de la norme du vecteur: x² + y² + z²
inline double DVect::getNorme2(void) const
{
	return (mX*mX + mY*mY + mZ*mZ);
}

//renvoie la valeur de la plus grande composante
inline double DVect::getMax(void) const
{
	double max = mX;
	if(mY > max)
		max = mY;
	if(mZ > max)
		max = mZ;
	return max;
}

//renvoie la valeur de la plus petite composante
inline double DVect::getMin(void) const
{
	double min = mX;
	if(mY < min)
		min = mY;
	if(mZ < min)
		min = mZ;
	return min;
}

// renvoie la somme des composantes
inline double DVect::getSomme(void) const
{
	return (mX + mY + mZ);
}

// renvoie la somme des valeurs absolues des composantes
inline double DVect::getSommeAbs(void) const
{
	return ( fabs(mX) + fabs(mY) + fabs(mZ) );
}

// renvoie le produit des composantes
inline double DVect::getProduit(void) const
{
	return (mX * mY * mZ);
}

// renvoie le produit des valeurs absolues des composantes
inline double DVect::getProduitAbs(void) const
{
	return ( fabs(mX) * fabs(mY) * fabs(mZ) );
}

// Norme le vecteur à la valeur donnée en parametre (1.0 par défaut)
inline void DVect::normer(double r)
{
	double fact = getNorme();
	if(fact == 0.0)
		return;
	fact = r / fact;
	mX *= fact;
	mY *= fact;
	mZ *= fact;
}

// renvoie une chaine de caractères représentant le vecteur sous une
// forme affichable à l'écran
QString DVect::toString(void) const
{
	QString result;
	QTextStream stream(&result);

	stream << "\t[ ";
	stream.precision(PREC_FLOAT_AFF); stream.width(LARG_FLOAT_AFF);
	stream << mData[0] << " | ";
	stream.precision(PREC_FLOAT_AFF); stream.width(LARG_FLOAT_AFF);
	stream << mData[1] << " | ";
	stream.precision(PREC_FLOAT_AFF); stream.width(LARG_FLOAT_AFF);
	stream << mData[2] << " ]";

	stream << flush;

	return result;
}

/******************************************************************************
****              Fonctions travaillant sur le type DVect                 *****
******************************************************************************/
///////////   Opérateurs  ////////////
// Multiplication par un scalaire: chaque compsante du DVect  est multipliée par
// le scalaire
inline DVect operator*(const double s, const DVect& v)
{
	return DVect(v.mX * s, v.mY * s, v.mZ * s);
}
