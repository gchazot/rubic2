#ifndef LVECTQUEUEELT_H
#define LVECTQUEUEELT_H
/** \file lvectqueueelt.h
  * \brief Contient la définition de la classe LVectQueueElt
  */

#include "lvect.h"
#include <cstdlib>

/// Element de liste chainée simple de type LVect
/** Cette classe permet de réaliser une liste chainée simple de LVect.
  * Elle est notament utilisée pour la réalisation de la file d'attente sans
  * doublons LVectUniqueQueue.
  */
class LVectQueueElt: public LVect
{
public:
	/// Constructeur
	/** Construit un nouveau LVectQueueElt contenant le LVect donné en
	  * paramètre. Une copie du Lvect est effectuée.
	  */
	LVectQueueElt(const LVect& vec);
	
	/// Destructeur
	~LVectQueueElt();
	
public:
	/// Pointeur vers le prochain élement de la liste
	LVectQueueElt* next;
};

#endif //LVECTQUEUEELT_H
