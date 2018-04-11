#ifndef SCANNINGTHREAD_H
#define SCANNINGTHREAD_H

/** \file scanningthread.h
  * \brief Contient la déclaration de la classe ScanningThread.
  */

#include <qthread.h>
#include <QSemaphore>
#include <qmutex.h>
#include <qstring.h>

#include "lvect.h"
#include "dvect.h"

#include "fonctions.h"

/// Classe de gestion d'un thread de scanning
/** Effectue le calcul du scanning pour un somment du cube, dans un thread
  * séparé.
  */
class ScanningThread: public QThread
{
public:
	/// Constructeur
	/** Constructeur donnant le point de départ dans l'image initiale,
	  * dans l'image déformée et le pointeur vers le minimum trouvé.
	  */
	ScanningThread(LVect& iniPnt, DVect& defPnt, DVect *minPnt, double* resultat, QSemaphore *sem, QString  name);
	/// Destructeur
	~ScanningThread();
	
	/// Fonction executée lors du lancement du Thread
	virtual void run();
	
private:
	/// Le point central du voxern dans l'image initiale
	LVect mIniPnt;
	/// Le point central du voxern dans l'image déformée
	DVect mDefPnt;
	
	/// Le point où la correlation est minimale
	DVect* mMinPnt;
	
	/// La correlation minimale
	double* mResultat;
	
	/// Le sémaphore limitant le nombre de threads en cours d'éxecution
	QSemaphore* mSem;
	
	/// Variable disant si le thread a fini le calcul
	bool mFinished;
	/// Mutex de protection de la variable mFinished
	QMutex mFinishedMutex;
	
	/// Nom du sommet calculé par le thread
	QString mName;
public:
	/// Vérifie si le thread a fini son calcul
	/** Cette fonction est thread-safe et peut donc être appelée sans condition.
	  */
	bool isFinished();
};

#endif //SCANNINGTHREAD_H
