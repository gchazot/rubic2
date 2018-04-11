#ifndef DVECT24_H
#define DVECT24_H

/** \file dvect24.h
  * \brief Contient la définition de la classe DVect24
  */

class QString;

/// Un vecteur à 24 dimensions double précision
/** Cette classe permet de manipuler des vecteurs de dimension 24 et de les
  * utiliser dans des expressions mathématiques. Il est possible de les
  * additionner, de les soustraire, de les multiplier entre eux (produit
  * scalaire), de les multiplier ou de les diviser par un scalaire, de les
  * normer, ...
  *
  * Des fonctions permettent de calculer le carré de la norme, la norme, la
  * somme des composantes , etc.
  *
  * \sa DVect, LVect
  */
class DVect24
{
public:
	/// Constructeur par défaut
	DVect24();
	/// Constructeur de recopie
	DVect24(const DVect24& v);
	/// Constructeur d'initialisation par un pointeur
	DVect24(const DVect24* v);

	/// Destructeur
	~DVect24();

private:
	/// Composantes du vecteur
	/** Le tableau mData et les variables au, av,... sont stockées dans la
	  * même plage mémoir de sorte que hu référence la même zone de données
	  * que mData[0], de même pour hv et mData[1] ainsi que ev et mData[16].
	  */
public:
	union{
		/// Données sous forme de tableau
		double mData[24]; 
		/// Les mêmes données sous forme de structure
		struct{
            double hu, hv, hw; //  0  1  2
            double au, av, aw; //  3  4  5
            double bu, bv, bw; //  6  7  8
            double cu, cv, cw; //  9 10 11
            double du, dv, dw; // 12 13 14
            double eu, ev, ew; // 15 16 17
            double fu, fv, fw; // 18 19 20
            double gu, gv, gw; // 21 22 23
		};
	};

public:
	/**********************************************************
	****     Définition des opérateurs valables pour DVect24 ****
	**********************************************************/
	/// Addition avec un autre DVect24
	/** L'addition se fait composante à composante.
	  */
	DVect24 operator+(const DVect24& v) const;
	/// Soustraction d'un autre DVect24
	/** Chaque composante du membre de droite est soustraite à la
	  * composante correspondante du membre de gauche.
	  */
	DVect24 operator-(const DVect24& v) const;
	/// Multiplication par un autre DVect24
	/** C'est le PRODUIT SCALAIRE qui est effectué (x1*x2 + y1*y2 + z1*z2).
	  */
	double operator*(const DVect24& v) const;
	/// Addition avec un scalaire
	/** Le scalaire est ajouté à chacune des composantes du DVect24.
	  */
	DVect24 operator+(const double s) const;
	/// Soustraction avec un scalaire
	/** le scalaire est soustrait à chacune des composantes du DVect24.
	  */
	DVect24 operator-(const double s) const;
	/// Multiplication par un scalaire
	/** Chaque compsante est multipliée par le scalaire.
	  */
	DVect24 operator*(const double s) const;
	/// Division par un scalaire
	/** Chaque composante est divisée par le scalaire.
	  */
	DVect24 operator/(const double s) const;
	/// Opposé du vecteur
	/** Remplace chaque composante Xi par son opposé -Xi.
	  */
	DVect24 operator-(void) const;
	
	
	/////////////// Opérateurs avec affectation /////////////////
	/// Addition avec un autre DVect24 avec affectation
	/** Addition composante à composante.
	  */
	void operator+=(const DVect24& v);
	/// Soustraction d'un autre DVect24 avec affectation
	/** Chaque composante du membre de droite est soustraite à la
	  * composante correspondante du membre de gauche.
	  */
	void operator-=(const DVect24& v);
	/// Addition avec un scalaire avec affectation
	/** Le scalaire est ajouté à chacune des composantes du DVect24.
	  */
	void operator+=(const double s);
	/// Soustraction d'un scalaire avec affectation
	/** Le scalaire est soustrait à chacune des composantes du DVect24.
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
	/// Opérateur d'affectation d'un autre DVect24
	/** Permet de faire a = b; sans appel au constructeur.
	  */
	void operator=(const DVect24& v);
	
	/// Opérateur d'indiçage
	/** Permet d'affecter une valeur à une composante; (ex: vect[i] = 250)
	  * ou de récupérer la valeur d'une composante (ex: double Xa = vect[0]).
	  */
	double& operator[](short index);

	/////////////// Opérateurs de comparaison  ////////////////
	/// Oppérateur d'égalité.
	/** Renvoie true ssi les deux vecteurs sont strictement identiques.
	  */
	bool operator==(const DVect24& v) const;

public:
	/**********************************************************
	****     Définition des fonctions utiles pour DVect24    ****
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
	friend DVect24 operator*(const double s, const DVect24& v);

public:
	/// Conversion en chaine de caractères
	/** Renvoie une chaine de caractères représentant le vecteur sous une
	  * forme affichable à l'écran.
	  */
	QString toString(void) const;
	/// Conversion en chaine de caractères des 3 premières valeurs
	/** Renvoie une chaine de caractères représentant les 3 premières
	  * composantes du vecteur sous une forme affichable à l'écran.
	  */
	QString toString3(void) const;
};

/// Multiplication par un scalaire
/** Chaque compsante du DVect24  est multipliée par le scalaire.
  */
DVect24 operator*(const double s, const DVect24& v);

#endif // DVECT24_H
