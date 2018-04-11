#ifndef RUBICPARAMS_H
#define RUBICPARAMS_H

/** \file rubicparams.h
  * \brief Définition de la classe RubicParams.
  */


#include "lvect.h"

#include <qstring.h>


/// Classe de gestion des paramètres spécifiques au projet rubic.
/** Les objets de cette classe sont créés par le programme rubic et
  * enregistrés dans un fichier de commande. Ce fichier est lu par le programme
  * rubicalc pour reconstruire l'objet RubicParams afin qu'il soit utilisé pour
  * récupérer les paramètres de calcul.
  *
  * \sa Params, ParamGroup, ParamItem
  */
class RubicParams
{
public:
	/**********************************************************************
	****                 Construction et Destruction                   ****
	**********************************************************************/
	/// Constructeur par défaut
	RubicParams(void);

	/// Destructeur
	~RubicParams();

private:
	/**********************************************************************
	****               Stockage des variables internes                 ****
	**********************************************************************/
	/*         Fichiers d'entrée-sortie            */
	/// Nom du fichier de commande
	QString mCmdFileName;
	/// Nom du fichier vgi de l'image de référence
	QString mVgiRefFileName;
	/// Nom du fichier vgi de l'image déformée
	QString mVgiDefFileName;
	
	/// Fichier de sortie des résultats (ASCII)
	QString mAsciiOutputFileName;
	/// Fichier de sortie des résultats (INP Abaqus)
	QString mInpOutputFileName;
	/// Fichier de log du programme
	QString mLogOutputFileName;
	
	/*           Paramètres de calcul              */
	/// Taille d'un voxern
	long mVoxernSize;
	
	/// Coordonnées mini de la zone dans laquelle effectuer le calcul
	LVect mMinCoordCalc;
	/// Coordonnées maxi de la zone dans laquelle effectuer le calcul
	LVect mMaxCoordCalc;
	
	/// Point de départ du calcul dans l'image de référence
	LVect mStartPointRef;
	/// Point de départ du calcul dans l'image déformée
	LVect mStartPointDef;
	
	/// Niveau de gris minimal à prendre en compte dans les images
	unsigned char mNGMini;
	/// Niveau de gris maximal à prendre en compte dans les images
	unsigned char mNGMaxi;
	
	/// Nombre maximal de threads executables en même temps
	int mMaxThreads;
	
	/// Utilisation de l'interpolation par splines cubiques?
	bool mInterpolCubiq;
	
	/*     Paramètres des algorithmes de calcul    */
	/// Taille de la zone de recherche en scanning pour le premier voxern
	double mTailleZoneScanning;
	
	/// Pas de calcul du scanning initial
	double mPasScanning;
	/// Pas de calcul dans le Newton interne au gradient conjugué
	double mPasNewtonGradConj;
	
	/// Précision désirée pour la convergence du scanning
	double mPrecScanning;
	/// Précision désirée pour la convergence du gradient conjugué
	double mPrecGradConj;
	/// Précision désirée pour la convergence du Newton dans le gradient conjugué.
	double mPrecNewtonGradConj;
	
	/// Nombre maximum d'itérations pour la convergence du gradient conjugué
	long mMaxitGradConj;
	/// Nombre maximum d'itérations pour la convergence du Newton du gradient conjugué
	long mMaxitNewtonGradConj;
	
public:
	/**********************************************************************
	****             Fonctions d'accès aux paramètres                  ****
	**********************************************************************/
	/*         Fichiers d'entrée-sortie           */
	/// Récupère le nom du fichier de commande
	QString getCmdFileName(void) const;
	/// Modifie le nom du fichier de commande
	void setCmdFileName(const QString& newName);
	
	/// Récupère le nom du fichier vgi de l'image de référence
	QString getVgiRefFileName(void) const;
	/// Modifie le nom du fichier vgi de l'image de référence
	void setVgiRefFileName(QString& newName);
	
	/// Récupère le nom du fichier vgi de l'image déformée
	QString getVgiDefFileName(void) const;
	/// Modifie le nom du fichier vgi de l'image déformée
	void setVgiDefFileName(QString& newName);
	
	/// Récupère le nom du fichier de sortie des résultats (ASCII)
	QString getAsciiOutputFileName(void) const;
	/// Modifie le nom du fichier de sortie des résultats (ASCII)
	void setAsciiOutputFileName(QString& newName);
	
	/// Récupère le nom du fichier de sortie des résultats (INP Abaqus)
	QString getInpOutputFileName(void) const;
	/// Modifie le nom du fichier de sortie des résultats (INP Abaqus)
	void setInpOutputFileName(QString& newName);
	
	/// Récupère le nom du fichier de log du programme
	QString getLogFileName(void) const;
	/// Modifie le nom du fichier de log du programme
	void setLogFilName(QString& newName);
	
	/*           Paramètres de calcul              */
	/// Récupère la taille d'un voxern
	long getVoxernSize(void) const;
	/// Modifie la taille du voxern
	void setVoxernSize(long newSize);
	
	/// Récupère les coordonnées MINI de la zone dans laquelle effectuer le calcul
	LVect getMinCoord(void) const;
	/// Récupère les coordonnées MAXI de la zone dans laquelle effectuer le calcul
	LVect getMaxCoord(void) const;
	
	/// Modifie les coordonnées MINI de la zone dans laquelle effectuer le cacul
	void setMinCoord(LVect& newCoord);
	/// Modifie les coordonnées MAXI de la zone dans laquelle effectuer le cacul
	void setMaxCoord(LVect& newCoord);
	
	/// Récupère les coordonnées du point de départ du calcul dans l'image de référence
	LVect getStartPointRef(void) const;
	/// Modifie les coordonnées du point de départ du calcul dans l'image de référence
	void setStartPointRef(LVect newCoord);
	
	/// Récupère les coordonnées du point de départ du calcul dans l'image déformée
	LVect getStartPointDef(void) const;
	/// Modifie les coordonnées du point de départ du calcul dans l'image déformée
	void setStartPointDef(LVect newCoord);
	
	/// Récupère le niveau de gris minimal à prendre en compte dans les calculs
	unsigned char getNGMini(void) const;
	/// Modifie le niveau de gris minimal à prendre en compte dans les calculs
	void setNGMini(unsigned char newMin);
	
	/// Récupère le niveau de gris maximal à prendre en compte dans les calculs
	unsigned char getNGMaxi(void) const;
	/// Modifie le niveau de gris maximal à prendre en compte dans les calculs
	void setNGMaxi(unsigned char newMax);
	
	/// Récupère le nombre maximal de threads executables en même temps
	int getMaxThreads(void) const;
	/// Modifie le nombre maximal de threads executables en même temps
	void setMaxThreads(int newMax);
	
	/// Récupère : Utilisation de l'interpolation par splines cubiques?
	bool getInterpolCubiq(void) const;
	/// Modifie : Utilisation de l'interpolation par splines cubiques?
	void setInterpoleCubiq(bool newVal);
	
	/*     Paramètres des algorithmes de calcul    */
	/// Récupère la taille de la zone de recherche en scanning pour le premier voxern
	double getTailleZoneScanning(void) const;
	/// Modifie la taille de la zone de recherche en scanning pour le premier voxern
	void setTailleZoneScanning(double newSize);
	
	/// Récupère le pas de calcul du scanning initial
	double getPasScanning(void) const;
	/// Modifie le pas de calcul du scanning initial
	void setPasScanning(double newVal);

	/// Récupère le pas de calcul du Newton dans le gradient conjuqué
	double getPasNewtonGradConj(void) const;
	/// Modifie le pas de calcul du Newton dans le gradient conjuqué
	void setPasNewtonGradConj(double newVal);
	
	/// Récupère la précision désirée pour la convergence du scanning initial
	double getPrecScanning(void) const;
	/// Modifie la précision désirée pour la convergence du scanning initial
	void setPrecScanning(double newVal);
	
	/// Récupère la précision désirée pour la convergence du gradient conjugué
	double getPrecGradConj(void) const;
	/// Modifie la précision désirée pour la convergence du gradient conjugué
	void setPrecGradConj(double newVal);
	
	/// Récupère la précision désirée pour la convergence du Newton dans le gradient conjugué
	double getPrecNewtonGradConj(void) const;
	/// Modifie la précision désirée pour la convergence du Newton dans le gradient conjugué
	void setPrecNewtonGradConj(double newVal);
	
	/// Récupère le nombre maximum d'itérations pour la convergence du gradient conjugué
	long getMaxitGradConj(void) const;
	/// Récupère le nombre maximum d'itérations pour la convergence du gradient conjugué
	void setMaxitGradConj(const long newVal );
	
	/// Récupère le nombre maximum d'itérations pour la convergence du Newton du gradient conjugué
	long getMaxitNewtonGradConj(void) const;
	/// Récupère le nombre maximum d'itérations pour la convergence du Newton du gradient conjugué
	void setMaxitNewtonGradConj(const long newVal );
	
public:
	/**********************************************************************
	****  Fonctions de lecture/écriture des paramètres dans un fichier ****
	**********************************************************************/
	/// Lecture d'un fichier de commande
	/** Lit les paramètres depuis le fichier de commande et renvoie true si
	  * l'opération réussit. Renvoie false si une erreur se produit durant
	  * la lecture des paramètres.
	  */
	bool readCommandFile(void);
	/// Ecriture d'un fichier de commande
	/** Ecrit les paramètres dans le fichier de commande et renvoie true si
	  * l'opération réussit. Renvoie false si une erreur se produit durant
	  * l'écriture des paramètres.
	  */
	bool writeCommandFile(void) const;
};

#endif //RUBICPARAMS_H
