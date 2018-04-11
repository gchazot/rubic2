#ifndef LOGUTIL_H
#define LOGUTIL_H

/** \file logutil.h
  * \brief Contient la définition de la classe LogUtil.
  */

#include <qstring.h>
class QFile;
class QTextStream;
class QMutex;

/**********************************************************************
****                   Définition des constantes                   ****
**********************************************************************/
/// Nom de fichier à utiliser pour spécifier un log sur la sortie standard
#define LOG_TO_STDOUT "!! log_to_stdout !!"
/// Nom de fichier à utiliser pour spécifier un log sur la sortie d'erreur
#define LOG_TO_STDERR "!! log_to_stderr !!"

/// Pour un message de debug
#define LOGUTIL_DEBUG		1
/// Pour un message d'information
#define LOGUTIL_INFO		2
/// Pour un message d'avertissement
#define LOGUTIL_WARNING		3
/// Pour un message d'erreur non-fatale
#define LOGUTIL_ERROR		4
/// Pour un message d'erreur fatale
#define LOGUTIL_CRITICAL	5
/// Pour spécifier qu'aucun message ne doit passer dans le log
#define LOGUTIL_NOLOG		10


/// Classe de gestion des messages de log des programmes rubic.
/** Cette classe permet d'écrire des messages dans un fichier de log en fonction
  * du niveau de détail désiré. Le fichier de log choisi peut être une sortie à
  * l'écran. Pour choisir l'affichage à l'écran comme sortie des messages de log
  * il faut spécifier la constante LOG_TO_STDOUT ou LOG_TO_STDERR comme nom de
  * fichier de log.
  * Les niveaux de log sont définis comme des constantes dans la classe, ils sont
  * au nombre de 3, par ordre de priorité:
  *  1) LOGUTIL_DEBUG :	Message de débogage
  *  2) LOGUTIL_INFO :	Message d'information
  *  3) LOGUTIL_WARNING :	Message d'avertissement
  *  4) LOGUTIL_ERROR :	Message d'erreur (n'entrainant pas la fin du programme)
  *  5) LOGUTIL_CRITICAL : Message d'erreur (entrainant la fin du  programme)
  *  6) LOGUTIL_NOLOG :	Aucun message
  */
class LogUtil
{
public:
	/**********************************************************************
	****                 Constructeurs et Destructeur                  ****
	**********************************************************************/
	/// Constructeur par défaut
	LogUtil(void);
	/// Constructeur avec définition du nom du fichier de sortie et du niveau de log désiré.
	LogUtil(const QString& logFileName, int logLevel);
	/// Destructeur
	~LogUtil();
	
private:
	/**********************************************************************
	****                Stockage des variables internes                ****
	**********************************************************************/
	/// Niveau de log demandé
	int mLogLevel;
	/// Nom du fichier de sortie des messages de log
	QString mLogFileName;
	/// Fichier de sortie des messages de log (si utile)
	QFile* mLogFile;
	/// Stream de sortie
	/** Objet permettant les sorties de flux vers le fichier de log ou la
	  * sortie standard ou la sortie d'erreur.
	  */
	QTextStream* mOut;
	
	/// Vérou global de l'objet
	QMutex* mMutex;
	
	/// Définit si l'objet est prêt à recevoir des écritures.
	bool mIsReady;

public:
	/**********************************************************************
	****    Fonctions de modification des paramètres de logging        ****
	**********************************************************************/
	/// Récupération du niveau de logging
	int getLogLevel(void) const;
	/// Modification du niveau de logging
	void setLogLevel(int newLevel);
	
	/// Récupération du nom du fichier de log
	QString getLogFileName(void) const;

	/// Vérifie si l'objet est prêt à écrire des messages
	bool isReady(void) const;
	
public:
	/**********************************************************************
	****           Fonctions d'écriture des messages de log            ****
	**********************************************************************/
	/// Ecrire un message dans le log
	/** Ecrit un message dans le log avec le niveau donné en paramètre. Si
	  * le niveau donné est inférieur strictement au niveau limite de
	  * l'objet, alors rien n'est écrit.
	  * Le message est écrit avec chaque ligne précédée du niveau de log
	  * suivi de ": ".
	  */
	void message(int level, const QString& text) const;
	/// Ecrire un message dans le log en spécifiant un nom de fichier et une ligne
	/** Ecrit un message avec en plus des précisions sur l'endroit d'où le
	  * message a été écrit. Cette fonction peut être appellée directement
	  * ou alors remplacée par la macro DEBUGMESS dont la définition suit.
	  */
	void message(int level, const QString& file, const int line, const QString& text) const;
	/// Macro d'aide aux messages de debogage
	/** Cette macro s'utilise comme une fonction membre de la classe LogUtil.
	  * Elle permet d'envoyer facilement un message au niveau DEBUG avec
	  * le fichier et la ligne d'où le message a été envoyé spécifié en
	  * début de ligne. C'est la principale fonction de débogage.
	  */
	#define DEBUGMESS(text) message(LOGUTIL_DEBUG, __FILE__, __LINE__, text)
	
	/// Opérateur permettant d'utiliser l'objet comme une fonction de log
	void operator()(int level, const QString& text) const;
};
	
#endif //LOGUTIL_H
