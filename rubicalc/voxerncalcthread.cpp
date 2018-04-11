/** \file voxerncalcthread.cpp
  * \brief Contient la définition des fonctions membres de la classe VoxernCalcThread
  */

#include "voxerncalcthread.h"
#include "globals.h"

// Constructeur
VoxernCalcThread::VoxernCalcThread()
{
	mEtat = 0;
}
	
// Destructeur
VoxernCalcThread::~VoxernCalcThread()
{
}

// Fonction d'initialisation du thread
void VoxernCalcThread::setVoxern(LVect& voxern)
{
	if( getEtat() == 0 )
	{
		// On sauvegarde le voxern à calculer
		mVox = voxern;
		
		// On récupère les données d'initialisation
		(*resultats)[mVox]->lock();
		mResult = (*resultats)[mVox]->U;
		(*resultats)[mVox]->unlock();

		// On passe dans l'état "Calcul en cours"
		mEtatMutex.lock();
		mEtat = 1;
		mEtatMutex.unlock();
	}
	else
	{
		stdlog->message(LOGUTIL_CRITICAL, "Tentative de modification d'un thread de caclul pas au repos");
		quitter(75);
	}
	return;
}

// Fonction executée lors du lancement du Thread
void VoxernCalcThread::run()
{
	// Les coordonnées en voxel du voxern à calculer
	LVect Voxel;
	// Initialisation et résultat du gradient conjugué
	DVect24 Uin, Uout;
	
	// Le résultat du gradient conjugué (correlation)
	double result;
	
	// Récupération des données du voxern
	(*resultats)[mVox]->lock();
	Voxel = (*resultats)[mVox]->Coord;
	Uin = (*resultats)[mVox]->U;
	(*resultats)[mVox]->unlock();
	
	stdlog->DEBUGMESS("Calcul du voxern : " + mVox.toString() );
	
	// On effectue le calcul
	result = ChercheVoxern(Voxel, Uin, &Uout);
	
	// Stockage des résultats en fin de calcul
	(*resultats)[mVox]->lock();
	(*resultats)[mVox]->U = Uout;
	(*resultats)[mVox]->Correl = result;
	(*resultats)[mVox]->unlock();
	
	// On change l'état en fonction du résultat du gradient conjugué.
	mEtatMutex.lock();
	if(result < 0.0)
		mEtat = 3; // Cas ou une erreur s'est produite
	else
		mEtat = 2; // Cas ou le calcul s'est bien passé
	mEtatMutex.unlock();
	
	// Sortie du résultat dans le log de calcul
	calclog->message(LOGUTIL_INFO, mVox.toString() + " : " + QString::number(result) + " : " + Uout.toString() );	
	// Sortie du résultat dans le log de debug
	stdlog->message(LOGUTIL_INFO, mVox.toString() + " : " + QString::number(result) + " : " + Uout.toString() );
}

// Récupère l'état du voxern
uint VoxernCalcThread::getEtat(void)
{
	uint res;
	mEtatMutex.lock();
	res = mEtat;
	mEtatMutex.unlock();
	return res;
}

// Récupère les coordonnées du voxern
LVect VoxernCalcThread::getVoxern(void) const
{
	return mVox;
}

void VoxernCalcThread::setSleep(void)
{
	mEtatMutex.lock();
	mEtat = 0;
	mEtatMutex.unlock();
}
