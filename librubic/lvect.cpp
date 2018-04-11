/** \file lvect.cpp
  * \brief Contient la définition des fonctions membres la classe LVect.
  */

#include "lvect.h"
#include "dvect.h"
#include "constants.h"

#include <cmath>
#include <cstdlib>

#include <qstring.h>
#include <QTextStream>

// Constructeur par défaut
inline LVect::LVect()
{
	mX = mY = mZ = 0;
}

// Constructeur avec initialisation des composantes
inline LVect::LVect(const long x, const long y, const long z)
{
	mX = x;
	mY = y;
	mZ = z;
}

// Constructeur de recopie
inline LVect::LVect(const LVect& v)
{
	mX = v.mX;
	mY = v.mY;
	mZ = v.mZ;
}

// Constructeur d'initialisation par un pointeur
inline LVect::LVect(const LVect* v)
{
	mX = v->mX;
	mY = v->mY;
	mZ = v->mZ;
}

// Constructeur de conversion depuis le type DVect. La conversion
// double -> long suit la même règle que pour les types de base
inline LVect::LVect(const DVect v)
{
	mX = (long)(v.mX);
	mY = (long)(v.mY);
	mZ = (long)(v.mZ);
}
// Destructeur
LVect::~LVect()
{
	// Ne fait rien
}

/*****************************************************************************
****           Manipulation des composantes par fonctions membres         ****
*****************************************************************************/
// Renvoie la composante x
inline long LVect::getX(void) const
{
	return mX;
}
// Renvoie la composante y
inline long LVect::getY(void) const
{
	return mY;
}
// Renvoie la composante y
inline long LVect::getZ(void) const
{
	return mZ;
}
// Définit la composante x
inline void LVect::setX(const long x)
{
	mX = x;
}
// Définit la composante y
inline void LVect::setY(const long y)
{
	mY = y;
}
// Définit la composante z
inline void LVect::setZ(const long z)
{
	mZ = z;
}

/*****************************************************************************
****            Définition des opérateurs valables pour LVect             ****
*****************************************************************************/
// Addition avec un autre LVect: addition composante à composante
inline LVect LVect::operator+(const LVect& v) const
{
	return LVect(mX + v.mX, mY + v.mY, mZ + v.mZ);
}

// Soustraction d'un autre LVect: chaque composante du membre de
// droite est soustraite à la composante correspondante du membre
// de droite
inline LVect LVect::operator-(const LVect& v) const
{
	return LVect(mX - v.mX, mY - v.mY, mZ - v.mZ);
}

// Opérateur modulo par rapport à un entier
/* Effectue un modulo sur chacune des composantes séparément
*/
inline LVect LVect::operator%(const long e) const
{
	return LVect(mX % e, mY % e, mZ % e);
}

// Opérateur modulo par rapport à un autre LVect
/* Effectue un modulo entre chacune des composante du membre de gauche
* et la composante correcpondante du membre de droite.
*/
inline LVect LVect::operator%(const LVect& v) const
{
	return LVect(mX % v.mX, mY % v.mY, mZ % v.mZ);
}

// Multiplication par un autre LVect: c'est le PRODUIT SCALAIRE qui
// est effectué: x1*x2 + y1*y2 + z1*z2
inline long LVect::operator*(const LVect& v) const
{
	return ( (mX * v.mX) + (mY * v.mY) + (mZ * v.mZ) );
}

// Addition avec un scalaire: le scalaire est ajouté à chacune des 
// composantes du LVect
inline LVect LVect::operator+(const long s) const
{
	return LVect(mX + s, mY + s, mZ + s);
}

// Soustraction avec un scalaire: le scalaire est soustrait à chacune
// des composantes du LVect
inline LVect LVect::operator-(const long s) const
{
	return LVect(mX - s, mY - s, mZ - s);
}

// Multiplication par un scalaire: chaque compsante est multipliée par
// le scalaire
inline LVect LVect::operator*(const long s) const
{
	return LVect(mX * s, mY * s, mZ * s);
}

// Division par un scalaire: chaque composante est divisée par le
// scalaire
inline LVect LVect::operator/(const long s) const
{
	return LVect(mX / s, mY / s, mZ / s);
}

// Opposé du vecteur: remplace chaque composante Xi par son opposé -Xi
inline LVect LVect::operator-(void) const
{
	return LVect( -mX, -mY, -mZ);
}

/////////////// Opérateurs avec affectation /////////////////
// Addition avec un autre LVect: addition composante à composante
inline void LVect::operator+=(const LVect& v)
{
	mX += v.mX;
	mY += v.mY;
	mZ += v.mZ;
}

// Soustraction d'un autre LVect: chaque composante du membre de
// droite est soustraite à la composante correspondante du membre
// de droite
inline void LVect::operator-=(const LVect& v)
{
	mX -= v.mX;
	mY -= v.mY;
	mZ -= v.mZ;
}

// Addition avec un scalaire: le scalaire est ajouté à chacune des 
// composantes du LVect
inline void LVect::operator+=(const long s)
{
	mX += s;
	mY += s;
	mZ += s;
}

// Soustraction d'un scalaire: le scalaire est soustrait à chacune des
// composantes du LVect
inline void LVect::operator-=(const long s)
{
	mX -= s;
	mY -= s;
	mZ -= s;
}

// Multiplication par un scalaire: chaque compsante est multipliée par
// le scalaire
inline void LVect::operator*=(const long s)
{
	mX *= s;
	mY *= s;
	mZ *= s;
}

// Division par un scalaire: chaque composante est divisée par le
// scalaire
inline void LVect::operator/=(const long s)
{
	mX /= s;
	mY /= s;
	mZ /= s;
}

// Opérateur d'affectation d'un autre LVect
// Permet de faire a = b; sans appel au constructeur
inline void LVect::operator=(const LVect& v)
{
	mX = v.mX;
	mY = v.mY;
	mZ = v.mZ;
}

// Opérateur d'affectation d'un tableau de 3 long
// Permet de faire a = {x, y, z)
inline void LVect::operator=(const long tab[3])
{
	mX = tab[0];
	mY = tab[1];
	mZ = tab[2];
}

// Récupération d'une référence à une composante par son numéro
// Permet d'affecter une valeur à une composante; (ex: vect[i] = 250)
// Ou de récupérer la valeur d'une composante; (ex: long Xa = vect[0])
inline long& LVect::operator[](short index)
{
	return (long &)mData[index];
}

/////////////// Opérateurs de comparaison  ////////////////
// Oppérateur d'égalité.: renvoie true si les deux vecteurs sont
// strictement identiques
inline bool LVect::operator==(const LVect& v) const
{
	return ( (v.mX == mX) && (v.mY == mY) && (v.mZ == mZ) );
}

/*****************************************************************************
****             Définition des fonctions utiles pour LVect               ****
*****************************************************************************/
// Calcul de la norme du vecteur : sqrt(x² + y² + z²)
inline double LVect::getNorme(void) const
{
	return sqrt(mX*mX + mY*mY + mZ*mZ);
}

// Calcul du CARRE de la norme du vecteur: x² + y² + z²
inline long LVect::getNorme2(void) const
{
	return (mX*mX + mY*mY + mZ*mZ);
}

//renvoie la valeur de la plus grande composante
inline long LVect::getMax(void) const
{
	long max = mX;
	if(mY > max)
		max = mY;
	if(mZ > max)
		max = mZ;
	return max;
}

//renvoie la valeur de la plus petite composante
inline long LVect::getMin(void) const
{
	long min = mX;
	if(mY < min)
		min = mY;
	if(mZ < min)
		min = mZ;
	return min;
}

// renvoie la somme des composantes
inline long LVect::getSomme(void) const
{
	return (mX + mY + mZ);
}

// renvoie la somme des valeurs absolues des composantes
inline long LVect::getSommeAbs(void) const
{
	return ( abs(mX) + abs(mY) + abs(mZ) );
}

// renvoie le produit des composantes
inline long LVect::getProduit(void) const
{
	return (mX * mY * mZ);
}

// renvoie le produit des valeurs absolues des composantes
inline long LVect::getProduitAbs(void) const
{
	return ( abs(mX) * abs(mY) * abs(mZ) );
}

// renvoie une chaine de caractères représentant le vecteur sous une
// forme affichable à l'écran
QString LVect::toString(void) const
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
****              Fonctions travaillant sur le type LVect                 *****
******************************************************************************/
///////////   Opérateurs  ////////////
// Multiplication par un scalaire: chaque compsante du LVect  est multipliée par
// le scalaire
inline LVect operator*(const long s, const LVect& v)
{
	return LVect(v.mX * s, v.mY * s, v.mZ * s);
}
