#include "lvect.h"
#include "lvectuniquequeue.h"

#include <qthread.h>
#include <qmutex.h>
#include <iostream>
using namespace std;

// Nombre d'éléments à ajouter à la liste (i*i)
#define TAILLE 5
// Nombre de threads à créer
#define NBTHR 3

// Classe d'ajout et de retrait de vecteurs dans la liste
class AddRemove: public QThread
{
public:
	// Constructeur
	AddRemove(int basis, LVectUniqueQueue * queue, QMutex* outMutex)
	{
		mBasis = basis;
		mQueue = queue;
		mOutMutex = outMutex;
	}
	// Destructeur
	~AddRemove() {}

private:
	// La base des vecteurs
	int mBasis;
	// La liste à gérer
	LVectUniqueQueue *mQueue;
	// Le mutex pour la sortie standard
	QMutex *mOutMutex;

public:
	// Fonction executée par le thread
	void run(void)
	{
		LVect tmpVec;
		for(int j = 0; j < TAILLE; j++)
		{
			tmpVec = LVect(mBasis, j, j);
			mQueue->addVecteur(tmpVec);
			mOutMutex->lock();
			cout << tmpVec.toString().toStdString() << endl << flush;
			mOutMutex->unlock();
			msleep(250);
		}
	}
};

// Le programme principal
int main(int, char**)
{
	// Création de la liste
	LVectUniqueQueue queue;
	
	// Création du mutex sur la sortie standard
	QMutex outMutex;
	
	// Création des threads
	AddRemove *threads[NBTHR+1];
	for(short i = 0; i < NBTHR; i++)
	{
		threads[i] = new AddRemove(i, &queue, &outMutex);
		if(threads[i] == NULL)
			return 10;
	}
	// Ajout d'un thread redondant
	threads[NBTHR] = new AddRemove(NBTHR-1, &queue, &outMutex);
	if(threads[NBTHR] == NULL)
		return 11;
			
	// Lancement des threads
	for(short i = 0; i < NBTHR+1; i++)
	{
		threads[i]->start();
	}
	
	// Attente de la fin des threads
	bool fin;
	do
	{
		fin = true;
		if(NBTHR > 0)
			threads[0]->wait(1000);
			
		for(int i = 0; i < NBTHR; i++)
		{
			if(! threads[i]->isFinished())
				fin = false;
		}
	}while(! fin);
	
	// On vérifie que tous les trucs y sont
	bool tous = true;
	for(int i = 0; i < NBTHR; i++)
		for(int j = 0; j < TAILLE; j++)
		{
			LVect tmpVec(i,j,j);
			if( ! queue.contains(tmpVec) )
				tous = false;
		}
	
	cout << "Résultat : " << (tous?"Ils y sont tous":"Il en manque") << endl;
	cout << "Taille :   " << queue.getSize() << endl;
	
	// Affichage de la liste
	cout << queue.toString().toStdString();
	
	// On supprime une partie des vecteurs de la liste
	int nbsuppr = 0;
	for(int i = 0; i < NBTHR; i++)
	{
		LVect tmpVec(i,i,i);
		if( queue.removeVecteur(tmpVec) )
			nbsuppr++;
	}
	cout << "Nombre supprimé / Tentatives de suppression : " << nbsuppr << " / " << NBTHR << endl;
	
	// Affichage de la liste
	cout << queue.toString().toStdString();
	
	// Fin normale du programme
	return 0;
}
