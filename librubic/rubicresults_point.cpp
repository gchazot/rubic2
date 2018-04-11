/** \file rubicresults_point.cpp
  * \brief Contient la définition des fonctions membres de RubicResults_Point
  */

#include "rubicresults_point.h"

// Constructeur par défaut
RubicResults_Point::RubicResults_Point() :QMutex()
{
	U = DVect24();
	Correl = -1.0;
	Etat = 0;
	Coord = LVect(-1, -1, -1);
}

// Destructeur
RubicResults_Point::~RubicResults_Point()
{
	// Ne fait rien
}
