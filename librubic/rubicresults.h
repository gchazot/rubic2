#ifndef RUBICRESULTS_H
#define RUBICRESULTS_H
/** \file rubicresults.h
  * \brief Contient la définition de la classe RubicResults
  */

#include "lvect.h"

class RubicResults_Point;
class QString;

/// Classe de gestion des résultats de calcul de rubic
/** Cette classe permet de gérer les résultats de calcul obtenus dans rubicalc.
  * Elle gère le stockage des données durant l'execution et permet d'y accéder
  * en lecture comme en écriture avec une syntaxe identique à celle utilisée
  * pour les tableaux à trois dimensions.
  *
  * La classe gère également le stockage des résultats dans un fichier au
  * format ASCII lisible, ou dans un fichier de commande .INP pour le
  * post-processing avec Abaqus.
  */
class RubicResults
{
public:
	/// Constructeur avec dimensions séparées
	RubicResults(const long xSize, const long ySize, const long zSize);
	/// Constructeur avec dimensions en LVect
	RubicResults(const LVect& size);
	
	/// Destructeur
	~RubicResults();
	
private:
	/// Stockage du tableau de résultats
	RubicResults_Point ***mData;
	/// Stockage des dimensions de l'image
	LVect mSize;
	/// Détermine si l'objet est utilisable ou non
	bool mIsValid;
	
public:
	/// Opérateur d'accès aux données avec 3 entiers
	RubicResults_Point** operator[](const long index);
	/// Opérateur d'accès aux données avec un LVect
	RubicResults_Point* operator[](const LVect& index);
	
	/// Pour récupérer la taille de l'image
	LVect getSize(void) const;
	
	/// Renvoie true ssi l'objet est utilisable
	bool isValid(void) const;
	
	/// Ecrit les données dans un fichier ASCII
	/** Cette fonction écrit l'ensemble des résultats dans un fichier texte
	  * très lisible avec un en-tête décrivant le format du fichier.
	  *
	  * La fonction renvoie true si l'écriture réussit et false sinon.
	  */
	bool writeToASCII(const QString& fileName) const;
	
	/// Ecrit les données dans un fichier INP pour Abaqus
	/** Cette fonction écrit l'ensemble des résultats dans un fichier texte
	  * utilisable comme fichier d'entrée Abaqus pour effectuer du
	  * post-traitement. Le fichier est commenté afin de pouvoir le
	  * modifier aisément.
	  *
	  * La fonction renvoie true si l'écriture réussit et false sinon.
	  */
	bool writeToINP(const QString& fileName, const long Lcur) const;
};

#endif // RUBICRESULTS_H
