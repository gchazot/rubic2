#ifndef LVECTUNIQUEQUEUE_H
#define LVECTUNIQUEQUEUE_H
/** \file lvectuniquequeue.h
  * \brief Contient la définition de la classe LVectUniqueQueue
  */

#include <qmutex.h>
#include <qstring.h>

class LVect;
class LVectQueueElt;

/// Classe de queue (FIFO) de LVect sans doublons.
/** Implémente une file (FIFO) de LVect avec gestion des doublons.
  * La file ne peut pas contenir de doublons.
  *
  * La classe est thread-safe pour ce qui concerne l'ajout et la suppression
  * d'élements.
  */
class LVectUniqueQueue
{
public:
	/// Constructeur
	LVectUniqueQueue();
	/// Constructeur de recopie
	LVectUniqueQueue(LVectUniqueQueue& ini);
	/// Destructeur
	/** Le destructeur nettoir le contenu restant dans la liste en
	  * supprimant la totalité des vecteurs restants.
	  */
	~LVectUniqueQueue();

private:
	/// Mutex de gestion des accès à la liste
	QMutex mMutex;
	/// Pointeur vers la liste
	LVectQueueElt *list;
	
	/// Compteur d'éléments
	uint count;

public:
	/// Ajout d'un élement à la file
	/** Avant d'ajouter un vecteur à la liste, la fonction vérifie qu'il
	  * n'y en a pas d'autre identique. Si un autre vecteur identique est
	  * trouvé, la fonction ne l'ajoute pas. Si aucun vecteur identique
	  * n'est trouvé, la fonction ajoute le nouveau vecteur à la fin de la
	  * liste.
	  */
	void addVecteur(const LVect& vec);
	
	/// Récupération d'un élément de la file sans suppression.
	/** Renvoie le premier élément de la file sans le supprimer de la
	  * file. Si il n'y a plus rien dans la file, renvoie NULL;
	  */
	LVect* getVecteur(void);
	
	/// Récupération d'un élément de la file avec suppréssion.
	/** Renvoie le premier élément de la file et le supprime de la file. Si
	  * il n'y a plus rien dans la file, renvoie NULL;
	  */
	LVect* popVecteur(void);
	
	/// Suppression d'un vecteur d'après son contenu
	/** Renvoie true si le vecteur a été trouvé, false sinon
	  */
	bool removeVecteur(LVect& vec);
	
	/// Récupération d'un élément de la file sans suppression d'après son numéro d'ordre
	/** Renvoie le Nème élément de la file sans le supprimer de la file.
	  * Si il n'y a plus rien dans la file ou si N est supérieur au nombre
	  * d'élements de la liste, renvoie NULL;
	  */
	LVect* getVecteur(long index);
	
	/// Récupère le nombre d'éléments de la file
	uint getSize(void);
	
	/// Vérifie si un élément existe déjà dans la liste
	/** Renvoie true si l'élement existe déjà et false sinon.
	  */
	bool contains(LVect& vec);
	
	/// Rendu en chaine de caractères
	/** Renvoie une chaine de caractères représentant le contenu de la
	  * liste. Chaque vecteur est sur une ligne séparée et le premier de la
	  * liste est en haut.
	  */
	QString toString(void);
	
	/// Récupération de la position d'un vecteur dans la liste
	/** Renvoie l'entier représentant la position du vecteur donné dans la liste.
	  * Si le vecteur n'est pas dans la liste, renvoie -1.
	  */
	uint findVecteur(LVect& vec);
	
};

#endif //LVECTUNIQUEQUEUE_H
