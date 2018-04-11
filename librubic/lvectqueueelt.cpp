/** \file lvectqueueelt.cpp
  * \brief Contient la définition des fonctions membres de la classe LVectQueueElt
  */

#include "lvectqueueelt.h"

// Constructeur
LVectQueueElt::LVectQueueElt(const LVect& vec): LVect(vec)
{
	next = NULL;
}
	
// Destructeur
LVectQueueElt::~LVectQueueElt()
{
}
