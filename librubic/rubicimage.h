#ifndef RUBICIMAGE_H
#define RUBICIMAGE_H

/** \file rubicimage.h
  * \brief Contient La définition de la classe RubicImage.
  */

class LVect;
class DVect;
class DVect24;

/// Classe de gestion des images 3D de rubic
/** Cette classe permet de gérer simplement les opérations impliquant des
  * images tomographiques dans le projet Rubic. Elle englobe dans un seul objet
  * le contenu de l'image et ses dimensions.
  *
  * Cette classe permet également d'obtenir une valeur interpolée dans l'image.
 */
class RubicImage
{
public:
	/// Constructeur avec création de l'image selon les dimensions données
	RubicImage(long xSize, long ySize, long zSize);
	/// Constructeur avec création de l'image selon les dimensions en LVect
	RubicImage(LVect& size);
	/// Destructeur
	~RubicImage();

private:
	/// Stockage des données interne, pointeur vers les données
	unsigned char ***mData;
	/// Dimensions de l'image
	long mXSize, mYSize, mZSize;
	/// Booléen pour savoir si la mémoire est bien allouée
	bool mValid;
	/// Niveau de gris minimal à prendre en compte pour l'interpolation
	unsigned char mNGMin;
	/// Niveau de gris maximal à prendre en compte pour l'interpolation
	unsigned char mNGMax;

public:
	/// Renvoie true si la mémoire a été allouée pour l'image
	bool isValid();
	
	/// Récupération de la dimension X de l'image
	long getXSize() const;
	/// Récupération de la dimension Y de l'image
	long getYSize() const;
	/// Récupération de la dimension Z de l'image
	long getZSize() const;
	
	/// Récupération des dimensions de l'image
	LVect getSize(void) const;
	
	/// Modification du niveau de gris minimal à prendre en compte pour l'interpolation
	void setNGMin(const unsigned char val);
	/// Modification du niveau de gris maximal à prendre en compte pour l'interpolation
	void setNGMax(const unsigned char val);
	
	/// Récupération d'une donnée dans l'image
	unsigned char getData(long xpos, long ypos, long zpos) const;
	/// Récupération d'une donnée dans l'image
	unsigned char getData(LVect& pos) const;
	
	/// Modification d'une donnée dans l'image
	void setData(unsigned char data, long xpos, long ypos, long zpos);
	/// Modification d'une donnée dans l'image
	void setData(unsigned char data, LVect& pos);
	
	/// Interpolation trilinéaire de valeur dans l'image
	/** Calcule la valeur interpolée au point donné en paramètre.
	  * La valeur est arrondie à l'entier le plus proche.
	  * L'interpolation effectuée est TRILINEAIRE.
	  */
	double interpoleLineaire(double xpos, double ypos, double zpos);
	/// Interpolation trilinéaire de valeur dans l'image
	/** Calcule la valeur interpolée au point donné en paramètre.
	  * La valeur est arrondie à l'entier le plus proche.
	  * L'interpolation effectuée est TRILINEAIRE.
	  */
	double interpoleLineaire(DVect& pos);
	
	/// Interpolation par splines cubiques 3D dans l'image
	/** Calcule la valeur interpolée au point donné en paramètre.
	  * La valeur est arrondie à l'entier le plus proche.
	  * L'interpolation est effectuée en utilisant des splines cubiques.
	  */
	double interpoleCubique(double xpos, double ypos, double zpos);
	 
	/// Interpolation par splines cubiques 3D dans l'image
	/** Calcule la valeur interpolée au point donné en paramètre.
	  * La valeur est arrondie à l'entier le plus proche.
	  * L'interpolation est effectuée en utilisant des splines cubiques.
	  */
	double interpoleCubique(DVect& pos);
	
	/// Calcul des dérivées du niveau de gris interpolé par splines cubiques
	/** Calcul des dérivées partielles par rapport aux 24 paramètres du champ
	  * de déformation linéaire (au, bu, ..., hw). Les dérivées sont stockées
	  * dans le DVect24 renvoyé.
	  */
	bool deriveCubique(double xpos, double ypos, double zpos, double Ip, double Jp, double Kp, double Lcur, DVect24* result);
	
	/// Calcul des dérivées du niveau de gris interpolé par splines cubiques
	/** Calcul des dérivées partielles par rapport aux 24 paramètres du champ
	  * de déformation linéaire (au, bu, ..., hw). Les dérivées sont stockées
	  * dans le DVect24 renvoyé.
	  */
	bool deriveCubique(DVect& pos, double Ip, double Jp, double Kp, double Lcur, DVect24* result);
	
	/// Opérateur d'accès en tableau
	unsigned char** operator[](long index);
	
	/// Opérateur d'accès en tableau par LVect
	unsigned char operator[](LVect& index);
};

/// Fonction d'arrondi d'un double dans la plage [0, 255]
/** Arrondi le paramètre double à l'entier le plus proche
  */
unsigned char arrondi(const double& valeur);

#endif //RUBICIMAGE_H
