#ifndef RUBICFILES_H
#define RUBICFILES_H

/** \file rubicfiles.h
  * \brief Contient la définition de la classe RubicFiles
  */

#include <qstring.h>

class QWidget;

class LVect;
class RubicImage;

/// Classe de gestion de la paire de fichiers RAW + VGI
/** Cette classe permet de gérer avec un seul objet la paire de fichiers
  * RAW + VGI utilisée pour stocker les images tomographiques 3D dans le
  * projet rubic.
  * Cette classe permet de lire et écrire ces deux fichiers et offre des
  * fonctions de vérification de la présence de ces deux fichiers e vue d'y
  * écrire ou d'y lire des données.
  *
  * \sa RubicImage
  */
class RubicFiles
{
public:
	/// Constructeur par défaut
	RubicFiles(void);
	/// Destructeur
	~RubicFiles();
	
private:
	/// Dimensions de l'image
	long mXSize, mYSize, mZSize;
	/// Nom du fichier vgi
	QString* mVgiFileName;
	/// Nom du fichier raw
	QString* mRawFileName;
	
public:
	/// Renvoie le nom du fichier vgi ou la chaine nulle si il n'est pas défini
	QString getVgiFileName(void) const;
	/// Renvoie le nom du fichier raw ou la chaine nulle si il n'est pas défini
	QString getRawFileName(void) const;
	/// Modifie le nom du fichier vgi
	void setVgiFileName(const QString& name);
	/// Modifie le nom du fichier raw
	void setRawFileName(const QString& name);
	
	/// Crée le nom du fichier raw d'après le nom du fichier vgi
	/** Renvoie true si la création a réussi et false sinon
	  */
	bool setRawFileNameFromVgiFileName(void);
	
	
	/// Renvoie les dimensions de l'image
	LVect getSize(void) const;
	/// Renvoie la dimension X de l'image
	long getXSize(void) const;
	/// Renvoie la dimension Y de l'image
	long getYSize(void) const;
	/// Renvoie la dimension Z de l'image
	long getZSize(void) const;
	
	/// Modifie toutes les dimensions de l'image:
	void setSize(long xval, long yval,long zval);
	/// Modifie la dimension X de l'image
	void setXSize(long val);
	/// Modifie la dimension Y de l'image
	void setYSize(long val);
	/// Modifie la dimension Z de l'image
	void setZSize(long val);

	/// Message de confirmation d'écrasement de fichiers
	/** Vérifie si les fichiers correspondant à l'objet existent et affiche
	  * un avertissement avec confirmation d'écrasement si ils existent
	  * La fonction renvoie true si les fichiers n'existent pas ou si
	  * l'utilisateur a décidé de les écraser. Elle renvoie false si les
	  * fichiers existent mais que l'utilisateur ne veut pas les supprimer
	  * ou si on ne peut pas écrire dedans (droits d'accèssi une autre erreur
	  * se produit.
	  */
	bool checkFilesOverwrite(QWidget* parent) const;

	/// Vérification de lecture
	/** Vérifie si les fichiers correspondant à l'objet existent et s'ils
	  * sont lisibles. La fonction renvoie true s'ils existent et sont
	  * lisibles. Elle renvoie false sinon.
	  */
	bool checkFilesRead(void) const;
	
	/// Vérification d'écriture
	/** Vérifie si l'on peut écrire dans les fichiers correspondant à
	  * l'objet. Renvoie true si c'est possible, false sinon
	  */
	bool checkFilesWrite(void) const;
	
	/// Lit la taille de l'image depuis le fichier vgi.
	/** Renvoie true en cas de succès, false sinon.
	  */
	bool readSizeFromVgi(void);
	
	/// Ecriture du fichier vgi
	/** Ecrit un fichier vgi minimal contenant uniquement la taille de
	  * l'image. Renvoie true en cas de succès et false sinon.
	  * Si le paramètreoverwrite est à false et que le fichier vgi existe
	  * déjà, alors le fichier ne sera pas écrit et la fonction renverra
	  * false. Si la paramètre est à true, un fichier vgi existant sera
	  * remplacé par un nouveau.
	  */
	bool writeVgiFile(bool overwrite = false) const;
	
	/// Lecture du fichier .raw
	/** Lit le fichier .raw et alloue la mémoire du tableau donné en
	  * paramètre pour y placer les données.
	  * Renvoie true si l'opération réussit. Renvoie false et NULL dans
	  * le paramètre si une erreur de lecture ou d'allocation de la mémoire
	  * s'est produite.
	  */
	bool readRawFile(unsigned char ****data) const;
	
	/// Lecture du fichier .raw
	/** Lit le fichier .raw et crée l'objet RubicImage avec les données
	  * qu'il contient. Renvoie true si l'opération réussit. Renvoie false
	  * si une erreur de lecture ou d'allocation de la mémoire s'est
	  * produite.
	  */
	bool readRawFile(RubicImage **img);
	
	/// Ecriture du fichier .raw
	/** Ecrit les données passées par le paramètre data dans le fichier .raw
	  * La fonction renvoie true si tout se passe bien ou false si une erreur
	  * d'écriture se produit. La taille du tableau en paramètre doit être
	  * supérieure ou égale à celle donnée par mXSize * mYSize * mZSize.
	  */
	bool writeRawFile(const unsigned char ****data, bool overwrite = false) const;
	
	/// Ecriture du fichier .raw
	/** Ecrit les données passées par le paramètre img dans le fichier .raw
	  * La fonction renvoie true si tout se passe bien ou false si une erreur
	  * d'écriture se produit. La taille de l'image en paramètre doit être
	  * supérieure ou égale à celle donnée par mXSize * mYSize * mZSize.
	  */
	bool writeRawFile(RubicImage* img, bool overwrite = false) const;
	
	/// Désalloue la mémoire d'une image
	void delRawFile(unsigned char ****data);
};



#endif // RUBICFILES_H
