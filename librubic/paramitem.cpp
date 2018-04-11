/** \file paramitem.cpp
  * \brief Contient la définition des fonctions membres de ParamItem
  */
#include "paramitem.h"
#include "constants.h"

#include <qstring.h>
#include <qstringlist.h>
#include <qregexp.h>

// #include <iostream>
// using namespace std;

// Constructeur par défaut
ParamItem::ParamItem(void)
{
	mType = UndefinedParam;
}

// Constructeur avec une valeur short
ParamItem::ParamItem(const short val)
{
	mType = ShortParam;
	//mDataShort = val;
	mDataLong = (long) val;
}

// Constructeur avec une valeur int
ParamItem::ParamItem(const int val)
{
	mType = IntParam;
	//mDataInt = val;
	mDataLong = (long) val;
}

// Constructeur avec une valeur long
ParamItem::ParamItem(const long val)
{
	mType = LongParam;
	mDataLong = val;
}

// Constructeur avec une valeur float
ParamItem::ParamItem(const float val)
{
	mType = FloatParam;
	//mDataFloat = val;
	mDataDouble = (double) val;
}

// Constructeur avec une valeur double
ParamItem::ParamItem(const double val)
{
	mType = DoubleParam;
	mDataDouble = val;
}

// Constructeur avec une valeur bool
ParamItem::ParamItem(const bool val)
{
	mType = BoolParam;
	mDataBool = val;
}

// Constructeur avec une valeur QString
ParamItem::ParamItem(const QString& val)
{
	mType = StringParam;
	mDataString = new QString(val);
}

// Constructeur avec une valeur chaine
ParamItem::ParamItem(const char* val)
{
	mType = StringParam;
	mDataString = new QString(val);
}

// Constructeur avec une valeur DVect
ParamItem::ParamItem(const DVect& val)
{
	mType = DVectParam;
	mDataDVect = new DVect(val);
}

// Constructeur avec une valeur DVect
ParamItem::ParamItem(const LVect& val)
{
	mType = LVectParam;
	mDataLVect = new LVect(val);
}

// Destructeur
ParamItem::~ParamItem()
{
	if( (mType == StringParam) && (mDataString != NULL) )
		delete mDataString;
	else if ( (mType == DVectParam) && (mDataDVect != NULL) )
		delete mDataDVect;
	else if ( (mType == LVectParam) && (mDataLVect != NULL) )
		delete mDataLVect;
}

/**********************************************************************
****           Fonctions de gestion du type de données             ****
**********************************************************************/
// Renvoie la constante définissant le type de paramètre
const short int ParamItem::getType(void) const
{
	return mType;
}

// Modifie le type de données stocké
void ParamItem::setType(mDataType type)
{
	if( (mType == StringParam) && (mDataString != NULL) )
		delete mDataString;
	else if ( (mType == DVectParam) && (mDataDVect != NULL) )
		delete mDataDVect;
	else if ( (mType == LVectParam) && (mDataLVect != NULL) )
		delete mDataLVect;
	mType = type;
}
	
/**********************************************************************
****            Fonctions de récupération des données              ****
**********************************************************************/
// Renvoie le contenu de mDataShort dans le paramètre
// Si l'objet contient un short, renvoie true, sinon false
bool ParamItem::getShortValue(short* val) const
{
	if(mType == ShortParam)
	{
		//*val = mDataShort;
		*val = (short)mDataLong;
		return true;
	}
	else
		return false;
}

// Renvoie le contenu de mDataInt dans le paramètre
// Si l'objet contient un int, renvoie true, sinon false
bool ParamItem::getIntValue(int* val) const
{
	if(mType == IntParam)
	{
		//*val = mDataInt;
		*val = (int)mDataLong;
		return true;
	}
	else
		return false;
}

// Renvoie le contenu de mDataLong dans le paramètre
// Si l'objet contient un long, renvoie true, sinon false
bool ParamItem::getLongValue(long* val) const
{
	if(mType == LongParam)
	{
		*val = mDataLong;
		return true;
	}
	else
		return false;
}

// Renvoie le contenu de mDataFloat dans le paramètre
// Si l'objet contient un float, renvoie true, sinon false
bool ParamItem::getFloatValue(float* val) const
{
	if(mType == FloatParam)
	{
		//*val = mDataFloat;
		*val = (float)mDataDouble;
		return true;
	}
	else
		return false;
}

// Renvoie le contenu de mDataDouble dans le paramètre
// Si l'objet contient un double, renvoie true, sinon false
bool ParamItem::getDoubleValue(double* val) const
{
	if(mType == DoubleParam)
	{
		*val = mDataDouble;
		return true;
	}
	else
		return false;
}

// Renvoie le contenu de mDataBool dans le paramètre
// Si l'objet contient un booleen, renvoie true, sinon false
bool ParamItem::getBoolValue(bool* val) const
{
	if(mType == BoolParam)
	{
		*val = mDataBool;
		return true;
	}
	else
		return false;
}

// Renvoie le contenu de mDataString dans le paramètre
// Si l'objet contient une chaine, renvoie true, sinon false
bool ParamItem::getStringValue(QString* val) const
{
	if(mType == StringParam)
	{
		*val = *mDataString;
		return true;
	}
	else
		return false;
}

/* Renvoie le contenu de mDataDVect dans le paramètre.
* Si l'objet contient un DVect, renvoie true, sinon false.
*/
bool ParamItem::getDVectValue(DVect* vec)
{
	if(mType == DVectParam)
	{
		*vec = *mDataDVect;
		return true;
	}
	else
		return false;
}
/* Renvoie le contenu de mDataDVect dans le paramètre.
* Si l'objet contient un LVect, renvoie true, sinon false.
*/
bool ParamItem::getLVectValue(LVect* vec)
{
	if(mType == LVectParam)
	{
		*vec = *mDataLVect;
		return true;
	}
	else
		return false;
}

// Modifie le contenu si le type est compatible
// Renvoie true si la modification a été effectuée, false sinon
bool ParamItem::setLongValue(long val)
{
	if(mType == UndefinedParam)
		mType = LongParam;
	if(mType == LongParam)
	{
		mDataLong = val;
		return true;
	}
	else
		return false;
}

// Modifie le contenu si le type est compatible
// Renvoie true si la modification a été effectuée, false sinon
bool ParamItem::setDoubleValue(double val)
{
	if(mType == UndefinedParam)
		mType = DoubleParam;
	if(mType == DoubleParam)
	{
		mDataDouble = val;
		return true;
	}
	else
		return false;
}

// Modifie le contenu si le type est compatible
// Renvoie true si la modification a été effectuée, false sinon
bool ParamItem::setBoolValue(bool val)
{
	if(mType == UndefinedParam)
		mType = BoolParam;
	if(mType == BoolParam)
	{
		mDataBool = val;
		return true;
	}
	else
		return false;
}

// Modifie le contenu si le type est compatible
// Renvoie true si la modification a été effectuée, false sinon
bool ParamItem::setStringValue(QString* val)
{
	if(mType == UndefinedParam)
		mType = StringParam;
	if(mType == StringParam)
	{
		if(mDataString != NULL)
			delete mDataString;
		mDataString = new QString(*val);
		return true;
	}
	else
		return false;
}

/* Modifie le contenu si le type est compatible.
* Renvoie true si la modification a été effectuée, false sinon.
*/
bool ParamItem::setDVectValue(DVect vec)
{
	if(mType == UndefinedParam)
		mType = DVectParam;
	if(mType == DVectParam)
	{
		if(mDataDVect != NULL)
			delete mDataDVect;
		mDataDVect = new DVect(vec);
		return true;
	}
	else
		return false;
}

/* Modifie le contenu si le type est compatible.
* Renvoie true si la modification a été effectuée, false sinon.
*/
bool ParamItem::setLVectValue(LVect vec)
{
	if(mType == UndefinedParam)
		mType = LVectParam;
	if(mType == LVectParam)
	{
		if(mDataLVect != NULL)
			delete mDataLVect;
		mDataLVect = new LVect(vec);
		return true;
	}
	else
		return false;
}

/**********************************************************************
****            Fonctions liées à la mise en fichier               ****
**********************************************************************/
// Renvoie une chaine de caractères contenant les données
QString ParamItem::getValueString(void) const
{
	switch(mType)
	{
		case ShortParam:
			return QString::number((short)mDataLong);
		case IntParam:
			return QString::number((int)mDataLong);
		case LongParam:
			return QString::number(mDataLong);
		case FloatParam:
			return QString::number((float)mDataDouble, 'e', PREC_FLOAT_CONF);
		case DoubleParam:
			return QString::number(mDataDouble, 'e', PREC_DOUBLE_CONF);
		case BoolParam:
			return ( mDataBool ? QString("true") : QString("false") );
		case StringParam:
			return *mDataString;
		case DVectParam:
			return QString("%1;%2;%3").arg(mDataDVect->getX()).arg(mDataDVect->getY()).arg(mDataDVect->getZ());
		case LVectParam:
			return QString("%1;%2;%3").arg(mDataLVect->getX()).arg(mDataDVect->getY()).arg(mDataDVect->getZ());
		default:
			return QString("ERREUR dans ParamItem::getValueString()");
	}
}

// Renvoie la constante associée au type de paramètre contenu dans la
// chaine de caractères spécifiée. La constante UndefinedParam est
// renvoyée si le type n'est pas reconnu.
const short ParamItem::getStringParamType(const QString str)
{
	// Traitements préliminaires de la chaine:
	QString clean = str.stripWhiteSpace();
	
	// Si la chaine est vide, renvoie UndefinedParam
	if( clean.isEmpty() )
	{
// 		cout << "La chaine à convertir est vide" << endl;
// 		cout << "original: " << str << endl;
// 		cout << "modifiée: " << clean << endl;
		return UndefinedParam;
	}
	
	// Version avec tout en minuscules
	QString minus = clean.lower();
	
	// Testons s'il s'agit d'un booléen (true ou false), sans être sensible
	// à la casse.
	if ( (minus == QString("true")) || (minus == QString("false")) )
		return BoolParam;
// 	cout << "Ce n'est pas un booleen" << endl;
	
	// Testons s'il s'agit d'un nombre entier (que des chiffres avec ou non
	// le signe + ou -)
	QRegExp entier("^[+-]?[0-9]+$");
	if( clean.contains(entier) )
	{
		// On convertit en un long
		bool reussi;
		clean.toLong(&reussi);
		// Si la conversion échoue, on renvoie le type indeterminé
		if( !reussi )
		{
// 			cout << "Erreur de conversion d'entier" << endl;
			return UndefinedParam;
		}
		return LongParam;
	}
// 	cout << "Ce n'est pas un entier" << endl;
		
	// Testons s'il s'agit d'un nombre à virgule avec ou sans exponentielle
	QRegExp virgule("^[+-]?([0-9]+[.]|[.][0-9]+|[0-9]+[.][0-9]+)$");
	QRegExp expon("^[+-]?([0-9]+[.]|[.][0-9]+|[0-9]+[.][0-9]+|[0-9]+)[eE][+-]?[0-9]+$");
	if( clean.contains(virgule) || clean.contains(expon) )
	{
		// On vérifie que l'on peut convertir en un double
		bool reussi;
		clean.toDouble(&reussi);
		// Si la conversion échoue, on renvoie le type indeterminé
		if( !reussi )
		{
// 			cout << "Erreur de conversion de nombre à virgule" << endl;
			return UndefinedParam;
		}

		return DoubleParam;
	}
// 	cout << "Ce n'est pas une valeur à virgule" << endl;
	
	// Testons s'il s'agit d'un LVect
	QRegExp lvect("^[+-]?[0-9]+;[+-]?[0-9]+;[+-]?[0-9]+$");
	if( clean.contains(lvect) )
	{
		bool reussi = true;
		QStringList parts = QStringList::split(";", clean, FALSE);
		if( parts.size() == 3 )
		{
			for ( QStringList::Iterator it = parts.begin(); it != parts.end(); ++it )
			{
				bool test;
				(*it).toLong(&test);
				if(! test)
					reussi = false;
			}
			if( reussi )
				return LVectParam;
		}
	}
	
	// Testons s'il s'agit d'un DVect
	QString dvect1base("([+-]?([0-9]+[.]|[.][0-9]+|[0-9]+[.][0-9]+))");
	QRegExp dvect1( QString("^%1;%2;%3$").arg(dvect1base).arg(dvect1base).arg(dvect1base) );
	QString dvect2base("([+-]?([0-9]+[.]|[.][0-9]+|[0-9]+[.][0-9]+|[0-9]+)[eE][+-]?[0-9]+)");
	QRegExp dvect2( QString("^%1;%2;%3$").arg(dvect2base).arg(dvect2base).arg(dvect2base) );
	if( clean.contains(dvect1) || clean.contains(dvect2) )
	{
		bool reussi = true;
		QStringList parts = QStringList::split(";", clean, FALSE);
		if( parts.size() == 3 )
		{
			for ( QStringList::Iterator it = parts.begin(); it != parts.end(); ++it )
			{
				bool test;
				(*it).toDouble(&test);
				if(! test)
					reussi = false;
			}
			if( reussi )
				return DVectParam;
		}
	}
	
	// Sinon on renvoie StringParam
	return StringParam;
}

// Crée un Paramètre d'après le contenu sous forme de chaine de
// caractères de sa valeur. Renvoie un pointeur vers l'élement créé ou
// la valeur NULL si une erreur se produit.
ParamItem* ParamItem::createParamFromString(const QString str)
{
	short paramType = getStringParamType(str);
	if( paramType == UndefinedParam )
	{
// 		cout << "Impossible de déterminer le type de paramètre de " << str << endl;
		return NULL;
	}
		
	str.stripWhiteSpace();
	bool convres;
	switch(paramType)
	{
		case ShortParam:
		{
			short result = str.toShort(&convres);
			if(convres)
				return new ParamItem(result);
// 			cout << "Erreur de création d'un ParamItem avec un short" << endl;
			return NULL;
		}
		case IntParam:
		{
			int result = str.toInt(&convres);
			if(convres)
				return new ParamItem(result);
// 			cout << "Erreur de création d'un ParamItem avec un int" << endl;
			return NULL;
		}
		case LongParam:
		{
			long result = str.toLong(&convres);
			if(convres)
				return new ParamItem(result);
// 			cout << "Erreur de création d'un ParamItem avec un long" << endl;
			return NULL;
		}
		case FloatParam:
		{
			float result = str.toFloat(&convres);
			if(convres)
				return new ParamItem(result);
// 			cout << "Erreur de création d'un ParamItem avec un float" << endl;
			return NULL;
		}
		case DoubleParam:
		{
			double result = str.toDouble(&convres);
			if(convres)
				return new ParamItem(result);
// 			cout << "Erreur de création d'un ParamItem avec un double" << endl;
			return NULL;
		}
		case BoolParam:
		{
			if(str.lower() == QString("true"))
				return new ParamItem(true);
			if(str.lower() == QString("false"))
				return new ParamItem(false);
// 			cout << "Erreur de création d'un ParamItem avec un bool" << endl;
			return NULL;
		}
		case StringParam:
		{
			return new ParamItem(str);
		}
		case DVectParam:
		{
			DVect result;
			bool reussi = true;
			QStringList parts = QStringList::split(";", str, FALSE);
			for (int i = 0; i<3; i++ )
			{
				bool test;
				result[i] = parts[i].toDouble(&test);
				if(! test)
					reussi = false;
			}
			if( reussi )
				return new ParamItem(result);
			else
				return NULL;
		}
		case LVectParam:
		{
			LVect result;
			bool reussi = true;
			QStringList parts = QStringList::split(";", str, FALSE);
			for (int i = 0; i<3; i++ )
			{
				bool test;
				result[i] = parts[i].toLong(&test);
				if(! test)
					reussi = false;
			}
			if( reussi )
				return new ParamItem(result);
			else
				return NULL;
		}
		default:
// 			cout << "!paramitem.cpp: on ne devrait pas arriver ici" << endl;
			return NULL;
	}
}
