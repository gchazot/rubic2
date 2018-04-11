/** \file dvect24.cpp
  * \brief Contient les définitions des fonctions membres de DVect24
  */

#include "dvect24.h"
#include "constants.h"

#include <cmath>

#include <qstring.h>
#include <QTextStream>

// Constructeur par défaut
inline DVect24::DVect24()
{
	for(short i=0; i<24; i++)
		mData[i] = 0.0;
}

// Constructeur de recopie
inline DVect24::DVect24(const DVect24& v)
{
	for(short i=0; i<24; i++)
		mData[i] = v.mData[i];
}

// Constructeur d'initialisation par un pointeur
inline DVect24::DVect24(const DVect24* v)
{
	for(short i=0; i<24; i++)
		mData[i] = v->mData[i];
}

// Destructeur
DVect24::~DVect24()
{
	// Ne fait rien
}

/*****************************************************************************
****            Définition des opérateurs valables pour DVect24             ****
*****************************************************************************/
// Addition avec un autre DVect24: addition composante à composante
inline DVect24 DVect24::operator+(const DVect24& v) const
{
	DVect24 result(this);
	for(short i=0; i<24; i++)
		result.mData[i] += v.mData[i];
	
	return result;
}

// Soustraction d'un autre DVect24: chaque composante du membre de
// droite est soustraite à la composante correspondante du membre
// de droite
inline DVect24 DVect24::operator-(const DVect24& v) const
{
	DVect24 result(this);
	for(short i=0; i<24; i++)
		result.mData[i] -= v.mData[i];
		
	return result;
}

// Multiplication par un autre DVect24: c'est le PRODUIT SCALAIRE qui
// est effectué: x1*x2 + y1*y2 + z1*z2
inline double DVect24::operator*(const DVect24& v) const
{
	double result = 0.0;
	for(short i=0; i<24; i++)
		result += mData[i] * v.mData[i];
		
	return result;
}

// Addition avec un scalaire: le scalaire est ajouté à chacune des 
// composantes du DVect24
inline DVect24 DVect24::operator+(const double s) const
{
	DVect24 result(this);
	for(short i=0; i<24; i++)
		result.mData[i] += s;
	
	return result;
}

// Soustraction avec un scalaire: le scalaire est soustrait à chacune
// des composantes du DVect24
inline DVect24 DVect24::operator-(const double s) const
{
	DVect24 result(this);
	for(short i=0; i<24; i++)
		result.mData[i] -= s;
		
	return result;
}

// Multiplication par un scalaire: chaque compsante est multipliée par
// le scalaire
inline DVect24 DVect24::operator*(const double s) const
{
	DVect24 result(this);
	for(short i=0; i<24; i++)
		result.mData[i] *= s;
		
	return result;
}

// Division par un scalaire: chaque composante est divisée par le
// scalaire
inline DVect24 DVect24::operator/(const double s) const
{
	DVect24 result(this);
	for(short i=0; i<24; i++)
		result.mData[i] /= s;
		
	return result;
}

// Opposé du vecteur: remplace chaque composante Xi par son opposé -Xi
inline DVect24 DVect24::operator-(void) const
{
	DVect24 result(this);
	for(short i=0; i<24; i++)
		result.mData[i] = -result.mData[i];
		
	return result;
}

/////////////// Opérateurs avec affectation /////////////////
// Addition avec un autre DVect24: addition composante à composante
inline void DVect24::operator+=(const DVect24& v)
{
	for(short i=0; i<24; i++)
		mData[i] += v.mData[i];
}

// Soustraction d'un autre DVect24: chaque composante du membre de
// droite est soustraite à la composante correspondante du membre
// de droite
inline void DVect24::operator-=(const DVect24& v)
{
	for(short i=0; i<24; i++)
		mData[i] -= v.mData[i];
}

// Addition avec un scalaire: le scalaire est ajouté à chacune des 
// composantes du DVect24
inline void DVect24::operator+=(const double s)
{
	for(short i=0; i<24; i++)
		mData[i] += s;
}

// Soustraction d'un scalaire: le scalaire est soustrait à chacune des
// composantes du DVect24
inline void DVect24::operator-=(const double s)
{
	for(short i=0; i<24; i++)
		mData[i] -= s;
}

// Multiplication par un scalaire: chaque compsante est multipliée par
// le scalaire
inline void DVect24::operator*=(const double s)
{
	for(short i=0; i<24; i++)
		mData[i] *= s;
}

// Division par un scalaire: chaque composante est divisée par le
// scalaire
inline void DVect24::operator/=(const double s)
{
	for(short i=0; i<24; i++)
		mData[i] /= s;
}

// Opérateur d'affectation d'un autre DVect24
// Permet de faire a = b; sans appel au constructeur
inline void DVect24::operator=(const DVect24& v)
{
	for(short i=0; i<24; i++)
		mData[i] = v.mData[i];
}


// Récupération d'une référence à une composante par son numéro
// Permet d'affecter une valeur à une composante; (ex: vect[i] = 250)
// Ou de récupérer la valeur d'une composante; (ex: double Xa = vect[0])
inline double& DVect24::operator[](short index)
{
	return (double &)mData[index];
}

/////////////// Opérateurs de comparaison  ////////////////
// Oppérateur d'égalité.: renvoie true si les deux vecteurs sont
// strictement identiques
inline bool DVect24::operator==(const DVect24& v) const
{
	for(short i=0; i<24; i++)
		if( mData[i] != v.mData[i] )
			return false;
	return true;
}

/*****************************************************************************
****             Définition des fonctions utiles pour DVect24               ****
*****************************************************************************/
// Calcul de la norme du vecteur : sqrt(x² + y² + z²)
inline double DVect24::getNorme(void) const
{
	return sqrt(getNorme2());
}

// Calcul du CARRE de la norme du vecteur: x² + y² + z²
inline double DVect24::getNorme2(void) const
{
	double result = 0.0;
	for(short i=0; i<24; i++)
		result += mData[i] * mData[i];
	
	return result;
}

//renvoie la valeur de la plus grande composante
inline double DVect24::getMax(void) const
{
	double max = mData[0];
	
	for(short i=1; i<24; i++)
		if(mData[i] > max)
			max = mData[i];

	return max;
}

//renvoie la valeur de la plus petite composante
inline double DVect24::getMin(void) const
{
	double min = mData[0];
	
	for(short i=1; i<24; i++)
		if(mData[i] < min)
			min = mData[i];

	return min;
}

// renvoie la somme des composantes
inline double DVect24::getSomme(void) const
{
	double somme = 0.0;
	
	for(short i=0; i<24; i++)
		somme += mData[i];

	return somme;
}

// renvoie la somme des valeurs absolues des composantes
inline double DVect24::getSommeAbs(void) const
{
	double somme = 0.0;
	
	for(short i=0; i<24; i++)
		somme += fabs(mData[i]);

	return somme;
}

// renvoie le produit des composantes
inline double DVect24::getProduit(void) const
{
	double produit = 1.0;
	
	for(short i=0; i<24; i++)
		produit *= mData[i];

	return produit;
}

// renvoie le produit des valeurs absolues des composantes
inline double DVect24::getProduitAbs(void) const
{
	double produit = 1.0;
	
	for(short i=0; i<24; i++)
		produit *= fabs(mData[i]);

	return produit;
}

// Norme le vecteur à la valeur donnée en parametre (1.0 par défaut)
inline void DVect24::normer(double r)
{
	double fact = getNorme();
	if(fact == 0.0)
		return;
	fact = r / fact;
	
	operator*=(fact);
}

// renvoie une chaine de caractères représentant le vecteur sous une
// forme affichable à l'écran
QString DVect24::toString(void) const
{
	QString result;
	QTextStream stream(&result);

	stream << "\t[ ";
	stream.precision(PREC_FLOAT_AFF); stream.width(LARG_FLOAT_AFF);
	stream << mData[0];
	
	for(short i=1; i<24; i++)
	{
		stream << " | ";
		stream.precision(PREC_FLOAT_AFF); stream.width(LARG_FLOAT_AFF);
		stream << mData[i];
	}
	
	stream <<  " ]";

	stream << flush;

	return result;
}

// renvoie une chaine de caractères représentant le3 3 premières composantes
// du vecteur sous une forme affichable à l'écran
QString DVect24::toString3(void) const
{
	QString result;
	QTextStream stream(&result);

	stream << "\t[ ";
	stream.precision(PREC_FLOAT_AFF); stream.width(LARG_FLOAT_AFF);
	stream << mData[0];
	
	for(short i=1; i<3; i++)
	{
		stream << " | ";
		stream.precision(PREC_FLOAT_AFF); stream.width(LARG_FLOAT_AFF);
		stream << mData[i];
	}
	
	stream <<  " ]";

	stream << flush;

	return result;
}


/******************************************************************************
****              Fonctions travaillant sur le type DVect24                 *****
******************************************************************************/
///////////   Opérateurs  ////////////
// Multiplication par un scalaire: chaque compsante du DVect24  est multipliée par
// le scalaire
inline DVect24 operator*(const double s, const DVect24& v)
{
	DVect24 result(v);
	
	for(short i=0; i<24; i++)
		result.mData[i] *= s;
		
	return result;
}
