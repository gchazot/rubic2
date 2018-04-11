#ifndef RUBICPARAMS_NAMES
#define RUBICPARAMS_NAMES
/** \file rubicparams_names.h
  * \brief Noms des paramètres et des groupes dans le fichier de commande pour RubicParams
  *
  * Ce fichier définit les constantes chaine de caractères utilisées dans les
  * fichiers de commande pour nommer les groupes et les paramètres utilisés par
  * rubic
  *
  * \sa RubicParams
  */

/******************************************************************************
****                              Groupes                                  ****
******************************************************************************/
/// GROUPE : Fichiers d'entrée-sortie
#define RPN_GRP_IOFILES			"Fichiers_IO"
/// GROUPE : Paramètres de calcul
#define RPN_GRP_CALCPARAMS		"Params_Calcul"
/// GROUPE : Paramètres des algorithmes de calcul
#define RPN_GRP_ALGOPARAMS		"Params_Algos"

/******************************************************************************
****               Paramètres pour "Fichiers d'entrée-sortie"              ****
******************************************************************************/
/// Nom du fichier vgi de l'image de référence
#define RPN_IOFILES_VGIREF		"Vgi_Reference"
/// Nom du fichier vgi de l'image déformée
#define RPN_IOFILES_VGIDEF		"Vgi_Deformee"
/// Fichier de sortie des résultats (ASCII)
#define RPN_IOFILES_ASCIIOUT		"Sortie_ASCII"
/// Fichier de sortie des résultats (INP Abaqus)
#define RPN_IOFILES_INPOUT		"Sortie_INP"
/// Fichier de log du programme
#define RPN_IOFILES_LOGOUT		"Sortie_Log"

/******************************************************************************
****                 Paramètres pour "Paramètres de calcul"                ****
******************************************************************************/
/// Taille d'un voxern
#define RPN_CALCPARAMS_VOXSIZE		"Taille_Voxern"
/// Coordonnées mini de la zone dans laquelle effectuer le calcul
#define RPN_CALCPARAMS_ZONEMIN		"ZoneCalc_Min"
/// Coordonnées maxi de la zone dans laquelle effectuer le calcul
#define RPN_CALCPARAMS_ZONEMAX		"ZoneCalc_Max"

/// Point de départ du calcul dans l'image de référence
#define RPN_CALCPARAMS_STARTREF		"Depart_Ref"
/// Point de départ du calcul dans l'image déformée
#define RPN_CALCPARAMS_STARTDEF		"Depart_Def"

/// Niveau de gris minimal à prendre en compte dans les images
#define RPN_CALCPARAMS_NG_MIN		"NiveauGris_Mini"
/// Niveau de gris maximal à prendre en compte dans les images
#define RPN_CALCPARAMS_NG_MAX		"NiveauGris_Maxi"

/// Nombre maximal de threads executables en même temps
#define RPN_CALCPARAMS_MAXTHREADS	"Max_Threads"

/// Utilisation de l'interpolation par splines cubiques?
#define RPN_CALCPARAMS_INTERPOLCUBIQ	"Interpolation_Cubique"

/******************************************************************************
****                Paramètres des algorithmes de calcul                   ****
******************************************************************************/
/// Taille de la zone de recherche en scanning pour le premier voxern
#define RPN_ALGOPARAMS_ZONESCAN		"Taille_Zone_Scanning"

/// Pas de calcul du scanning initial
#define RPN_ALGOPARAMS_PASSCAN		"Pas_Scanning"
// /// Pas de calcul des dérivées dans le gradient conjugué
/// Pas de calcul dans le Newton interne au gradient conjugué
#define RPN_ALGOPARAMS_PASNEWTONGC	"Pas_NewtonGradConj"

/// Précision désirée pour la convergence du scanning
#define RPN_ALGOPARAMS_PRECSCAN		"Prec_Scanning"
/// Précision désirée pour la convergence du gradient conjugué
#define RPN_ALGOPARAMS_PRECGRADCCONJ	"Prec_GradConj"
/// Précision désirée pour la convergence du Newton dans le gradient conjugué.
#define RPN_ALGOPARAMS_PRECNEWTONGC	"Prec_NewtonGradConj"

/// Nombre maximum d'itérations pour la convergence du gradient conjugué
#define RPN_ALGOPARAMS_MAXITGRADCONJ	"MaxIter_GradConj"
/// Nombre maximum d'itérations pour la convergence du Newton du gradient conjugué
#define RPN_ALGOPARAMS_MAXITNEWTONGC	"MaxIter_NewtonGradConj"

#endif // RUBICPARAMS_NAMES
