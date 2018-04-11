#ifndef DVECT_H
#define DVECT_H

/** \file dvect.h
  * \brief Contient la définition de la classe DVect
  */

class LVect;
class DMatrix;

class QString;

/// Un vecteur à 3 dimensions double précision
/** Cette classe permet de manipuler des vecteurs de dimension 3 et de les
  * utiliser dans des expressions mathématiques. Il est possible de les
  * additionner, de les soustraire, de les multiplier entre eux (produit
  * scalaire), de les multiplier ou de les diviser par un scalaire, de les
  * normer, ...
  *
  * Des fonctions permettent de calculer le carré de la norme, la norme, la
  * somme des composantes , etc.
  *
  * \sa LVect, DMatrix
  */
class DVect
{
public:
	/// Constructeur par défaut
	DVect();
	/// Constructeur avec initialisation des composantes
	DVect(const double x, const double y, const double z);
	/// Constructeur de recopie
	DVect(const DVect& v);
	/// Constructeur d'initialisation par un pointeur
	DVect(const DVect* v);

	/// Conversion depuis le type LVect
	/** Constructeur de conversion depuis le type LVect. La conversion
	  * long -> double suit la même règle que pour les types de base, c'est
	  * à dire que la partie décimale est supprimée, seule la partie
	  * entière est conservée, sans arrondi.
	  */
	DVect(const LVect v);
	friend class LVect;

	/// Destructeur
	~DVect();

private:
	/// Composantes du vecteur
	/** Le tableau mData et les variables mX, mY, mZ sont stockées dans la
	  * même plage mémoir de sorte que mX référence la même zone de données
	  * que mData[0], de même pour mY et mData[1] ainsi que mZ et mData[2].
	  */
	union{
		/// Données sous forme de tableau
		double mData[3]; 
		/// Les mêmes données sous forme de structure
		struct{
		double mX, mY, mZ;
		};
	};

public:
	/*********************************************************************
	****         Manipulation des composantes par des fonctions       ****
	*********************************************************************/
	/// Récupération de la composante X du vecteur
	double getX(void) const;
	/// Récupération de la composante Y du vecteur
	double getY(void) const;
	/// Récupération de la composante Z du vecteur
	double getZ(void) const;
	/// Modification de la composante X du vecteur
	void setX(const double x);
	/// Modification de la composante Y du vecteur
	void setY(const double y);
	/// Modification de la composante Z du vecteur
	void setZ(const double z);

public:
	/**********************************************************
	****     Définition des opérateurs valables pour DVect ****
	**********************************************************/
	/// Addition avec un autre DVect
	/** L'addition se fait composante à composante.
	  */
	DVect operator+(const DVect& v) const;
	/// Soustraction d'un autre DVect
	/** Chaque composante du membre de droite est soustraite à la
	  * composante correspondante du membre de gauche.
	  */
	DVect operator-(const DVect& v) const;
	/// Multiplication par un autre DVect
	/** C'est le PRODUIT SCALAIRE qui est effectué (x1*x2 + y1*y2 + z1*z2).
	  */
	double operator*(const DVect& v) const;
	/// Addition avec un scalaire
	/** Le scalaire est ajouté à chacune des composantes du DVect.
	  */
	DVect operator+(const double s) const;
	/// Soustraction avec un scalaire
	/** le scalaire est soustrait à chacune des composantes du DVect.
	  */
	DVect operator-(const double s) const;
	/// Multiplication par un scalaire
	/** Chaque compsante est multipliée par le scalaire.
	  */
	DVect operator*(const double s) const;
	/// Division par un scalaire
	/** Chaque composante est divisée par le scalaire.
	  */
	DVect operator/(const double s) const;
	/// Opposé du vecteur
	/** Remplace chaque composante Xi par son opposé -Xi.
	  */
	DVect operator-(void) const;
	
	
	/////////////// Opérateurs avec affectation /////////////////
	/// Addition avec un autre DVect avec affectation
	/** Addition composante à composante.
	  */
	void operator+=(const DVect& v);
	/// Soustraction d'un autre DVect avec affectation
	/** Chaque composante du membre de droite est soustraite à la
	  * composante correspondante du membre de gauche.
	  */
	void operator-=(const DVect& v);
	/// Addition avec un scalaire avec affectation
	/** Le scalaire est ajouté à chacune des composantes du DVect.
	  */
	void operator+=(const double s);
	/// Soustraction d'un scalaire avec affectation
	/** Le scalaire est soustrait à chacune des composantes du DVect.
	  */
	void operator-=(const double s);
	/// Multiplication par un scalaire avec affectation
	/** Chaque compsante est multipliée par le scalaire.
	  */
	void operator*=(const double s);
	/// Division par un scalaire avec affectation
	/** Chaque composante est divisée par le scalaire.
	  */
	void operator/=(const double s);
	/// Opérateur d'affectation d'un autre DVect
	/** Permet de faire a = b; sans appel au constructeur.
	  */
	void operator=(const DVect& v);
	/// Opérateur d'affectation d'un tableau de 3 double
	/** Permet de faire a = (DVect){x, y, z).
	  */
	void operator=(const double tab[3]);

	/// Opérateur d'indiçage
	/** Permet d'affecter une valeur à une composante; (ex: vect[i] = 250)
	  * ou de récupérer la valeur d'une composante (ex: double Xa = vect[0]).
	  */
	double& operator[](short index);

	///Produit d'un vecteur par une matrice
	/** Opérateur qui renvoie le vecteur résultant du produit d'un
	  * vecteur par une matrice.
	  */
	DVect operator*(DMatrix& mat) const;

	/////////////// Opérateurs de comparaison  ////////////////
	/// Oppérateur d'égalité.
	/** Renvoie true ssi les deux vecteurs sont strictement identiques.
	  */
	bool operator==(const DVect& v) const;

public:
	/**********************************************************
	****     Définition des fonctions utiles pour DVect    ****
	**********************************************************/
	/// Calcul de la norme du vecteur
	/** Formule: sqrt(x² + y² + z²)
	  */
	double getNorme(void) const;
	/// Calcul du CARRE de la norme du vecteur
	/** Formule: x² + y² + z²
	  */
	double getNorme2(void) const;
	/// Plus grande composante
	double getMax(void) const;
	/// Plus petite composante
	double getMin(void) const;
	/// Somme des composantes
	double getSomme(void) const;
	/// Somme des valeurs absolues des composantes
	double getSommeAbs(void) const;
	/// Produit des composantes
	double getProduit(void) const;
	/// Produit des valeurs absolues des composantes
	double getProduitAbs(void) const;

	/// Normer le vecteur
	/** Norme le vecteur à la valeur donnée en parametre (1.0 par défaut)
	  */
	void normer(double r = 1.0);
	
	// Opérateur de multiplication d'un scalaire par un vecteur
	friend DVect operator*(const double s, const DVect& v);

public:
	/// Conversion en chaine de caractères
	/** Renvoie une chaine de caractères représentant le vecteur sous une
	  * forme affichable à l'écran.
	  */
	QString toString(void) const;
};

/// Multiplication par un scalaire
/** Chaque compsante du DVect  est multipliée par le scalaire.
  */
DVect operator*(const double s, const DVect& v);

/// Macro permettant de convertir simplement un DVect en 3 double (pour OpenGL)
#define DVECT23F(v) v.getX(),v.getY(),v.getZ()

#endif // DVECT_H
