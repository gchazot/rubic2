/** \file paramgroup.cpp
  * \brief Contient la définition des fonctions membres de la classe ParamGroup
  */

#include "paramgroup.h"
#include "paramitem.h"

#include "constants.h"

#include <q3dict.h>
#include <qstring.h>

// Constructeur par défaut
ParamGroup::ParamGroup(void)
{
	mName = NULL;
	
	mItems = new ItemList(29);
	mItems->setAutoDelete(true);
	
	mIter = new ItemListIterator(*mItems);
}
	
// Constructeur de recopie
ParamGroup::ParamGroup(const ParamGroup& group)
{
	mName = group.mName;
	
	mItems = new ItemList(*(group.mItems));
	mItems->setAutoDelete(true);
	
	mIter = new ItemListIterator(*mItems);
}

// Constructeur avec initialisation du nom
ParamGroup::ParamGroup(const QString& name)
{
	mName = new QString(name);
	
	mItems = new ItemList(29);
	mItems->setAutoDelete(true);
	
	mIter = new ItemListIterator(*mItems);
	
}

// Dectructeur
ParamGroup::~ParamGroup(void)
{
	if(mName != NULL)
		delete mName;
	if(mItems != NULL)
		delete mItems;
	if(mIter != NULL)
		delete mIter;
}

/**********************************************************************
****                 Fonctions de gestion du contenu               ****
**********************************************************************/
// Permet de changer le nom du sous-groupe
void ParamGroup::setName(QString& name)
{
	if(mName != NULL)
		delete mName;
	mName = new QString(name);
}

// Renvoie le nom du groupe de paramètres où la chaine vide si le nom
// n'a pas encore été donné
QString ParamGroup::getName(void) const
{
	if(mName != NULL)
		return *mName;
	else
		return QString("");
}

// Ajoute un paramètre dans la liste et renvoie true si tout s'est bien
// passé et false si une erreur se produit. Si un paramètre du même nom
// existe déjà, il est remplacé
bool ParamGroup::addParam(QString& nom, ParamItem* param)
{
	if(param != NULL)
	{
		mItems->remove(nom);
		mItems->insert(nom, param);
		return true;
	}
	else
		return false;
}

// Supprime un paramètre trouvé par son nom de la liste. Renvoie true
// si tout se passe bien, false sinon.
bool ParamGroup::removeParam(const QString& nom)
{
	return mItems->remove(nom);
}

// Récupère un pointeur vers un paramètre d'après son nom. Renvoie NULL
// si le nom n'est pas trouvé. Le paramètre n'est PAS enlevé de la
// liste
ParamItem* ParamGroup::getItem(const QString& nom) const
{
	return mItems->find(nom);
}

// Renvoie le nombre de paramètres stockés
uint ParamGroup::getSize(void) const
{
	return mItems->size();
}


/**********************************************************************
****            Fonctions liées à la mise en fichier               ****
**********************************************************************/
// Renvoie la chaine de caractères complète du nom du sous-groupe de
// paramètres où la chaine vide si le nom n'a pas encore été défini.
// La chaine a la forme "[nom]" sans retour à la ligne
QString ParamGroup::getGroupString(void) const
{
	QString res("");	
	if(mName != NULL)
		res += "[" + *mName + "]";
	return res;
}

// Renvoie la chaine de caractère complète du paramètre trouvé par son
// nom ou une chaine vide si la chaine n'a pas été trouvée
QString ParamGroup::getItemString(const QString& nom) const
{
	QString res("");
	ParamItem* tmpItem = mItems->find(nom);
	if(tmpItem != 0)
		res = nom + " = " + tmpItem->getValueString();
	return res;
}

// Renvoie la chaine de caractères complète du paramètre courant
// ou une chaine vide si le paramètre n'existe pas.
// La chaine a la forme "nom = valeur" sans retour à la ligne
QString ParamGroup::getCurrentItemString(void) const
{
	QString res("");
	ParamItem* tmpItem = mIter->current();
	if(tmpItem != 0)
		res = mIter->currentKey() + " = " + tmpItem->getValueString();
	return res;
}

// Renvoie le texte complet du sous-groupe sous la forme:
// 	[nomgroupe]
// 	nomparam1 = valeurparam1
// 	nomparam2 = valeurparam2
// 	....
// Le bloc se termine par un retour à la ligne mais ne débute PAS par
// un retour à la ligne
// Si le nom du groupe n'est pas encore affecté, la fonction renvoie
// une chaine vide. Si un paramètre n'a pas de nom ou de valeur, la
// ligne correspondant à ce paramètre est supprimée.
QString ParamGroup::getGroupBlock(void) const
{
	QString res("");	
	if(mName != NULL)
		res += "[" + *mName + "]" + RET_LIGNE;
	else
		return res;

	ParamItem* tmpItem;
	ItemListIterator iter(*mItems);
	for( ; (tmpItem = iter.current()) != 0 ; ++iter)
	{
		if(tmpItem != 0)
		{
			res += iter.currentKey();
			res += " = ";
			res += tmpItem->getValueString();
			res += RET_LIGNE;
		}
	}
	return res;
}
	
/**********************************************************************
****                Fonctions de parcours du contenu               ****
**********************************************************************/
// Réinitialise l'itérateur au premier élement
void ParamGroup::toBegin(void)
{
	mIter->toFirst();
}
// Renvoie l'élement pointé actuellement où NULL si l'élement a été
// supprimé ou si le dernier a été dépassé. L''élement n'est PAS
// supprimé.
ParamItem* ParamGroup::getCurrentItem(void) const
{
	return mIter->current();
}
// Renvoie le nom de l'élement pointé actuellement où une chaine vide
// si l'élement a été supprimé ou si le dernier a été dépassé.
// L'élement n'est PAS supprimé.
QString ParamGroup::getCurrentName(void) const
{
	return mIter->currentKey();
}

// Passe à l'élement suivant si possible.
bool ParamGroup::toNext(void)
{
	if( (++mIter) == 0 )
		return false;
	else
		return true;
}
