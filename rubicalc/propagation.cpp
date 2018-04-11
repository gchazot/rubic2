/** \file propagation.cpp
  * \brief Contient la défition des fonctions gérant le calcul et sa propagation
  */

#include "voxerncalcthread.h"
#include "globals.h"

VoxernCalcThread *threads;

/// Renvoie le numéro du premier thread inactif ou -1 si ils sont tous actifs
int getFreeThread(void)
{
	for(int i = 0; i < MaxThreads; i++)
		if(threads[i].getEtat() == 0)
			return i;
	return -1;
}

/// Renvoie le numéro du premier thread terminé ou -1 si aucun n'est terminé
int getFinishedThread(void)
{
	for(int i = 0; i < MaxThreads; i++)
		if(threads[i].getEtat() >= 2)
			return i;
	return -1;
}

/// Ajoute les 6 voxerns alentours du paramètre aux voxerns calculables.
/** Cette fonction vérifie que l'on reste dans le maillage virtuel et que les
  * voxerns ajoutés ne sont pas en cours de calcul ou calculés.
  */
void ajouteVoxernsAdjacents(LVect voxern)
{
	LVect vox;
	// Pour boucler sur les 6
	for(short i = 0; i < 3; i++)
		for(short j = -1; j<=1; j+=2)
		{
			// On calcule les nouvelles coordonnées
			LVect rel = LVect(0,0,0);
			rel[i] = j;
			vox = voxern + rel;
			
			// On vérifie qu'on reste dans le maillage virtuel
			if( (vox.getMin() < 0) || (vox[0] >= nbVoxerns[0])
			 || (vox[1] >= nbVoxerns[1]) || (vox[2] >= nbVoxerns[2]) )
				continue;
			// On vérifie qu'il n'est pas déjà calculé
			if(voxCalcules.contains(vox))
				continue;
			// On vérifie qu'il n'est pas en cours de calcul
			if(voxEnCours.contains(vox))
				continue;
			
			// On l'ajoute à la liste des calculables
			voxCalculables.addVecteur(vox);
			if(vox == voxern)
				quitter(76);
			// On place le vecteur d'initialisation
			(*resultats)[vox]->lock();
			(*resultats)[voxern]->lock();
			
			(*resultats)[vox]->U = (*resultats)[voxern]->U;
			
			(*resultats)[vox]->unlock();
			(*resultats)[voxern]->unlock();
		}
}

bool executePropagation(void)
{
	stdlog->DEBUGMESS("executePropagation()");
	
	// Initialisation des listes de voxerns
	voxCalculables.addVecteur(Ivoxern);
	(*resultats)[Ivoxern]->U = U24;
	
	// Le pool de threads de calcul
	threads = new VoxernCalcThread[MaxThreads];
	
	if(threads == NULL)
	{
		stdlog->message(LOGUTIL_CRITICAL, "Impossible de créer les threads de calcul");
		return false;
	}
	
	stdlog->message(LOGUTIL_INFO, "Threads de calcul créés");
	
	// Un pointeur temporaire pour des LVect
	LVect *tmpVec;
	// Un entier temporaire
	int tmpInt;
	
	// Permet de sortir de la boucle principale en cas d'erreur
	bool erreurCritique = false;
	
	while( ((voxEnCours.getSize() != 0) || (voxCalculables.getSize() != 0)) && (erreurCritique == false) )
	{
		// On vérifie si on a des voxerns calculables et des threads libres
		while( (voxCalculables.getSize() != 0) && ((tmpInt = getFreeThread()) != -1) )
		{
			// On prend le premier élement de la pile
			tmpVec = voxCalculables.popVecteur();
			// On le place dans la liste de ceux qui sont en cours de calcul
			voxEnCours.addVecteur(*tmpVec);
			
			// On prépare le thread pour le calcul
			threads[tmpInt].setVoxern(*tmpVec);
			
			// On libère la mémoire du vecteur
			delete tmpVec;
			
			// On lance le calcul sur le thread
			threads[tmpInt].start();
		}
		
		// Boucle de vérification des calculs terminés
		while( (tmpInt = getFinishedThread()) != -1 )
		{
			// On récupère le voxern correspondant
			LVect voxern = threads[tmpInt].getVoxern();
			
			// Cas d'un voxern correct
			if(threads[tmpInt].getEtat() == 2)
			{
				// On l'ajoute aux voxerns calculés
				voxCalcules.addVecteur(voxern);
				// On l'enlève aux voxerns en cours
				voxEnCours.removeVecteur(voxern);
				
				// On ajoute les 6 alentours aux calculables
				ajouteVoxernsAdjacents(voxern);
			}
			// Cas d'un voxern erroné
			else
			{
				// On l'ajoute aux voxerns calculés
//				voxCalcules.addVecteur(voxern);
				// On l'ajoute aux voxerns erronés
//				voxErrones.addVecteur(voxern);
				// On l'enlève aux voxerns en cours
				voxEnCours.removeVecteur(voxern);
			}
			// On remet le thread en veille
			threads[tmpInt].setSleep();
		}
		
		// On lache le CPU 1 secondes
		QThread::currentThread()->wait(1);
	}
	
	if(erreurCritique)
	{
		stdlog->message(LOGUTIL_CRITICAL, "Erreur dans la boucle de gestion de la propagation");
		for(int i = 0; i<MaxThreads; i++)
			threads[i].terminate();
		delete [] threads;
		return false;
	}
	
	stdlog->message(LOGUTIL_INFO, "Fin de la propagation");
	
	// Suppression des threads de calcul
	delete [] threads;
	
	stdlog->message(LOGUTIL_INFO, "Threads de calcul supprimés");
	
	// Fin sans problème
	return true;
}
