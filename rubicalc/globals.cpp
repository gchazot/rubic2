// Inclusions
#include "globals.h"

/// Le système de log global
LogUtil *stdlog;
/// Le système de log du calcul
LogUtil *calclog;

/// Les paramètres de l'application
RubicParams *params;

/// Les fichiers de l'image de référence
RubicFiles *refFiles;
/// L'image de référence
RubicImage *refImg;

/// Les fichiers de l'image déformée
RubicFiles *defFiles;
/// L'image déformée
RubicImage* defImg;

/// Les résultats du calcul
RubicResults *resultats;

/// Taille des voxerns
long Lcur;

/// Niveau de gris mini à prendre en compte
unsigned char NGMin;
/// Niveau de gris maxi à prendre en compte
unsigned char NGMax;

/// Nombre maximal de threads executables en même temps
int MaxThreads;

/// Utilisation de l'interpolation par splines cubiques
bool InterpolCubiq;

/// Point de départ dans l'image initiale (coordonnées Voxel !)
LVect Iv;
/// Point de départ dans l'image initiale (coordonnées Voxerns !)
LVect Ivoxern;
/// Champs de déplacement initial
DVect24 U24;

/// Coordonnées mini de la zone de calcul
LVect minZone;
/// Coordonnées maxi de la zone de calcul
LVect maxZone;

/// Nombre de voxerns à calcules dans chaque dimension.
LVect nbVoxerns;

/// Pas du scanning initial
double pasScanning;
/// Taille de la zone de scanning
double tailleZoneScanning;

/// Précision désirée pour le gradient conjugué
double precGradConj;
/// Précision désirée pour le Newton du gradient conjugué
double precNewtonGradConj;
/// Pas de calcul du Newton dans les gradients conjugués
double pasNewtonGradConj;

/// Maximum d'itérations pour le gradient conjugué
long maxitGC;
/// Maximum d'itérations pour le Newton dans le gradient conjugué
long maxitNewtonGC;

/// Liste des voxerns pouvant être calculés
LVectUniqueQueue voxCalculables;
/// Liste des voxerns en cours de calcul
LVectUniqueQueue voxEnCours;
/// Liste des voxerns déjà calculés
LVectUniqueQueue voxCalcules;
/// Liste des voxerns qui n'ont pas pu être caclulés à cause d'une erreur
LVectUniqueQueue voxErrones;

/// Nom de fichier pour la sortie des résultats au format ASCII
QString ASCIIFileName;
/// Nom de fichier pour la sortie des résultats au format INP Abaqus
QString INPFileName;


void quitter(int retval)
{
	// Libération de la mémoire des variables globales
	if(calclog != NULL)
		delete calclog;
	if(params != NULL)
		delete params;
	if(refImg != NULL)
		delete refImg;
	if(refFiles != NULL)
		delete refFiles;
	if(defImg != NULL)
		delete defImg;
	if(defFiles != NULL)
		delete defFiles;	
	if(resultats != NULL)
		delete resultats;
		
	stdlog->message( LOGUTIL_INFO, QString("Code de sortie : %1").arg(retval) );
	if(stdlog != NULL)
		delete stdlog;	

	exit(retval);
}
