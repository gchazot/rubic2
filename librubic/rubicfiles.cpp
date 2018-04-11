/** \file rubicfiles.cpp
  * \brief Contient les définitions des fonctions membres de RubicFiles
  */

#include "rubicfiles.h"
#include "rubicimage.h"
#include "lvect.h"

#include <qwidget.h>
#include <qstring.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qmessagebox.h>
#include <qregexp.h>
#include <q3textstream.h>

#include <iostream>
using namespace std;

//Constructeur par défaut
RubicFiles::RubicFiles(void)
{
	mXSize = mYSize = mZSize = 0;
	mVgiFileName = NULL;
	mRawFileName = NULL;
}

// Destructeur
RubicFiles::~RubicFiles()
{
	if(mVgiFileName != NULL)
		delete mVgiFileName;
	if(mRawFileName != NULL)
		delete mRawFileName;
}

// Renvoie le nom du fichier vgi ou la chaine nulle si il n'est pas défini
QString RubicFiles::getVgiFileName(void) const
{
	if(mVgiFileName != NULL)
		return *mVgiFileName;
	return QString::null;
}
// Renvoie le nom du fichier raw ou la chaine nulle si il n'est pas défini
QString RubicFiles::getRawFileName(void) const
{
	if(mRawFileName != NULL)
		return *mRawFileName;
	return QString::null;
}

// Modifie le nom du fichier vgi
void RubicFiles::setVgiFileName(const QString& name)
{
	if(mVgiFileName != NULL)
		delete mVgiFileName;
	mVgiFileName = new QString(name);
}
// Modifie le nom du fichier raw
void RubicFiles::setRawFileName(const QString& name)
{
	if(mRawFileName != NULL)
		delete mRawFileName;
	mRawFileName = new QString(name);
}

// Crée le nom du fichier raw d'après le nom du fichier vgi
// Renvoie true si la création a réussi et false sinon
bool RubicFiles::setRawFileNameFromVgiFileName(void)
{
	// On vérifie que le nom du fichier vgi existe
	if( mVgiFileName == NULL )
		return false;
	// On vérifie que l'extension est .vgi
	if( !mVgiFileName->endsWith(".vgi", false) )
		return false;
	
	// On copie le nom du vgi dans le nom du raw
	if(mRawFileName != NULL)
		delete mRawFileName;
	mRawFileName = new QString(*mVgiFileName);
	
	// On vérifie que la mémoire a été allouée
	if(mRawFileName == NULL)
		return false;
	
	// On remplace .vgi par .raw dans le nom du raw
	mRawFileName->replace(".vgi", ".raw", false);
	// On vérifie l'échec du remplacement
	if(mRawFileName == mVgiFileName)
		return false;
	
	// Voilà :)
	return true;
}

LVect RubicFiles::getSize(void) const
{
	return LVect(mXSize, mYSize, mZSize);
}

// Renvoie la dimension X de l'image
long RubicFiles::getXSize(void) const
{
	return mXSize;
}

// Renvoie la dimension Y de l'image
long RubicFiles::getYSize(void) const
{
	return mYSize;
}

// Renvoie la dimension Z de l'image
long RubicFiles::getZSize(void) const
{
	return mZSize;
}

// Modifie la dimension X de l'image
void RubicFiles::setXSize(long val)
{
	mXSize= val;
}

// Modifie la dimension Y de l'image
void RubicFiles::setYSize(long val)
{
	mYSize= val;
}

// Modifie la dimension Z de l'image
void RubicFiles::setZSize(long val)
{
	mZSize= val;
}


// Modifie toutes les dimensions de l'image:
void RubicFiles::setSize(long xval, long yval,long zval)
{
	mXSize = xval;
	mYSize = yval;
	mZSize = zval;
}

// Vérifie si les fichiers correspondant à l'objet existent et affiche
// un avertissement avec confirmation d'écrasement si ils existent
// La fonction renvoie true si les fichiers n'existent pas ou si
// l'utilisateur a décidé de les écraser. Elle renvoie false si les
// fichiers existent mais que l'utilisateur ne veut pas les supprimer
// ou si on ne peut pas écrire dedans (droits d'accès), ou si une autre
// erreur se produit.
bool RubicFiles::checkFilesOverwrite(QWidget* parent) const
{
	// On vérifie que les deux noms de fichier sont initalisés.
	if( (mVgiFileName == NULL) || (mRawFileName == NULL) )
		return false;
	
	// On vérifie que l'on peut écrire dans les deux fichiers.
	QFileInfo vgiInfo(*mVgiFileName);
	QFileInfo rawInfo(*mRawFileName);
	if( vgiInfo.exists() && !vgiInfo.isWritable() )
		return false;
	if( rawInfo.exists() && !rawInfo.isWritable() )
		return false;
	
	// Si le fichier vgi existe, on demande confirmation à l'utilisateur
	if( QFile::exists(*mVgiFileName) )
		if( QMessageBox::warning(parent, "Ecrasement de fichier",
			"Le fichier :\n" + *mVgiFileName +
			"\nexiste déjà, il sera écrasé!\nEtes-vous certain de vouloir le remplacer?",
			QMessageBox::Yes | QMessageBox::Default,
			QMessageBox::No | QMessageBox::Escape) != QMessageBox::Yes )
			return false;
	// On fait de même avec le fichier raw
	if( QFile::exists(*mRawFileName) )
		if( QMessageBox::warning(parent, "Ecrasement de fichier",
			"Le fichier :\n" + *mRawFileName +
			"\n existe déjà, il sera écrasé!\nEtes-vous certain de vouloir le remplacer?",
			QMessageBox::Yes | QMessageBox::Default,
			QMessageBox::No | QMessageBox::Escape) != QMessageBox::Yes )
			return false;
	// Tout est OK, on peut (tenter d') écrire dans les fichiers
	return true;	
}

// Vérifie si les fichiers correspondant à l'objet existent et s'ils
// sont lisibles. La fonction renvoie true s'ils existent et sont
// lisibles. Elle renvoie false sinon
bool RubicFiles::checkFilesRead(void) const
{
	// On vérifie que les deux noms de fichier sont initalisés.
	if( (mVgiFileName == NULL) || (mRawFileName == NULL) )
		return false;
	
	// On vérifie que les deux fichiers existent
	if( QFile::exists(*mVgiFileName) && QFile::exists(*mRawFileName) )
	{
		// On vérifie que l'on peut lire dans les deux fichiers.
		QFileInfo vgiInfo(*mVgiFileName);
		QFileInfo rawInfo(*mRawFileName);
		if( vgiInfo.isReadable() && rawInfo.isReadable() )
			return true;
	}
	return false;
}

// Vérifie si l'on peut écrire dans les fichiers correspondant à
// l'objet. Renvoie true si c'est possible, false sinon
bool RubicFiles::checkFilesWrite(void) const
{
	// On vérifie que les deux noms de fichier sont initalisés.
	if( (mVgiFileName == NULL) || (mRawFileName == NULL) )
		return false;
	
	QFileInfo vgiInfo(*mVgiFileName);
	QFileInfo rawInfo(*mRawFileName);
	
	// On vérifie si le fichier vgi existe. S'il existe, on vérifie
	// que l'on peut écrire dedans
	if( vgiInfo.exists() && (! vgiInfo.isWritable()) )
		return false;
	
	if( rawInfo.exists() && (! rawInfo.isWritable()) )
		return false;

	return true;
}

// Lit la taille de l'image depuis le fichier vgi. Renvoie true en
// cas de succès, false sinon
bool RubicFiles::readSizeFromVgi(void)
{
	// On vérifie que le nom du fichier vgi existe
	if( mVgiFileName == NULL )
		return false;
	// On vérifie que le fichier existe
	if( !QFile::exists(*mVgiFileName) )
	{
		cout << "Le fichier n'existe pas : " << mVgiFileName->toStdString() << endl;
		return false;
	}
	
	// On vérifie que l'on peut lire dans le fichier
	QFileInfo vgiInfo(*mVgiFileName);
	if( !vgiInfo.isReadable() )
	{
		cout << "Impossible d'écrire dans : " << mVgiFileName->toStdString() << endl;
		return false;
	}
		
	// Ouverture du fichier
	QFile vgiFile(*mVgiFileName);
	if( ! vgiFile.open(QIODevice::ReadOnly | QIODevice::Text) )
	{
		cout << "Impossible d'ouvrir : " << mVgiFileName->toStdString() << endl;
		return false;
	}
	
	// Lecture du fichier ligne par ligne
	QString line;
	char buf[1024];
	while( vgiFile.readLine(buf, 128) > 0)
	{
		line = buf;
		line = line.stripWhiteSpace();
		// On ignore les lignes de commentaire et de titre
		QRegExp ignored("^[#\[\{]");
		if( line.contains(ignored) )
			continue;
		// La ligne qui nous intéresse
		if( line.startsWith("size") )
		{
			// On récupère la partie après le "="
			QString sizes = line.section("=", 1).stripWhiteSpace();
			QRegExp sep("[ \t]+", false); // Un ou + caractères blancs (tab, espace)
			// On récupère les composantes
			QString xSize = sizes.section(sep, 0, 0);
			QString ySize = sizes.section(sep, 1, 1);
			QString zSize = sizes.section(sep, 2, 2);
			// Traduction des composantes
			bool xOk, yOk, zOk;
			long xRead, yRead, zRead;
			xRead = xSize.toLong( &xOk );
			yRead = ySize.toLong( &yOk );
			zRead = zSize.toLong( &zOk );
			// On vérifie que tout s'est bien passé:
			if( xOk && yOk && zOk)
			{
				mXSize = xRead;
				mYSize = yRead;
				mZSize = zRead;
				return true;
			}
			// Sinon on arrète la boucle
			cout << "Erreur de conversion de la ligne : " << endl;
			cout << "\t" << line.toStdString() << endl;
			cout << "\tTailles : " << sizes.toStdString() << endl;
			cout << "\tX : " << xSize.toStdString() << " -> " << (xOk?"ok":"erreur") << endl;
			cout << "\tY : " << ySize.toStdString() << " -> " << (yOk?"ok":"erreur") << endl;
			cout << "\tZ : " << zSize.toStdString() << " -> " << (zOk?"ok":"erreur") << endl;
			cout << flush;
			break;
		}// Fin if
	}// Fin while
	// Si on arrive ici, c'est que l'on a pas trouvé la ligne don ECHEC
	cout << "Paramètre \"size\" introuvable dans : " << mVgiFileName->toStdString() << endl;
	return false;
}

//Ecrit un fichier vgi minimal contenant uniquement la taille de
// l'image. Renvoie true en cas de succès et false sinon.
// Si le paramètreoverwrite est à false et que le fichier vgi existe
// déjà, alors le fichier ne sera pas écrit et la fonction renverra
// false. Si la paramètre est à true, un fichier vgi existant sera
// remplacé par un nouveau
bool RubicFiles::writeVgiFile(bool overwrite) const
{
	// On vérifie que le nom du fichier vgi existe
	if( mVgiFileName == NULL )
		return false;
	// On voit si le fichier existe et si overwrite vaut true
	if( QFile::exists(*mVgiFileName) && (!overwrite) )
		return false;
	// On vérifie que l'on peut écrire dans le fichier
	QFileInfo vgiInfo(*mVgiFileName);
	if( vgiInfo.exists() && !vgiInfo.isWritable() )
		return false;
		
	// Ouverture du fichier
	QFile vgiFile(*mVgiFileName);
	if( ! vgiFile.open(QIODevice::WriteOnly | QIODevice::Text) )
		return false;
	Q3TextStream outStream( &vgiFile );
	
	// Création de la ligne de paramètres contenant les dimensions
	QString line("size = %1 %2 %3");
	line = line.arg(mXSize).arg(mYSize).arg(mZSize);
	//Ecriture de la ligne
	outStream << line << endl;
	// Fermeture du fichier
	vgiFile.close();
	
	return true;
}

// Lit le fichier .raw et alloue la mémoire du tableau donné en
// paramètre pour y placer les données.
// Renvoie true si l'opération réussit. Renvoie false et NULL dans
// le paramètre si une erreur de lecture ou d'allocation de la mémoire
// s'est produite.
bool RubicFiles::readRawFile(unsigned char ****data) const
{
	// On vérifie que le nom du fichier vgi existe
	if( mRawFileName == NULL )
		return false;
	// On vérifie que le fichier existe
	if( !QFile::exists(*mRawFileName) )
		return false;
	// On vérifie que l'on peut lire dans le fichier
	QFileInfo rawInfo(*mRawFileName);
	if( !rawInfo.isReadable() )
		return false;

	// Ouverture du fichier
	QFile rawFile(*mRawFileName);
	if( ! rawFile.open(QIODevice::ReadOnly | QIODevice::Text) )
		return false;

	// Allocation de la mémoire pour le tableau;
	data = new unsigned char***[mXSize];
	if(data == NULL)
		return false;
	for(long i=0; i<mXSize; i++)
	{
		data[i] = new unsigned char**[mYSize];
		if(data[i] == NULL)
			return false;
		for(long j=0; j<mYSize; j++)
		{
			data[i][j] = new unsigned char*[mZSize];
			if(data[i][j] == NULL)
				return false;
			for(long k=0; k<mZSize; k++)
			{
				data[i][j][k] = new unsigned char[3];
				if(data[i][j][k] == NULL)
					return false;
			}
		}
	}

	int buf;

	for(long k=0; k<mZSize; k++)
		for(long j=0; j<mYSize; j++)
			for(long i=0; i<mXSize; i++)
// 				for(short c=0; c<3; c++)
// 				{
// 					data[i][j][k][c] = rawFile.getch();
// 					if(data[i][j][k][c] == -1)
// 						return false;
// 				}
			{
				buf = rawFile.getch();
				if(buf == -1)
				{
					cout << "Erreur de lecture de fichier :" << endl;
					cout << rawFile.errorString().toStdString() << endl;
					return false;
				}
				for(short c=0; c<3; c++)
					data[i][j][k][c] = (unsigned char)buf;
			}
	return true;
}

// Lit le fichier .raw et crée l'objet RubicImage avec les données
// qu'il contient. Renvoie true si l'opération réussit. Renvoie false
// si une erreur de lecture ou d'allocation de la mémoire s'est
// produite.
bool RubicFiles::readRawFile(RubicImage **img)
{
	// Vérification des dimensions
	if( (mXSize <= 0) || (mYSize <= 0) || (mZSize <= 0) )
	{
		return false;
	}
	
	// On vérifie que le nom du fichier vgi existe
	if( mRawFileName == NULL )
	{
		return false;
	}
	
	// On vérifie que le fichier existe
	if( !QFile::exists(*mRawFileName) )
	{
		return false;
	}
	
	// On vérifie que l'on peut lire dans le fichier
	QFileInfo rawInfo(*mRawFileName);
	if( !rawInfo.isReadable() )
	{
		return false;
	}

	// Ouverture du fichier
	QFile rawFile(*mRawFileName);
	if( ! rawFile.open(QIODevice::ReadOnly | QIODevice::Text) )
	{
		return false;
	}

	// Création de l'objet RubicImage
	*img = NULL;
	*img = new RubicImage(mXSize, mYSize, mZSize);

	if(*img == NULL)
		return false;
	
	if( ! (*img)->isValid() )
	{
		delete img;
		img = NULL;
		return false;
	}
	
	// Lecture du tableau
	int buf;
// 	char moy;
	for(long k=0; k<mZSize; k++)
		for(long j=0; j<mYSize; j++)
			for(long i=0; i<mXSize; i++)
			{
// 				moy = 0;
// 				for(short c=0; c<3; c++)
// 				{
// 					buf = rawFile.getch();
// 					if(buf == -1)
// 					{
// 						delete img;
// 						*img = NULL;
// 						return false;
// 					}
// 					moy += buf;
// 				}
// 				if( (moy % 3) == 2 )
// 					moy = moy / 3 + 1;
// 				else
// 					moy = moy / 3;
// 				(*img)->setData(moy, i, j, k);
				
				buf = rawFile.getch();
				if(buf == -1)
				{
					cout << "Erreur de lecture de fichier :" << endl;
					cout << rawFile.errorString().toStdString() << endl;
					delete *img;
					*img = NULL;
					return false;
				}
				(*img)->setData((unsigned char)buf, i, j, k);
			}
	
	rawFile.close();
	return true;
}

// Ecrit les données passées par le paramètre data dans le fichier .raw
// La fonction renvoie true si tout se passe bien ou false si une erreur
// d'écriture se produit. La taille du tableau en paramètre doit être
// supérieure ou égale à celle donnée par mXSize * mYSize * mZSize * 
bool RubicFiles::writeRawFile(const unsigned char ****data, bool overwrite) const
{
	// On vérifie que le nom du fichier vgi existe
	if( mRawFileName == NULL )
		return false;
	// On voit si le fichier existe et si overwrite vaut true
	if( QFile::exists(*mRawFileName) && (!overwrite) )
		return false;
	// On vérifie que l'on peut écrire dans le fichier
	QFileInfo rawInfo(*mRawFileName);
	if( rawInfo.exists() && !rawInfo.isWritable() )
		return false;

	// Ouverture du fichier
	QFile rawFile(*mRawFileName);
	if( ! rawFile.open(QIODevice::WriteOnly | QIODevice::Text) )
		return false;

	for(long k=0; k<mZSize; k++)
		for(long j=0; j<mYSize; j++)
			for(long i=0; i<mXSize; i++)
// 				for(short c=0; c<3; c++)
// 					if( rawFile.putch(data[i][j][k][c]) == -1 )
// 					{
// 						rawFile.close();
// 						return false;
// 					}
				if( rawFile.putch(data[i][j][k][0]) == -1 )
				{
					rawFile.close();
					return false;
				}
	rawFile.close();
	return true;
}

// Ecrit les données passées par le paramètre img dans le fichier .raw
// La fonction renvoie true si tout se passe bien ou false si une erreur
// d'écriture se produit. La taille de l'image en paramètre doit être
// supérieure ou égale à celle donnée par mXSize * mYSize * mZSize
bool RubicFiles::writeRawFile(RubicImage* img, bool overwrite) const
{
	// On vérifie que le nom du fichier vgi existe
	if( mRawFileName == NULL )
		return false;
	// On voit si le fichier existe et si overwrite vaut true
	if( QFile::exists(*mRawFileName) && (!overwrite) )
		return false;
	// On vérifie que l'on peut écrire dans le fichier
	QFileInfo rawInfo(*mRawFileName);
	if( rawInfo.exists() && !rawInfo.isWritable() )
		return false;

	// Ouverture du fichier
	QFile rawFile(*mRawFileName);
	if( ! rawFile.open(QIODevice::WriteOnly | QIODevice::Text) )
		return false;
	
	for(long k=0; k<mZSize; k++)
		for(long j=0; j<mYSize; j++)
			for(long i=0; i<mXSize; i++)
// 				for(short c=0; c<3; c++)
// 					if( rawFile.putch(img->getData(i,j,k)) == -1 )
// 					{
// 						rawFile.close();
// 						return false;
// 					}
				if( rawFile.putch(img->getData(i,j,k)) == -1 )
				{
					rawFile.close();
					return false;
				}

	rawFile.close();
	return true;
}

void RubicFiles::delRawFile(unsigned char ****data)
{
	for(long i=0; i<mXSize; i++)
	{
		for(long j=0; j<mYSize; j++)
		{
			for(long k=0; k<mZSize; k++)
				if(data[i][j][k] != NULL)
					delete [] data[i][j][k];
			delete [] data[i][j];
		}
		delete [] data[i];
	}
}
