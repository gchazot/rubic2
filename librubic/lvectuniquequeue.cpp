/** \file lvectuniquequeue.cpp
  * \brief Contient la définition des fonctions membres de la classe LVectUniqueQueue
  */

#include "lvect.h"
#include "lvectuniquequeue.h"
#include "lvectqueueelt.h"

#include <cstdlib>
#include <QTextStream>

// Constructeur
LVectUniqueQueue::LVectUniqueQueue()
{
	list = NULL;
	count = 0;
}

// Constructeur de recopie
LVectUniqueQueue::LVectUniqueQueue(LVectUniqueQueue& ini)
{
	list = NULL;
	count = 0;
	
	LVectQueueElt * iter1 = ini.list;
	LVectQueueElt ** iter2 = &list;
	while(iter1 != NULL)
	{
		*iter2 = new LVectQueueElt(*iter1);
		
		count++;
		
		iter2 = &((*iter2)->next);
		
		iter1 = iter1->next;
	}
}

// Destructeur
LVectUniqueQueue::~LVectUniqueQueue()
{
	LVectQueueElt * tmp;
	while(list != NULL)
	{
		tmp = list;
		list = list->next;
		delete tmp;
	}
}

// Ajout d'un élement à la file
/* Avant d'ajouter un vecteur à la liste, la fonction vérifie qu'il
* n'y en a pas d'autre identique. Si un autre vecteur identique est
* trouvé, la fonction ne l'ajoute pas. Si aucun vecteur identique
* n'est trouvé, la fonction ajoute le nouveau vecteur à la fin de la
* liste.
*/
inline void LVectUniqueQueue::addVecteur(const LVect& vec)
{
	mMutex.lock();
	LVectQueueElt *iter = list;
	LVectQueueElt *prev = NULL;
	// On parcours la liste
	while(iter != NULL)
	{	
		// On vérifie que ce n'est pas le même (anti-doublon)
		if( (*iter) == vec )
		{
			mMutex.unlock();
			return;
		}
		prev = iter;
		iter = iter->next;
	}
	
	// Si la liste n'était pas vide ...
	if(prev != NULL)
		// ... on ajoute à la fin
		prev->next = new LVectQueueElt(vec);
	// Sinon ...
	else
		// ... on crée la liste
		list = new LVectQueueElt(vec);
	
	// Mise à jour du compteur
	count++;
	mMutex.unlock();
}

// Récupération d'un élément de la file sans suppression.
/* Renvoie le premier élément de la file sans le supprimer de la
* file. Si il n'y a plus rien dans la file, renvoie NULL;
*/
inline LVect* LVectUniqueQueue::getVecteur(void)
{
	
	mMutex.lock();
	LVect* res = list;
	mMutex.unlock();
	return res;
}	

// Récupération d'un élément de la file avec suppréssion.
/* Renvoie le premier élément de la file et le supprime de la file. Si
* il n'y a plus rien dans la file, renvoie NULL;
*/
inline LVect* LVectUniqueQueue::popVecteur(void)
{
	mMutex.lock();
	LVectQueueElt *iter = list;
	if(list != NULL)
		list = list->next;
	count--;
	mMutex.unlock();
	return iter;
}
// Suppression d'un vecteur d'après son contenu
inline bool LVectUniqueQueue::removeVecteur(LVect& vec)
{
	mMutex.lock();
	LVectQueueElt *iter = list;
	LVectQueueElt *prev = NULL;
	// On parcours la liste
	while(iter != NULL)
	{
		// On cherche le même vecteur
		if( (*iter) == vec )
		{
			// On l'a trouvé
			// Si c'est le premier de la liste ...
			if(prev == NULL)
				// ... on change le début de la liste
				list = list->next;
			// Sinon ...
			else
				// ... on l'enlève de la séquence
				prev->next = iter->next;
			// On libère la méméoire de l'objet
			delete iter;
			count--;
			mMutex.unlock();
			return true;
		}
		prev = iter;
		iter = iter->next;
	}
	mMutex.unlock();
	return false;
}

inline LVect* LVectUniqueQueue::getVecteur(long index)
{
	mMutex.lock();
	LVectQueueElt *iter = list;
	long i;
	for(i = 0; i < index; i++)
		if(iter != NULL)
			iter = iter->next;
		else
		{
			mMutex.unlock();
			return NULL;
		}
	mMutex.unlock();
	return iter;
}

// Récupère le nombre d'éléments de la file
inline uint LVectUniqueQueue::getSize(void)
{
	mMutex.lock();
	uint res = count;
	mMutex.unlock();
	return res;
}

// Vérifie si un élément existe déjà dans la liste
inline bool LVectUniqueQueue::contains(LVect& vec)
{
	mMutex.lock();
	LVectQueueElt *iter = list;
	while(iter != NULL)
	{	
		if( (*iter) == vec )
		{
			mMutex.unlock();
			return true;
		}
		iter = iter->next;
	}
	mMutex.unlock();
	return false;
}

// Rendu en chaine de caractères
QString LVectUniqueQueue::toString(void)
{
	mMutex.lock();
	LVectQueueElt *iter = list;
	QString result;
	QTextStream stream(&result);
	
	// Pour chaque élement de la liste ...
	while(iter != NULL)
	{
		// ... on affiche une ligne avec le vecteur
		stream << iter->toString() << endl;
		iter = iter->next;
	}
	
	mMutex.unlock();
	return result;
}

uint LVectUniqueQueue::findVecteur(LVect& vec)
{
	mMutex.lock();
	LVectQueueElt *iter = list;
	uint pos = 0;
	while(iter != NULL)
	{	
		if( (*iter) == vec )
		{
			mMutex.unlock();
			return pos;
		}
		iter = iter->next;
		pos++;
	}
	mMutex.unlock();
	return ~0;
}
