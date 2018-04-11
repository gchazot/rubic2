#ifndef PARAMITEM_H
#define PARAMITEM_H

/** \file paramitem.h
  * \brief Contient la définition de la classe ParamItem.
  */

#include "dvect.h"
#include "lvect.h"

class QString;

/// Classe de gestion des valeurs des paramètres
/** Cette classe permet de stocker les valeurs des paramètres quel que soit
  * le type de données correspondant.
  *
  * La classe offre également des fonctions pour la mise en fichier des données
  * et pour la lecture des valeurs à partir de chaines de caractères.
  *
  * Sauf cas très particulier, cette classe ne devrait pas être utilisée
  * directement. A la place, il faut utiliser la classe Params.
  *
  * \sa ParamGroup, Params
  */
class ParamItem
{
public:
	/// Constructeur par défaut
	ParamItem(void);
	/// Constructeur avec une valeur short
	ParamItem(const short val);
	/// Constructeur avec une valeur int
	ParamItem(const int val);
	/// Constructeur avec une valeur long
	ParamItem(const long val);
	/// Constructeur avec une valeur float
	ParamItem(const float val);
	/// Constructeur avec une valeur double
	ParamItem(const double val);
	/// Constructeur avec une valeur bool
	ParamItem(const bool val);
	/// Constructeur avec une valeur QString
	ParamItem(const QString& val);
	/// Constructeur avec une valeur chaine de caractères C
	ParamItem(const char* val);
	/// Constructeur avec une valeur DVect
	ParamItem(const DVect& val);
	/// Constructeur avec une valeur LVect
	ParamItem(const LVect& val);
	
	/// Destructeur
	~ParamItem();
	
private:
	/**********************************************************************
	****                  Structure de données interne                 ****
	**********************************************************************/
	/// Stocke un entier représentant le type de données stocké
	short mType;
	/// Stockage des données
	union{
		long mDataLong;
		double mDataDouble;
		bool mDataBool;
		QString* mDataString;
		DVect* mDataDVect;
		LVect* mDataLVect;
	};
	
public:
	/// Constantes décrivant les Types de données stockables
	enum mDataType{
		UndefinedParam	= 0,
		ShortParam	= 10,
		IntParam	,
		LongParam	,
		FloatParam	= 20,
		DoubleParam	,
		BoolParam	= 30,
		StringParam	= 40,
		DVectParam	= 50,
		LVectParam	= 51
	};

public:
	/**********************************************************************
	****           Fonctions de getsion du type de données             ****
	**********************************************************************/
	/// Renvoie la constante définissant le type de paramètre
	const short getType(void) const;
	/// Modifie le type de données stocké
	void setType(mDataType type);
	
	/**********************************************************************
	****            Fonctions de récupération des données              ****
	**********************************************************************/
	/// Récupérer une valeur short
	/** Renvoie le contenu de mDataShort dans le paramètre.
	  * Si l'objet contient un short, renvoie true, sinon false.
	  */
	bool getShortValue(short* val) const;
	/// Récupérer une valeur int
	/** Renvoie le contenu de mDataInt dans le paramètre.
	  * Si l'objet contient un int, renvoie true, sinon false.
	  */
	bool getIntValue(int* val) const;
	/// Récupérer une valeur long
	/** Renvoie le contenu de mDataLong dans le paramètre.
	  * Si l'objet contient un long, renvoie true, sinon false.
	  */
	bool getLongValue(long* val) const;
	/// Récupérer une valeur float
	/** Renvoie le contenu de mDataFloat dans le paramètre.
	  * Si l'objet contient un float, renvoie true, sinon false.
	  */
	bool getFloatValue(float* val) const;
	/// Récupérer une valeur double
	/** Renvoie le contenu de mDataDouble dans le paramètre.
	  * Si l'objet contient un double, renvoie true, sinon false.
	  */
	bool getDoubleValue(double* val) const;
	/// Récupérer une valeur bool
	/** Renvoie le contenu de mDataBool dans le paramètre.
	  * Si l'objet contient un booleen, renvoie true, sinon false.
	  */
	bool getBoolValue(bool* val) const;
	/// Récupérer une valeur string
	/** Renvoie le contenu de mDataString dans le paramètre.
	  * Si l'objet contient une chaine, renvoie true, sinon false.
	  */
	bool getStringValue(QString* val) const;
	/// Récupérer une valeur DVect
	/** Renvoie le contenu de mDataDVect dans le paramètre.
	  * Si l'objet contient un DVect, renvoie true, sinon false.
	  */
	bool getDVectValue(DVect* vec);
	/// Récupérer une valeur LVect
	/** Renvoie le contenu de mDataDVect dans le paramètre.
	  * Si l'objet contient un LVect, renvoie true, sinon false.
	  */
	bool getLVectValue(LVect* vec);
	
	/**********************************************************************
	****            Fonctions de modification des données              ****
	**********************************************************************/
	/// Placer une valeur long
	/** Modifie le contenu si le type est compatible.
	  * Renvoie true si la modification a été effectuée, false sinon.
	  */
	bool setLongValue(long val);
	/// Placer une valeur double
	/** Modifie le contenu si le type est compatible.
	  * Renvoie true si la modification a été effectuée, false sinon.
	  */
	bool setDoubleValue(double val);
	/// Placer une valeur bool
	/** Modifie le contenu si le type est compatible.
	  * Renvoie true si la modification a été effectuée, false sinon.
	  */
	bool setBoolValue(bool val);
	/// Placer une valeur string
	/** Modifie le contenu si le type est compatible.
	  * Renvoie true si la modification a été effectuée, false sinon.
	  */
	bool setStringValue(QString* val);
	/// Placer une valeur DVect
	/** Modifie le contenu si le type est compatible.
	  * Renvoie true si la modification a été effectuée, false sinon.
	  */
	bool setDVectValue(DVect vec);
	/// Placer une valeur LVect
	/** Modifie le contenu si le type est compatible.
	  * Renvoie true si la modification a été effectuée, false sinon.
	  */
	bool setLVectValue(LVect vec);
	/**********************************************************************
	****            Fonctions liées à la mise en fichier               ****
	**********************************************************************/
	/// Renvoie une chaine de caractères contenant les données
	QString getValueString(void) const;
	/// Déterminer le type de données contenu dans une chaine de caractères.
	/** Renvoie la constante associée au type de paramètre contenu dans la
	  * chaine de caractères spécifiée. La constante UndefinedParam est
	  * renvoyée si le type n'est pas reconnu.
	  */
	static const short getStringParamType(const QString str);
	/// Créer un paramètre d'après la chaine de caractères qui le représente
	/** Crée un Paramètre d'après le contenu sous forme de chaine de
	  * caractères de sa valeur. Renvoie un pointeur vers l'élement créé ou
	  * la valeur NULL si une erreur se produit.
	  */
	static ParamItem* createParamFromString(const QString str);
};
#endif //PARAMITEM_H
