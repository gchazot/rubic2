#ifndef DMATRIX_H
#define DMATRIX_H

/** \file dmatrix.h
  * \brief Contient la définition de la classe DMatrix et des fonctions s'y rapportant
  */

#include "dvect.h"
#include "lvect.h"

class QString;

/// Une matrice 3*3 double précision
/** Cette classe permet d'effectuer toutes sortes de calculs basés sur des
  * matrices 3*3. Les objets DMatrix peuvent êtres additionnés, soustraits,
  * multipliés (entre eux ou par des scalaires), divisés par des scalaires,
  * diagonalisés (UNIQUEMENT POU LES MATRICES SYMETRIQUES), inversés, ...
  *
  * Il est également possible de multiplier une matrice par un vecteur (DVect
  * ou LVect) et ainsi d'obtenir une vecteur (DVect).
  *
  * Des fonctions membres permettent d'obtenir la trace, la diagonale, le
  * déterminant d'une matrice.
  *
  * \sa DVect, LVect
  */
class DMatrix
{
public:
	/// Constructeur par défaut
	DMatrix();
	/// Constructeur de recopie
	DMatrix(const DMatrix& mat);
	/// Constructeur à partir de 3 DVect
	/** Les vecteurs donnés en paramètre représentent les LIGNES de la matrice
	  */
	DMatrix(DVect& v1, DVect& v2, DVect& v3);
	
	/// Destructeur
	~DMatrix();
	
private:
	/// Stockage interne de la matrice 3*3
	double mData[3][3];

public:
	/// Renvoie l'inverse de la matrice en cours
	/** Rappel: Une matrice avec un déterminant nul ne peut pas être
	  * inversée. Dans ce cas, la fonction renvoie la matrice elle-même.
	  */
	DMatrix inverse(void) const;
	/// Renvoie la transposee de la matrice en cours
	DMatrix transpose(void) const;
	/// Renvoie le vecteur contenant les valeurs de la diagonale
	DVect getDiagonal(void) const;
	/// Diagonalisation de la matrice
	/** Diagonalise la matrice et place la matrice diagonale dans
	  * l'argument diag et la matrice des vecteurs propres dans
	  * pass.
	  *
	  * Renvoie false en cas d'échec de la diagonalisation
	  *
	  * Ne fonctionne QUE POUR LES MATRICES SYMETRIQUES
	  */
	bool diagonalise(DMatrix &diag, DMatrix &pass) const;
	/// Renvoie le déterminant de la matrice
	double getDeterminant(void) const;
	/// Renvoie la trace de la matrice
	double getTrace(void) const;
	
public:
	/// Opérateur d'indicage
	/** Opérateur qui renvoie la LIGNE correspondant à l'indice donné.
	  * Permet l'affectation et la récupération des valeurs de la matrice
	  */
	double* operator[](short index);
	
	/// Opérateur permettant l'addition de deux matrices
	DMatrix operator+(const DMatrix& mat) const;
	/// Opérateur permettant la soustraction de deux matrices
	DMatrix operator-(const DMatrix& mat) const;
	/// Opérateur permettant la multiplication d'une matrice par un scalaire
	DMatrix operator*(const double s) const;
	friend DMatrix operator*(const double s, const DMatrix& mat);
	/// Opérateur permettant la division d'une matrice par un scalaire
	DMatrix operator/(const double s) const;
	
	/// Opérateur qui permet de multiplier deux matrices entre elles
	DMatrix operator*(const DMatrix& mat) const;
	/// Opérateur de multiplication d'une matrice par un vecteur DVect
	DVect operator*(DVect& vect) const;
	/// Opérateur de multiplication d'une matrice par un vecteur LVect
	DVect operator*(LVect& vect) const;
	/// Opérateur de multiplication d'une matrice par un vecteur DVect
	/** \sa operator*(DVect&) */
	DVect operator()(DVect& vect) const;
	/// Opérateur de multiplication d'une matrice par un vecteur LVect
	/** \sa operator*(LVect&) */
	DVect operator()(LVect& vect) const;
	
public:
	/// Construit une matrice identité
	static DMatrix Identite(void);
	/// Construit une matrice de rotation
	/** Les angles de rotation sont à donner en radians
	  */
	static DMatrix Rotation(const double rotX, const double rotY, const double rotZ);
	
public:
	/// Conversion en chaine de caractères
	/** Renvoie une chaine de caractères représentant la matrice sous une
	  * forme affichable à l'écran.
	  */
	QString toString(void) const;
	
};

/// Opérateur de multiplication d'un scalaire par une matrice.
DMatrix operator*(const double s, const DMatrix& mat);
#endif // DMATRIX_H
