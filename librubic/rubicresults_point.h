#ifndef RUBICRESULTS_POINT_H
#define RUBICRESULTS_POINT_H

/** \file rubicresults_point.h
  * \brief Contient la définition de la classe RubicResults_Point
  */
  
#include "dvect24.h"
#include "lvect.h"

#include <qmutex.h>

/// Classe de gestion des résultats de calcul en un point de l'image
/** Cette classe manipule en un seul objet les résultats de calcul sur un
  * voxern calculé. Ceci comprends les 24 coefficients du champ de
  * déformation, la valeur du coefficient de correlation, ...
  *
  * La variable Etat donne l'état de calcul du voxern:
  *   0  -> non-calculé
  *   1  -> calcul en cours
  *   2  -> calculé et résultat valide
  * >= 3 -> calculé et résultat non valide
  *
  * La classe derive de QMutex pour permettre son verrouillage et son
  * deverouillage. L'OBJET DOIT ETRE VERROUILLE PENDANT L'ACCES AUX DONNEES.
  *
  * \sa DVect24, LVect, DVect
  */
class RubicResults_Point : public QMutex
{
public:
	/// Constructeur par défaut
	RubicResults_Point();
	/// Destructeur
	~RubicResults_Point();
	
public:
	/// Le vecteur des 24 coefficients du champ de déplacement
	DVect24 U;
	/// La valeur du coefficient de correlation
	double Correl;
	/// L'état de calcul du voxern.
	short Etat;
	/// Les coordonnées du voxern dans l'image globale.
	LVect Coord;
	/// Le mutex de protection
	QMutex Mutex;
};

#endif // RUBICRESULTS_POINT_H
