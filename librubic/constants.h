#ifndef CONSTANTS_H
#define CONSTANTS_H

/** \file constants.h
  * \brief Définition des constantes d'ordre général
  *
  * Ce fichier contient la définition de toutes les constantes dont la portée
  * est globale pour le projet.
  */


#ifdef WIN32
#define RET_LIGNE "\n\r"
#else
// #ifdef unix
#define RET_LIGNE "\n"
// #endif
#endif
/** \def RET_LIGNE
  * \brief Chaine de caractère utilisée pour un retour à la ligne
  *
  * Cette constante permet de gérer les retours chariot sur différents systèmes
  * d'exploitation.
  * 
  * "\n\r" sous windows.
  *
  * "\n" sous les autres OS.
  */

/// Longueur maxi d'une ligne du fichier de configuration (en caractères)
#define LONG_MAX_LIGNE_CONF 511

/// Précision des valeurs float dans le fichier de configuration (décimales)
#define PREC_FLOAT_CONF 6
/// Précision des valeurs double dans le fichier de configuration (décimales)
#define PREC_DOUBLE_CONF 6

/// Précision des valeurs double ou float à l'affichage
#define PREC_FLOAT_AFF 6
/// Largeur des valeurs float ou doule à l'affichage
#define LARG_FLOAT_AFF 12


/// Valeurs maximales des différents types
#define MAXVAL(x) (x) ~( 1 << (( 8*sizeof(x) ) - 1) )

#endif
