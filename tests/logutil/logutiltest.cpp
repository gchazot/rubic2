#include "logutil.h"

/** Ce test vérifie le bon fonctionnement de la classe LogUtil
** proposée par librubic
*/

int main(int, char**)
{
	// Log dans la sortie standard avec les valeurs par défaut
	{
		LogUtil log1;
		log1.message(LOGUTIL_DEBUG, "Un message de debug");
		log1.message(LOGUTIL_INFO, "Un message d'info");
		log1.message(LOGUTIL_WARNING, "Un message d'avertissement");
		log1.message(LOGUTIL_ERROR, "Un message d'erreur");
		log1.message(LOGUTIL_CRITICAL, "Un message d'erreur critique");
	}
	
	// Log dans la sortie standard avec affichage de tous les messages
	{
		LogUtil log1(LOG_TO_STDOUT, LOGUTIL_DEBUG);
		log1.message(LOGUTIL_DEBUG, "Un message de debug");
		log1.message(LOGUTIL_INFO, "Un message d'info");
		log1.message(LOGUTIL_WARNING, "Un message d'avertissement");
		log1.message(LOGUTIL_ERROR, "Un message d'erreur");
		log1.message(LOGUTIL_CRITICAL, "Un message d'erreur critique");
	}
	
	// Log dans la sortie d'erreur avec affichage de tous les messages
	{
		LogUtil log1(LOG_TO_STDERR, LOGUTIL_DEBUG);
		log1.message(LOGUTIL_DEBUG, "Un message de debug");
		log1.message(LOGUTIL_INFO, "Un message d'info");
		log1.message(LOGUTIL_WARNING, "Un message d'avertissement");
		log1.message(LOGUTIL_ERROR, "Un message d'erreur");
		log1.message(LOGUTIL_CRITICAL, "Un message d'erreur critique");
	}
	
	// Log dans un fichier avec affichage de tous les messages
	{
		LogUtil log1("testlogutil.log", LOGUTIL_DEBUG);
		log1.message(LOGUTIL_DEBUG, "Un message de debug");
		log1.message(LOGUTIL_INFO, "Un message d'info");
		log1.message(LOGUTIL_WARNING, "Un message d'avertissement");
		log1.message(LOGUTIL_ERROR, "Un message d'erreur");
		log1.message(LOGUTIL_CRITICAL, "Un message d'erreur critique");
	}
	
	// Test des autres methodes d'envoi de message
	{
		LogUtil log1(LOG_TO_STDOUT, LOGUTIL_DEBUG);
		log1.message(LOGUTIL_DEBUG, __FILE__, __LINE__, "Un message de debug");
		log1.DEBUGMESS("Un autre message de debug");
		log1(LOGUTIL_WARNING, "Un message d'avertissement");
	}
}
