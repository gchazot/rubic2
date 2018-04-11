#ifndef FONCTIONS_H
#define FONCTIONS_H

/** \file fonctions.h
  * \brief Contient les prototypes des fonctions utilisées pour le calcul dans rubicalc
  */

class LVect;
class DVect;
class DVect24;

/// Scanning initial
/** Effectue un scanning initial à partir des coordonnées dans globals::Iv et
  * dans globals::U24 et améliore la valeur de U24 par scanning.
  *
  * Renvoie la valeur du false si une erreur se produit et true si tout s'est
  * déroulé sans problème.
  */
bool executeScanning(void);

/// Scanning pout un point particulier de l'image
/** Effectue un scanning pour un point donné en paramètre de l'image. Les
  * paramètres sont le point de l'image initiale et le point de l'image
  * déformée. La fonction se sert des variables globales pour déterminer le pas
  * et la taille de la zone pour le scanning.
  *
  * La fonction renvoie la valeur du meilleur coefficient de corrélation
  * calculé ou la valeur -1.0 si une erreur se produit. Elle renvoie également
  * la position de la meilleur corrélation dans le troisième paramètre.
  */
double scanningPoint(const LVect& iniPnt, const DVect& defPnt, DVect* minPoint);

/// Calcule le coefficient de corrélation entre deux voxerns
/** Le calcul est effectué entre le voxern de coordonnées mini Pini dans l'image initiale
  * et le voxern déformé par def.
  *
  * La valeur renvoyée est dans [0.0;1.0] si tout se passe bien, sinon elle
  * vaut -1.0.
  */
double Correlation(const LVect& Pini, const DVect24& def);

/// Calcule les principales parties du coefficient de corrélation
/** Le calcul est effectué entre le voxern de coordonnées mini Iv dans l'image initiale et le voxern déformé par u.
  * 
  * Les valeurs renvoyées (cinq derniers paramètres) permettent le calcul du coefficient de corrélation, et de ses 24 dérivées si deriv vaut true.
  * Voir rapport PFE pour les détails des formules.
  */
int Coeff_Correlation(const LVect& Iv, DVect24& u, bool deriv, double *correl_num, double *correl_den_def, double *correl_den_ref, DVect24& derivCoeff_1, DVect24& derivCoeff_2);

/// Gère la propagation du calcul dans les voxerns 
/** Renvoie true si le calcul s'est éxecuté correctement et false si une erreur
  * critique s'est produite et a intérrompu le calcul.
  */
bool executePropagation(void);

/// Minimise la fonction de corrélation entre les images initiale et déformée par la méthode des gradients conjugués
/** La fonction recherche dans l'image déformée le voxern qui minimise la corrélation avec le voxern de coordonnées Pini dans l'image initiale
  * 
  * On initialise la fonction avec uE, champ de déformations en entrée basé sur le calcul des voxerns adjacents déjà calculés. Elle renvoie uS, champ de déformation minimisant la fonction de corrélation.
  */
double ChercheVoxern(LVect Pini, DVect24 uE, DVect24 *uS);

#endif //FONCTIONS_H
