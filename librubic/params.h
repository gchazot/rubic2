#ifndef PARAMS_H
#define PARAMS_H

/** \file params.h
  * \brief Contient la définition de la classe Params
  */

#include <q3dict.h>

class QString;
class ParamItem;
class ParamGroup;
class DVect;
class LVect;

/// Classe générique de gestion d'un ensemble de paramètres
/** Cette classe permet la gestion d'un ensemble complet de paramètres ou
  * d'options pouvant être stockés dans un fichier. Les paramètres peuvent
  * être arrangés en sous-groupes et être de types différents.
  *
  * Cette classe offre les fonctione permettant de lire ou de créer de tels
  * fichiers, ainsi que les fonctions permettant d'ajouter et de récupérer les
  * valeurs des paramètres. Chaque paramètre est caractérisépar le nom du
  * groupe auquel il appartient, son nom et sa valeur.
  */
class Params
{
public:
	/// Constructeur par défaut
	Params(void);
	/// Constructeur de recopie
	Params(Params& param);
	
	/// Destructeur
	~Params(void);

private:
	/*********************************************************************
	****            Variables de stockage des paramètres              ****
	*********************************************************************/
	/// Type de liste pour le stockage des sous-groupes de paramètres.
	typedef Q3Dict<ParamGroup> GroupList;
	/// Stockage des sous-groupes de paramètres dans une liste.
	GroupList* mGroups;
	
	/// Type d'itérateur pour la liste de sous-groupes
	typedef Q3DictIterator<ParamGroup> GroupListIterator;
	/// Un itérateur interne pour lire les groupes séquentiellement
	GroupListIterator *mIter;

public:
	/*********************************************************************
	****               Fonctions d'accès aux paramètres               ****
	*********************************************************************/
	/// Ajouter une valeur short
	/** Ajouter un paramètre avec son groupe, son nom et sa valeur.
	  * Si le nom donné existe dans le groupe donné, l'ancienne valeur du
	  * paramètre sera supprimée. Si le groupe n'existe pas, il sera créé.
	  * De même si le nom n'existe pas, il sera créé.
	  */
	void insertShort(QString groupe, QString nom, short value);
	/// Ajouter une valeur int
	/** Voir insertShort().
	  */
	void insertInt(QString groupe, QString nom, int value);
	/// Ajouter une valeur long
	/** Voir insertShort().
	  */
	void insertLong(QString groupe, QString nom, long value);
	/// Ajouter une valeur float
	/** Voir insertShort().
	  */
	void insertFloat(QString groupe, QString nom, float value);
	/// Ajouter une valeur double
	/** Voir insertShort().
	  */
	void insertDouble(QString groupe, QString nom, double value);
	/// Ajouter une valeur bool
	/** Voir insertShort().
	  */
	void insertBool(QString groupe, QString nom, bool value);
	/// Ajouter une valeur string
	/** Voir insertShort().
	  */
	void insertString(QString groupe, QString nom, QString value);
	/// Ajouter une valeur DVect
	/** Voir insertShort().
	  */
	void insertDVect(QString groupe, QString nom, DVect value);
	/// Ajouter une valeur LVect
	/** Voir insertShort().
	  */
	void insertLVect(QString groupe, QString nom, LVect value);
	
	/// Récupérer une valeur short
	/** Récupérer un paramètre d'après son nom et son groupe.
	  * Si le nom ou le groupe donné n'existe pas ou si le paramètre ne
	  * contient pas de valeur, la fonction renvoie false. Si la valeur
	  * placée dans le paramètre est valide, la fonction renvoie true.
	  */
	bool getShort(QString groupe, QString nom, short* value) const;
	/// Récupérer une valeur int
	/** Voir getShort()
	  */
	bool getInt(QString groupe, QString nom, int* value) const;
	/// Récupérer une valeur long
	/** Voir getShort()
	  */
	bool getLong(QString groupe, QString nom, long* value) const;
	/// Récupérer une valeur float
	/** Voir getShort()
	  */
	bool getFloat(QString groupe, QString nom, float* value) const;
	/// Récupérer une valeur double
	/** Voir getShort()
	  */
	bool getDouble(QString groupe, QString nom, double* value) const;
	/// Récupérer une valeur bool
	/** Voir getShort()
	  */
	bool getBool(QString groupe, QString nom, bool* value) const;
	/// Récupérer une valeur string
	/** Voir getShort()
	  */
	bool getString(QString groupe, QString nom, QString* value) const;
	/// Récupérer une valeur DVect
	/** Voir getShort()
	  */
	bool getDVect(QString groupe, QString nom, DVect* value) const;
	/// Récupérer une valeur DVect
	/** Voir getShort()
	  */
	bool getLVect(QString groupe, QString nom, LVect* value) const;
	
	
	/// Ajout d'un paramètre complet
	/** Ajouter  un paramètre avec son groupe, son nom et un pointeur.
	  * Si le nom donné existe dans le groupe donné, l'ancienne valeur du
	  * paramètre sera supprimée. Si le groupe n'existe pas, il sera créé.
	  * De même si le nom n'existe pas, il sera créé.
	  */
	void insertParam(QString groupe, QString nom, ParamItem* param);
	/// Récupérer un paramètre complet
	/** Récupérer un paramètre d'après son nom et son groupe.
	  * Si le nom ou le groupe donné n'existe pas ou si le paramètre ne
	  * contient pas de valeur, la fonction renvoie false. Si la valeur
	  * placée dans le paramètre est valide, la fonction renvoie true.
	  */
	bool getParam(QString groupe, QString nom, ParamItem* param) const;
	
	/*********************************************************************
	****             Fonctions liées à la mise en fichier             ****
	*********************************************************************/
	/// Ecrire la configuration dans un fichier
	/** Ecrit l'ensemble de la configuration dans le fichier spécifié en
	  * paramètre. Si le paramètre overwrite est mis à true et que le
	  * fichier spécifié existe déjà, alors il sera remplacé. Si la fonction
	  * échoue, la valeur renvoyée est false, sinon, c'est true qui sera
	  * renvoyée. Les causes d'echec sont multiples: le répertoire contenant
	  * le fichier n'existe pas, le fichier spécifié existe et n'est pas
	  * éditable ou overwrite vaut false, il est impossible de créer le
	  * fichier spécifié, etc.
	  */
	bool writeToFile(QString filename, bool overwrite = false) const;
	
	/// Lire la configuration dans un fichier
	/** Lit l'ensemble de la configuration depuis le fichier spécifié.
	  * La fonction renvoie true si tout se passe bien et false si une
	  * erreur se produit. Les causes d'erreur sont multiples: fichier
	  * spécifié introuvabe ou non-lisible, erreur de syntaxe dans le
	  * fichier, ...
	  */
	bool readFromFile(QString filename);
	
private:
	/**********************************************************************
	****        Fonctions de gestion des groupes de paramètres         ****
	**********************************************************************/
	/// Vérifier si un groupe existe
	/** Renvoie true si le groupe dont le nom est passé en paramètre existe
	  * déjà, sinon renvoie false.
	  */
	bool groupExists(const QString groupe) const;
	/// Créer un groupe
	/** Crée le groupe dont le nom est donné en paramètre si il n'existe pas
	  * encore.
	  */
	void createGroup(const QString groupe);
};

#endif // PARAMS_H
