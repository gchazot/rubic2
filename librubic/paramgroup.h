#ifndef PARAMGROUP_H
#define PARAMGROUP_H

/** \file paramgroup.h
  * \brief Contient la définition de la classe ParamGroup
  */

#include <q3dict.h>

class QString;
class ParamItem;

/// Classe de gestion d'un sous-groupe de configuration
/**  Cette classe permet de gérer un sous-groupe de configuration compsé de
  * plusieurs paramètres et leur valeur. Les paramètres sont des élements de la
  * classe ParamItem et ils sont stockés dans un dictionnaire selon leur nom.
  * La classe stocke également le nom du sous-groupe.
  *
  * La classe offre des possibilités de recherche, d'insertion,et de
  * suppression des paramètres contenus dans le groupe. Elle offre également
  * des fonctions permettant la conversion d'un groupe en une chaine de
  * caractères directement prête à être écrite dans un fichier de
  * configuration.
  *
  * Sauf cas très particulier, cette classe ne devrait pas être utilisée
  * directement. A la place, il faut utiliser la classe Params.
  */
class ParamGroup
{
public:
	/// Constructeur par défaut
	ParamGroup(void);
	/// Constructeur de recopie
	ParamGroup(const ParamGroup& group);
	/// Constructeur avec initialisation du nom
	ParamGroup(const QString& name);
	
	/// Dectructeur
	~ParamGroup(void);
	
private:
	/**********************************************************************
	****                    Représentation des données                 ****
	**********************************************************************/
	/// Nom du groupe de paramètres
	QString* mName;
	/// Type de liste pour le stockage des paramètres
	typedef Q3Dict<ParamItem> ItemList;
	/// Liste des noms + valeurs du sous-groupe
	ItemList *mItems;
	
	/// Type d'itérateur pour parcourir la liste des paramètres
	typedef Q3DictIterator<ParamItem> ItemListIterator;
	/// Un itérateur interne pour lire les valeurs séquentiellement
	ItemListIterator *mIter;
	
public:
	/**********************************************************************
	****                 Fonctions de gestion du contenu               ****
	**********************************************************************/
	/// Modification du nom du groupe
	void setName(QString& name);
	/// Récupération du nom du groupe
	/** Renvoie le nom du groupe de paramètres où la chaine vide si le nom
	  * n'a pas encore été donné.
	  */
	QString getName(void) const;
	/// Ajout d'un paramètre au groupe
	/** Ajoute un paramètre dans la liste et renvoie true si tout s'est bien
	  * passé et false si une erreur se produit. Si un paramètre du même nom
	  * existe déjà, il est remplacé.
	  */
	bool addParam(QString& nom, ParamItem* param);
	/// Suppression d'un paramètre du groupe
	/** Supprime un paramètre trouvé par son nom de la liste. Renvoie true
	  * si tout se passe bien, false sinon.
	  */
	bool removeParam(const QString& nom);
	/// Récupération d'un paramètre du groupe
	/** Récupère un pointeur vers un paramètre d'après son nom. Renvoie NULL
	  * si le nom n'est pas trouvé. Le paramètre n'est PAS enlevé de la
	  * liste.
	  */
	ParamItem* getItem(const QString& nom) const;
	/// Renvoie le nombre de paramètres stockés
	uint getSize(void) const;
	
	/**********************************************************************
	****            Fonctions liées à la mise en fichier               ****
	**********************************************************************/
	/// Récupération de la chaine représentant le nom du groupe
	/** Renvoie la chaine de caractères complète du nom du sous-groupe de
	  * paramètres où la chaine vide si le nom n'a pas encore été défini.
	  * La chaine a la forme "[nom]" sans retour à la ligne à la fin.
	  */
	QString getGroupString(void) const;
	/// Récupération de la chaine représentant un paramètre et sa valeur
	/** Renvoie la chaine de caractères complète du paramètre trouvé par son
	  * nom ou une chaine vide si la chaine n'a pas été trouvée.
	  * La chaine a la forme "nom = valeur" sans retour à la ligne.
	  */
	QString getItemString(const QString& nom) const;
	/// Récupération de la chaine représentant le paramètre en cours et sa valeur
	/** Comme pour getItemString(const QString&) mais pour le paramètre
	  * en cours.
	  */
	QString getCurrentItemString(void) const;
	/// Récupération du bloc de texte représentant le groupe
	/** Renvoie le texte complet du sous-groupe sous la forme:
	  * 	[nomgroupe]
	  * 	nomparam1 = valeurparam1
	  * 	nomparam2 = valeurparam2
	  * 	....
	  * Le bloc se termine par un retour à la ligne mais ne débute PAS par
	  * un retour à la ligne.
	  */
	QString getGroupBlock(void) const;

	/**********************************************************************
	****                Fonctions de parcours du contenu               ****
	**********************************************************************/
	/// Réinitialise l'itérateur au premier élement
	void toBegin(void);
	/// Récupération de l'élement en cours
	/** Renvoie l'élement pointé actuellement où NULL si l'élement a été
	  * supprimé ou si le dernier a été dépassé. L''élement n'est PAS
	  * supprimé.
	  */
	ParamItem* getCurrentItem(void) const;
	/// Récupération du nom de l'élement en cours
	/** Renvoie le nom de l'élement pointé actuellement où une chaine vide
	  * si l'élement a été supprimé ou si le dernier a été dépassé.
	  * L'élement n'est PAS supprimé.
	  */
	QString getCurrentName(void) const;
	/// Passe à l'élement suivant.
	/** Renvoie true en cas de réussite ou false si il n'y a pas d'élement
	  * suivant (élement courant supprimé ou fin de la liste atteinte).
	  */
	bool toNext(void);
};

#endif // PARAMGROUP_H
