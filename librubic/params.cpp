/** \file params.cpp
  * \brief Contient la définition des fonctions membres de Params
  */
#include "params.h"

#include "paramitem.h"
#include "paramgroup.h"
#include "constants.h"
#include "lvect.h"
#include "dvect24.h"

#include <qstring.h>
#include <qfile.h>
#include <q3textstream.h>

#include <iostream>
using namespace std;

// Constructeur par défaut
Params::Params(void)
{
	mGroups = new GroupList(29);
	mGroups->setAutoDelete(true);
	
	mIter = new GroupListIterator(*mGroups);
}

// Constructeur de recopie
Params::Params(Params& param)
{
	mGroups = new GroupList(*(param.mGroups));
	
	mIter = new GroupListIterator(*mGroups);
}

// Destructeur
Params::~Params(void)
{
	if( mGroups != NULL )
		delete mGroups;
	if( mIter != NULL)
		delete mIter;
}

/*********************************************************************
****               Fonctions d'accès aux paramètres               ****
*********************************************************************/
// Ajouter un paramètre avec son groupe, son nom et sa valeur.
// Si le nom donné existe dans le groupe donné, l'ancienne valeur du
// paramètre sera supprimée. Si le groupe n'existe pas, il sera créé.
// De même si le nom n'existe pas, il sera créé.
void Params::insertShort(QString groupe, QString nom, short value)
{
	createGroup(groupe);
	ParamGroup* group = mGroups->find(groupe);
	group->addParam(nom, new ParamItem(value));
}
void Params::insertInt(QString groupe, QString nom, int value)
{
	createGroup(groupe);
	ParamGroup* group = mGroups->find(groupe);
	group->addParam(nom, new ParamItem(value));
}
void Params::insertLong(QString groupe, QString nom, long value)
{
	createGroup(groupe);
	ParamGroup* group = mGroups->find(groupe);
	group->addParam(nom, new ParamItem(value));
}
void Params::insertFloat(QString groupe, QString nom, float value)
{
	createGroup(groupe);
	ParamGroup* group = mGroups->find(groupe);
	group->addParam(nom, new ParamItem(value));
}
void Params::insertDouble(QString groupe, QString nom, double value)
{
	createGroup(groupe);
	ParamGroup* group = mGroups->find(groupe);
	group->addParam(nom, new ParamItem(value));
}
void Params::insertBool(QString groupe, QString nom, bool value)
{
	createGroup(groupe);
	ParamGroup* group = mGroups->find(groupe);
	group->addParam(nom, new ParamItem(value));
}
void Params::insertString(QString groupe, QString nom, QString value)
{
	createGroup(groupe);
	ParamGroup* group = mGroups->find(groupe);
	group->addParam(nom, new ParamItem(value));
}
void Params::insertDVect(QString groupe, QString nom, DVect value)
{
	createGroup(groupe);
	ParamGroup* group = mGroups->find(groupe);
	group->addParam(nom, new ParamItem(value));
}
void Params::insertLVect(QString groupe, QString nom, LVect value)
{
	createGroup(groupe);
	ParamGroup* group = mGroups->find(groupe);
	group->addParam(nom, new ParamItem(value));
}

// Récupérer un paramètre d'après son nom et son groupe.
// Si le nom ou le groupe donné n'existe pas ou si le paramètre ne
// contient pas de valeur, la fonction renvoie false. Si la valeur
// placée dans le paramètre est valide, la fonction renvoie true.
bool Params::getShort(QString groupe, QString nom, short* value) const
{
	if( (!groupExists(groupe)) || (value == NULL)  )
		return false;
	ParamGroup* group = mGroups->find(groupe);
	ParamItem* item = group->getItem(nom);
	if(item == NULL)
		return false;
	return item->getShortValue(value);
}

bool Params::getInt(QString groupe, QString nom, int* value) const
{
	if( (!groupExists(groupe)) || (value == NULL)  )
		return false;
	ParamGroup* group = mGroups->find(groupe);
	ParamItem* item = group->getItem(nom);
	if(item == NULL)
		return false;
	return item->getIntValue(value);
}

bool Params::getLong(QString groupe, QString nom, long* value) const
{
	if( (!groupExists(groupe)) || (value == NULL)  )
		return false;
	ParamGroup* group = mGroups->find(groupe);
	ParamItem* item = group->getItem(nom);
	if(item == NULL)
		return false;
	return item->getLongValue(value);
}

bool Params::getFloat(QString groupe, QString nom, float* value) const
{
	if( (!groupExists(groupe)) || (value == NULL)  )
		return false;
	ParamGroup* group = mGroups->find(groupe);
	ParamItem* item = group->getItem(nom);
	if(item == NULL)
		return false;
	return item->getFloatValue(value);
}

bool Params::getDouble(QString groupe, QString nom, double* value) const
{
	if( (!groupExists(groupe)) || (value == NULL)  )
		return false;
	ParamGroup* group = mGroups->find(groupe);
	ParamItem* item = group->getItem(nom);
	if(item == NULL)
		return false;
	return item->getDoubleValue(value);
}

bool Params::getBool(QString groupe, QString nom, bool* value) const
{
	if( (!groupExists(groupe)) || (value == NULL)  )
		return false;
	ParamGroup* group = mGroups->find(groupe);
	ParamItem* item = group->getItem(nom);
	if(item == NULL)
		return false;
	return item->getBoolValue(value);
}

bool Params::getString(QString groupe, QString nom, QString* value) const
{
	if( (!groupExists(groupe)) || (value == NULL)  )
		return false;
	ParamGroup* group = mGroups->find(groupe);
	ParamItem* item = group->getItem(nom);
	if(item == NULL)
		return false;
	return item->getStringValue(value);
}

bool Params::getDVect(QString groupe, QString nom, DVect* value) const
{
	if( (!groupExists(groupe)) || (value == NULL)  )
		return false;
	ParamGroup* group = mGroups->find(groupe);
	ParamItem* item = group->getItem(nom);
	if(item == NULL)
		return false;
	return item->getDVectValue(value);
}

bool Params::getLVect(QString groupe, QString nom, LVect* value) const
{
	if( (!groupExists(groupe)) || (value == NULL)  )
		return false;
	ParamGroup* group = mGroups->find(groupe);
	ParamItem* item = group->getItem(nom);
	if(item == NULL)
		return false;
	return item->getLVectValue(value);
}
// Ajouter  un paramètre avec son groupe, son nom et un pointeur.
// Si le nom donné existe dans le groupe donné, l'ancienne valeur du
// paramètre sera supprimée. Si le groupe n'existe pas, il sera créé.
// De même si le nom n'existe pas, il sera créé.
void Params::insertParam(QString groupe, QString nom, ParamItem* param)
{
	createGroup(groupe);
	ParamGroup* group = mGroups->find(groupe);
	if(param != NULL)
		group->addParam(nom, param);
}

// Récupérer un paramètre d'après son nom et son groupe.
// Si le nom ou le groupe donné n'existe pas ou si le paramètre ne
// contient pas de valeur, la fonction renvoie false. Si la valeur
// placée dans le paramètre est valide, la fonction renvoie true.
bool Params::getParam(QString groupe, QString nom, ParamItem* param) const
{
	if( !groupExists(groupe) )
		return false;
	ParamGroup* group = mGroups->find(groupe);
	ParamItem* item = group->getItem(nom);
	if(item == NULL)
		return false;
	param = item;
	return true;
}

/*********************************************************************
****             Fonctions liées à la mise en fichier             ****
*********************************************************************/
// Ecrit l'ensemble de la configuration dans le fichier spécifié en
// paramètre. Si le paramètre overwrite est mis à true et que le
// fichier spécifié existe déjà, alors il sera remplacé. Si la fonction
// échoue, la valeur renvoyée est false, sinon, c'est true qui sera
// renvoyée. Les causes d'echec sont multiples: le répertoire contenant
// le fichier n'existe pas, le fichier spécifié existe et n'est pas
// éditable ou overwrite vaut false, il est impossible de créer le
// fichier spécifié, etc.
bool Params::writeToFile(QString filename, bool overwrite) const
{
	QFile outFile(filename);
	if( outFile.exists() && !overwrite)
		return false;
	if( ! outFile.open(QIODevice::WriteOnly | QIODevice::Truncate) )
		return false;
	Q3TextStream outStream( &outFile );
	GroupListIterator iter(*mGroups);
	
	ParamGroup* tmpGroup;
	for(; (tmpGroup = iter.current()) != 0 ; ++iter)
		outStream << tmpGroup->getGroupBlock();
	outFile.flush();
	outFile.close();
	return true;
}
	

// Lit l'ensemble de la configuration depuis le fichier spécifié.
// La fonction renvoie true si tout se passe bien et false si une
// erreur se produit. Les causes d'erreur sont multiples: fichier
// spécifié introuvabe ou non-lisible, erreur de syntaxe dans le
// fichier, ...
bool Params::readFromFile(QString filename)
{
	QFile inFile(filename);
	if( ! inFile.exists() )
	{
// 		cout << "Fichier " << wfilename << " inexistant!" << endl;
		return false;
	}
	if( ! inFile.open(QIODevice::ReadOnly | QIODevice::Text) )
	{
// 		cout << "Impossible d'ouvrir " << filename << endl;
		return false;
	}
	QString line;
	QString curGroup = QString::null;
	QString paramName, paramValue;
	ParamItem* param;
	char buf[1024];
	while(inFile.readLine(buf, LONG_MAX_LIGNE_CONF) > 0)
	{
		line = buf;
		// On supprime les espaces en début et fin de ligne
		line = line.stripWhiteSpace();
		// On ignore les lignes de commentaire
		if( line.startsWith("#") || (line == "") )
			continue;
		// On traite les lignes définissant le nom du groupe
		if( line.startsWith("[") )
		{
			// La ligne doit finir par "]" sinon on s'arrète
			if( !line.endsWith("]") )
			{
// 				cout << "Ligne començant par [ mais sans ]" << endl;
				break;
			}
			curGroup = line.mid(1, line.length()-2);
			// On vérifie qu'il n'y a pas eu d'erreurs sinon on
			// s'arrète
			if(curGroup == QString::null)
			{
// 				cout << "Problème dans la lecture du nom de groupe" << endl;
				break;
			}
			curGroup = curGroup.stripWhiteSpace();
			// On passe à la ligne suivante
			continue;
		}
		// Le nom du paramètre est le premier champ avant " = "
		paramName = line.section("=", 0, 0).stripWhiteSpace();
		// La valeur est le reste de la ligne après " = "
		paramValue = line.section("=", 1).stripWhiteSpace();
		
		// On vérifie le nom
		if( (paramName == QString::null) || (paramName == QString("")) )
		{
// 			cout << "Problème avec le nom de la variable" << endl;
			break;
		}
		// On vérifie la valeur
		if( (paramValue == QString::null) || (paramValue == QString("")) )
		{
// 			cout << "Problème avec la valeur de la variable" << endl;
			break;
		}
		// On crée le paramètre
		param = ParamItem::createParamFromString(paramValue);
		// Si une erreur s'est produite, on s'arrète
		if(param == NULL)
		{
// 			cout << "Impossible de créer le paramètre " << paramName
// 				<< " avec " << paramValue << endl;
			break;
		}
		// On devrait déjà être entrés dans un groupe
		// Sinon on s'arrète
		if(curGroup == QString::null)
		{
// 			cout << "Définition d'une variable avant le nom du premier groupe" << endl;
			delete param;
			break;
		}
		createGroup(curGroup);
		// On ajoute la paramètre
		insertParam(curGroup, paramName, param);
	} // while de traitement de ligne
	if( !inFile.atEnd() )
	{
// 		cout << "Fin du traitement avant la fin du fichier" << endl;
		return false;
	}
	return true;
}

/**********************************************************************
****        Fonctions de gestion des groupes de paramètres         ****
**********************************************************************/
// Renvoie true si le groupe dont le nom est passé en paramètre existe
// déjà, sinon renvoie false.
bool Params::groupExists(const QString groupe) const
{
	if(mGroups->find(groupe) != 0)
		return true;
	return false;
}

// Crée le groupe dont le nom est donné en paramètre si il n'existe pas
// encore.
void Params::createGroup(const QString groupe)
{
	if( !groupExists(groupe) )
		mGroups->insert(groupe, new ParamGroup(groupe));
}
