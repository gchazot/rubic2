/** \file logutil.cpp
  * \brief Contient les définitions des fonctions membres de la classe LogUtil
  */

#include "logutil.h"
#include "constants.h"

#include <qstring.h>
#include <QTextStream>
#include <qfile.h>
#include <qdatetime.h>
#include <qstringlist.h>
#include <qmutex.h>

#include <iostream>
using namespace std;

/**********************************************************************
****                 Constructeurs et Destructeur                  ****
**********************************************************************/
// Constructeur par défaut
LogUtil::LogUtil(void)
{
	mMutex = new QMutex();
	
	mOut = new QTextStream(stdout);
	mLogFile = NULL;
	mLogFileName = LOG_TO_STDOUT;
	mLogLevel = LOGUTIL_WARNING;
	mIsReady = true;
	
	if(mOut == NULL)
	{
		mIsReady = false;
		return;
	}
	
	// En-tête de log
	*mOut << "###########################################################" << endl;
	*mOut << "Fichier de log \"" << mLogFileName << "\" initialisé." << endl;
	*mOut << QDateTime::currentDateTime().toString() << endl;
	*mOut << "############################" << endl;
}

// Constructeur avec définition du nom du fichier de sortie et du
// niveau de log désiré.
LogUtil::LogUtil(const QString& logFileName, int logLevel)
{
	mMutex = new QMutex();
	
	mLogFileName = logFileName;
	mLogLevel = logLevel;
	
	if(mLogFileName == LOG_TO_STDOUT)
	{
		mLogFile = NULL;
		mOut = new QTextStream(stdout);
		mIsReady = true;
	}
	else if(mLogFileName == LOG_TO_STDERR)
	{
		mLogFile = NULL;
		mOut = new QTextStream(stderr);
		mIsReady = true;
	}
	else
	{
		mLogFile = new QFile(mLogFileName);
		mIsReady = mLogFile->open(QIODevice::Truncate | QIODevice::WriteOnly);
		mOut = new QTextStream(mLogFile);
	}
	
	if(mOut == NULL)
	{
		mIsReady = false;
		return;
	}
	
	*mOut << "###########################################################" << endl;
	*mOut << "Fichier de log \"" << mLogFileName << "\" initialisé." << endl;
	*mOut << QDateTime::currentDateTime().toString() << endl;
	*mOut << "############################" << endl;
}

// Destructeur
LogUtil::~LogUtil()
{
	if(mOut != NULL)
	{
		*mOut << "############################" << endl;
		*mOut << "Fin du Log." << endl;
		*mOut << QDateTime::currentDateTime().toString() << endl;
		*mOut << "###########################################################" << endl;
		
		delete mOut;
	}
	
	if(mLogFile != NULL)
	{
		mLogFile->flush();
		mLogFile->close();
		delete mLogFile;
	}
	
	if(mMutex != NULL)
		delete mMutex;
}

/**********************************************************************
****    Fonctions de modification des paramètres de logging        ****
**********************************************************************/
// Récupération du niveau de logging
int LogUtil::getLogLevel(void) const
{
	mMutex->lock();
	int result = mLogLevel;
	mMutex->unlock();
	return result;
}

// Modification du niveau de logging
void LogUtil::setLogLevel(int newLevel)
{
	mMutex->lock();
	mLogLevel = newLevel;
	mMutex->unlock();
}

// Récupération du nom du fichier de log
QString LogUtil::getLogFileName(void) const
{
	mMutex->lock();
	QString result = mLogFileName;
	mMutex->unlock();
	return result;
}

// Vérifie si l'objet est prêt à écrire des messages
bool LogUtil::isReady(void) const
{
	mMutex->lock();
	bool result = mIsReady;
	mMutex->unlock();
	return result;
}

/**********************************************************************
****           Fonctions d'écriture des messages de log            ****
**********************************************************************/
// Ecrit un message dans le log avec le niveau donné en paramètre. Si
// le niveau donné est inférieur strictement au niveau limite de
// l'objet, alors rien n'est écrit.
// Le message est écrit avec chaque ligne précédée du niveau de log
// suivi de ": ".
inline void LogUtil::message(int level, const QString& text) const
{
	mMutex->lock();
	if( (level < mLogLevel) || (! mIsReady) )
	{
		mMutex->unlock();
		return;
	}
		
	QStringList lignes = QStringList::split( RET_LIGNE, text, true);
	QStringList::Iterator it;
	
	for(it = lignes.begin(); it != lignes.end(); ++it)
	{
		*mOut << level << ": " << *it << endl;
	}
	
	// Vidage du tampon
	*mOut << flush;
	
	mMutex->unlock();
}

// Ecrit un message avec en plus des précisions sur l'endroit d'où le
// message a été écrit. Cette fonction peut être appellée directement
// ou alors remplacée par la macro dont la définition suit.
inline void LogUtil::message(int level, const QString& file, const int line, const QString& text) const
{
	mMutex->lock();
	if( (level < mLogLevel) || (! mIsReady) )
	{
		mMutex->unlock();
		return;
	}
		
	QStringList lignes = QStringList::split( RET_LIGNE, text, true);
	QStringList::Iterator it;
	
	for(it = lignes.begin(); it != lignes.end(); ++it)
	{
		*mOut << level << ": " << file << ":" << line << " - ";
		*mOut << *it << endl;
	}
	
	// Vidage du tampon
	*mOut << flush;
	
	mMutex->unlock();
}


// Opérateur permettant d'utiliser l'objet comme une fonction de log
inline void LogUtil::operator()(int level, const QString& text) const
{
	message(level, text);
}
