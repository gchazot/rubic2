#ifndef RUBICPARAMS_DEFAULTS_H
#define RUBICPARAMS_DEFAULTS_H
/** \file rubicparams_defaults.h
  * \brief Valeurs par défaut des membres de RubicParams.
  *
  * Ce fichier définit les valeurs par défaut des paramètres de RubicParams
  * utilisés lorsque l'une des valeurs n'est pas spécifiée dans le fichier de
  * configuration.
  */

/******************************************************************************
****                       Paramètres de calcul                            ****
******************************************************************************/
/// Taille du voxern
#define RPD_VOXERNSIZE 10

/// Point de départ du calcul dans l'image de référence: coordonnée X
#define RPD_STARTPOINT_REF_X	0
/// Point de départ du calcul dans l'image de référence: coordonnée Y
#define RPD_STARTPOINT_REF_Y	0
/// Point de départ du calcul dans l'image de référence: coordonnée Z
#define RPD_STARTPOINT_REF_Z	0
/// Point de départ du calcul dans l'image de référence: DVect
#define RPD_STARTPOINT_REF	DVect(RPD_STARTPOINT_REF_X, RPD_STARTPOINT_REF_Y, RPD_STARTPOINT_REF_Z)

/// Point de départ du calcul dans l'image déformée: coordonnée X
#define RPD_STARTPOINT_DEF_X	0
/// Point de départ du calcul dans l'image déformée: coordonnée Y
#define RPD_STARTPOINT_DEF_Y	0
/// Point de départ du calcul dans l'image déformée: coordonnée Z
#define RPD_STARTPOINT_DEF_Z	0
/// Point de départ du calcul dans l'image déformée: DVect
#define RPD_STARTPOINT_DEF	DVect(RPD_STARTPOINT_DEF_X, RPD_STARTPOINT_DEF_Y, RPD_STARTPOINT_DEF_Z)

/// Niveau de gris mini à prendre en compte dans les calculs
#define RPD_NGMINI		1

/// Niveau de gris maxi à prendre en compte dans les calculs
#define RPD_NGMAXI		254

/// Nombre maximal de threads executables en même temps
#define RPD_MAXTHREADS		4

/// Utilisation de l'interpolation par splines cubiques?
#define RPD_INTERPOLCUBIQ	true

/******************************************************************************
****                 Paramètres des algorithmes de calcul                  ****
******************************************************************************/
/// Taille de la zone pour le scanning initial
#define RPD_TAILLEZONNESCANNING	2.0

/// Pas du scanning initial
#define RPD_PASSCANNING		1.0
/// Pas de calcul dans le Newton du gradient conjugué
#define RPD_PASNEWTONGRADCONJ	0.1f

/// Précision désirée pour la convergence du scanning
#define RPD_PRECSCANNING	0.01f
/// Précision désirée pour la convergence du gradient conjugué
#define RPD_PRECGRADCONJ	0.01f
/// Précision désirée pour la convergence du Newton dans le gradient conjugué
#define RPD_PRECNEWTONGRADCONJ	0.001f

/// Nombre maximum d'itérations pour la convergence du gradient conjugué
#define RPD_MAXITGRADCONJ	100
/// Nombre maximum d'itérations pour la convergence du Newton du gradient conjugué
#define RPD_MAXITNEWTONGC	100

#endif //RUBICPARAMS_DEFAULTS_H
