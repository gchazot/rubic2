#ifndef DMATRIX_UTILS_H
#define DMATRIX_UTILS_H

/** \file dmatrix_utils.h
  * \brief Contient les déclarations des fonctions utiles pour DMatrix.
  */

class DMatrix;
class LVect;
class DVect;

/// Algorithme de diagonalisation de JACOBI
/** Cette fonction prend en paramètre une matrice 3x3 a SYMETRIQUE.
  *
  * Elle calcule les valeurs propres de la matrice par la méthode de Jacobi et
  * les renvoie dans le tableau d. Les vecteurs propres correspondants sont
  * placés dans la matrice 3x3 v. La fonction renvoie dans nrot le nombre de
  * rotations qui ont été nécessaires pour que la diagonalisation converge.
  *
  * La valeur de retour de la fonction est true si la diagonalisation a convergé
  * ou false si le nombre d'itérations maximal a été atteint.
  *
  * FONCTION CRÉÉE D'APRES NUMERICAL RECIPIES IN C, PAGE 467
  */
bool jacobi(DMatrix* a, DVect* d, DMatrix* v, int *nrot);

/// Calcul d'un cofacteur dans une matrice
/** Calcule le cofacteur correspondant à la position donnée dans la
  * matrice donnée.
  */
double cofacteur(DMatrix* mat, short i, short j);

#endif //DMATRIX_UTILS_H
