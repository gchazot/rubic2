#ifndef VOXERNCALCTHREAD_H
#define VOXERNCALCTHREAD_H

/** \file voxerncalcthread.h
  * \brief Contient la définition de la classe VoxernCalcThread
  */

#include "fonctions.h"

#include "lvect.h"
#include "dvect.h"
#include "dvect24.h"

#include <qthread.h>
#include <q3semaphore.h>
#include <qmutex.h>

class VoxernCalcThread : public QThread
{
public:
	/// Constructeur
	/** Construit un thread de calcul de voxern sans aucun paramètre de calcul.
	  */
	VoxernCalcThread();
	/// Destructeur
	~VoxernCalcThread();
	
private:
	// Variables locales du thread
	/// Coordonnées du voxern calculé
	LVect mVox;
	/// Composantes du résultat
	DVect24 mResult;
	/// Etat du calcul
	/**   0  -> inactif
	  *   1  -> calcul en cours
	  *   2  -> calcul effectué et correct
	  * >= 3 -> calcul effectué et erroné 
	  */
	uint mEtat;
	
	/// Mutex de protection de la variable d'état
	QMutex mEtatMutex;
	
public:
	/// Fonction d'initialisation du thread
	/** Le paramètre précise quel sera le voxern à calculer.
	  * Cette fonction n'a aucun effet si le thread est en cours de calcul.
	  */
	void setVoxern(LVect& voxern);
	
	/// Fonction executée lors du lancement du Thread
	virtual void run();
	
	/// Récupère l'état du voxern
	/** Cette fonction est thread-safe et peut être appelée sans condition.
	  */
	uint getEtat(void);
	
	/// Récupère les coordonnées du voxern
	LVect getVoxern(void) const;
	
	/// Place le thread en attente d'un nouveau voxern
	void setSleep(void);
};

#endif //VOXERNCALCTHREAD_H
