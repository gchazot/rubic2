#ifndef GLOBALS_H
#define GLOBALS_H

/** \file globals.h
  * \brief Contient les déclarations de toutes les variables globales au programme rubicalc
  */

// Inclusions
#include "logutil.h"
#include "rubicparams.h"
#include "rubicimage.h"
#include "rubicfiles.h"
#include "dvect.h"
#include "lvect.h"
#include "dvect24.h"
#include "rubicresults.h"
#include "rubicresults_point.h"
#include "lvectuniquequeue.h"
#include "lvectqueueelt.h"

/// Le système de log global
extern LogUtil *stdlog;
/// Le système de log du calcul
extern LogUtil *calclog;

/// Les paramètres de l'application
extern RubicParams *params;

/// Les fichiers de l'image de référence
extern RubicFiles *refFiles;
/// L'image de référence
extern RubicImage *refImg;

/// Les fichiers de l'image déformée
extern RubicFiles *defFiles;
/// L'image déformée
extern RubicImage* defImg;

/// Les résultats du calcul
extern RubicResults *resultats;

/// Taille des voxerns
extern long Lcur;

/// Niveau de gris mini à prendre en compte
extern unsigned char NGMin;
/// Niveau de gris maxi à prendre en compte
extern unsigned char NGMax;

/// Nombre maximal de threads executables en même temps
extern int MaxThreads;

/// Utilisation de l'interpolation par splines cubiques
extern bool InterpolCubiq;

/// Point de départ dans l'image initiale (coordonnées Voxel !)
extern LVect Iv;
/// Point de départ dans l'image initiale (coordonnées Voxerns !)
extern LVect Ivoxern;
/// Champs de déplacement initial
extern DVect24 U24;

/// Coordonnées mini de la zone de calcul
extern LVect minZone;
/// Coordonnées maxi de la zone de calcul
extern LVect maxZone;

/// Nombre de voxerns à calcules dans chaque dimension.
extern LVect nbVoxerns;

/// Pas du scanning initial
extern double pasScanning;
/// Taille de la zone de scanning
extern double tailleZoneScanning;

/// Précision désirée pour le gradient conjugué
extern double precGradConj;
/// Précision désirée pour le Newton du gradient conjugué
extern double precNewtonGradConj;
/// Pas de calcul du Newton dans les gradients conjugués
extern double pasNewtonGradConj;

/// Maximum d'itérations pour le gradient conjugué
extern long maxitGC;
/// Maximum d'itérations pour le Newton dans le gradient conjugué
extern long maxitNewtonGC;

/// Liste des voxerns pouvant être calculés
extern LVectUniqueQueue voxCalculables;
/// Liste des voxerns en cours de calcul
extern LVectUniqueQueue voxEnCours;
/// Liste des voxerns déjà calculés
extern LVectUniqueQueue voxCalcules;
/// Liste des voxerns qui n'ont pas pu être caclulés à cause d'une erreur
extern LVectUniqueQueue voxErrones;

/// Nom de fichier pour la sortie des résultats au format ASCII
extern QString ASCIIFileName;
/// Nom de fichier pour la sortie des résultats au format INP Abaqus
extern QString INPFileName;

/// Fonction de nettoyage pour quitter proprement le programme
/** Il FAUT utiliser cette fonction pour supprimer la mémoire allouée
  * dynamiquement aux variables globales avant de quitter le programme.
  */
void quitter(int retval);

#endif //GLOBALS_H
