#ifndef LVECT_H
#define LVECT_H

/** \file lvect.h
  * \brief Contient la définition de la classe LVect.
  */

class DVect;
class QString;

/// Un vecteur à 3 dimensions d'entiers long
/** Cette classe permet de manipuler des vecteurs de dimension 3 et de les
  * utiliser dans des expressions mathématiques. Il est possible de les
  * additionner, de les soustraire, de les multiplier entre eux (produit
  * scalaire), de les multiplier ou de les diviser par un scalaire, de les
  * normer, ...
  *
  * Des fonctions permettent de calculer le carré de la norme, la norme, la
  * somme des composantes , etc.
  *
  * \sa DVect, DMatrix
  */
class LVect
{
public:
	/// Constructeur par défaut
	LVect();
	/// Constructeur avec initialisation des composantes
	LVect(const long x, const long y, const long z);
	/// Constructeur de recopie
	LVect(const LVect& v);
	/// Constructeur d'initialisation par un pointeur
	LVect(const LVect* v);

	/// Conversion depuis le type DVect
	/** Constructeur de conversion depuis le type DVect. La conversion
	  * double -> long suit la même règle que pour les types de base
	  */
	LVect(const DVect v);
	
	friend class DVect;

	/// Destructeur
	~LVect();

private:
	/// Composantes du vecteur
	/** Le tableau mData et les variables mX, mY, mZ sont stockées dans la
	  * même plage mémoir de sorte que mX référence la même zone de données
	  * que mData[0], de même pour mY et mData[1] ainsi que mZ et mData[2].
	  */
	union{
		/// Le stockage sous forme de tableau
		long mData[3];
		/// Le stockage sous forme de structure de compsantes
		struct{
		long mX, mY, mZ;
		};
	};

public:
	/*********************************************************************
	****         Manipulation des composantes par des fonctions       ****
	*********************************************************************/
	/// Récupération de la composante X du vecteur
	long getX(void) const;
	/// Récupération de la composante Y du vecteur
	long getY(void) const;
	/// Récupération de la composante Z du vecteur
	long getZ(void) const;
	/// Modification de la composante X du vecteur
	void setX(const long x);
	/// Modification de la composante Y du vecteur
	void setY(const long y);
	/// Modification de la composante Z du vecteur
	void setZ(const long z);

public:
	/**********************************************************
	****     Définition des opérateurs valables pour LVect ****
	**********************************************************/
	/// Addition avec un autre LVect
	/** L'addition se fait composante à composante.
	  */
	LVect operator+(const LVect& v) const;
	/// Soustraction d'un autre LVect
	/** Chaque composante du membre de droite est soustraite à la
	  * composante correspondante du membre de gauche.
	  */
	LVect operator-(const LVect& v) const;
	/// Multiplication par un autre LVect
	/** C'est le PRODUIT SCALAIRE qui est effectué (x1*x2 + y1*y2 + z1*z2).
	  */
	long operator*(const LVect& v) const;
	/// Addition avec un scalaire
	/** Le scalaire est ajouté à chacune des composantes du LVect.
	  */
	LVect operator+(const long s) const;
	/// Soustraction avec un scalaire
	/** le scalaire est soustrait à chacune des composantes du LVect.
	  */
	LVect operator-(const long s) const;
	/// Multiplication par un scalaire
	/** Chaque compsante est multipliée par le scalaire.
	  */
	LVect operator*(const long s) const;
	/// Division par un scalaire
	/** Chaque composante est divisée par le scalaire.
	  */
	LVect operator/(const long s) const;
	/// Opposé du vecteur
	/** Remplace chaque composante Xi par son opposé -Xi.
	  */
	LVect operator-(void) const;
	/// Opérateur modulo par rapport à un entier
	/** Effectue un modulo sur chacune des composantes séparément
	  */
	LVect operator%(const long e) const;
	/// Opérateur modulo par rapport à un autre LVect
	/** Effectue un modulo entre chacune des composante du membre de gauche
	  * et la composante correcpondante du membre de droite.
	  */
	LVect operator%(const LVect& v) const;
	
	
	/////////////// Opérateurs avec affectation /////////////////
	/// Addition avec un autre LVect avec affectation
	/** Addition composante à composante.
	  */
	void operator+=(const LVect& v);
	/// Soustraction d'un autre LVect avec affectation
	/** Chaque composante du membre de droite est soustraite à la
	  * composante correspondante du membre de gauche.
	  */
	void operator-=(const LVect& v);
	/// Addition avec un scalaire avec affectation
	/** Le scalaire est ajouté à chacune des composantes du LVect.
	  */
	void operator+=(const long s);
	/// Soustraction d'un scalaire avec affectation
	/** Le scalaire est soustrait à chacune des composantes du LVect.
	  */
	void operator-=(const long s);
	/// Multiplication par un scalaire avec affectation
	/** Chaque compsante est multipliée par le scalaire.
	  */
	void operator*=(const long s);
	/// Division par un scalaire avec affectation
	/** Chaque composante est divisée par le scalaire.
	  */
	void operator/=(const long s);
	/// Opérateur d'affectation d'un autre LVect
	/** Permet de faire a = b; sans appel au constructeur.
	  */
	void operator=(const LVect& v);
	/// Opérateur d'affectation d'un tableau de 3 long
	/** Permet de faire a = (LVect){x, y, z).
	  */
	void operator=(const long tab[3]);

	/// Opérateur d'indiçage
	/** Permet d'affecter une valeur à une composante; (ex: vect[i] = 250)
	  * ou de récupérer la valeur d'une composante (ex: long Xa = vect[0]).
	  */
	long& operator[](short index);

	/////////////// Opérateurs de comparaison  ////////////////
	//// Oppérateur d'égalité.
	/** Renvoie true ssi les deux vecteurs sont strictement identiques.
	  */
	bool operator==(const LVect& v) const;

public:
	/**********************************************************
	****     Définition des fonctions utiles pour LVect    ****
	**********************************************************/
	/// Calcul de la norme du vecteur
	/** Formule: sqrt(x² + y² + z²)
	  */
	double getNorme(void) const;
	/// Calcul du CARRE de la norme du vecteur
	/** Formule: x² + y² + z²
	  */
	long getNorme2(void) const;
	/// Plus grande composante
	long getMax(void) const;
	/// Plus petite composante
	long getMin(void) const;
	/// Somme des composantes
	long getSomme(void) const;
	/// Somme des valeurs absolues des composantes
	long getSommeAbs(void) const;
	/// Produit des composantes
	long getProduit(void) const;
	/// Produit des valeurs absolues des composantes
	long getProduitAbs(void) const;
	
	// Opérateur de multiplication d'un scalaire par un vecteur
	friend LVect operator*(const long s, const LVect& v);

public:
	/// Conversion en chaine de caractères
	/** Renvoie une chaine de caractères représentant le vecteur sous une
	  * forme affichable à l'écran.
	  */
	QString toString(void) const;
};
/******************************************************************************
****              Fonctions travaillant sur le type LVect                 *****
******************************************************************************/
/// Multiplication par un scalaire
/** Chaque compsante du LVect  est multipliée par le scalaire.
  */
LVect operator*(const long s, const LVect& v);


#endif // LVECT_H
