/** \file scanningthread.cpp
  * \brief Contient la définition des membres de ScanningThread.
  */
  
#include "scanningthread.h"
#include "globals.h"

ScanningThread::ScanningThread(LVect& iniPnt, DVect& defPnt, DVect *minPnt, double* resultat, QSemaphore *sem, QString Name)
{
	mIniPnt = iniPnt;
	mDefPnt = defPnt;
	mMinPnt = minPnt;
	
	mResultat = resultat;
	mSem = sem;
	mFinished = false;
	
	mName = Name;
}

ScanningThread::~ScanningThread()
{
}

void ScanningThread::run()
{
	mSem->acquire();
	
	stdlog->message(LOGUTIL_INFO, "Début du scanning sur le sommet " + mName);
	
	*mResultat = scanningPoint(mIniPnt, mDefPnt, mMinPnt);
		
	mFinishedMutex.lock();
	mFinished = true;
	mFinishedMutex.unlock();
	
	stdlog->message(LOGUTIL_INFO, "Fin du scanning sur le sommet " + mName);
	
	mSem->release();
}

bool ScanningThread::isFinished()
{
	mFinishedMutex.lock();
	bool result = mFinished;
	mFinishedMutex.unlock();
	
	return result;
}
