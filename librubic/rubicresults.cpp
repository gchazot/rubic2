/** \file rubicresults.cpp
  * \brief Contient la définition des fonctions membres de la classe RiubicResults
  */
  
#include "rubicresults.h"
#include "rubicresults_point.h"
#include "constants.h"
#include "dvect24.h"

#include <iostream>
using namespace std;

#include <qfile.h>
#include <qstring.h>
#include <qfileinfo.h>
#include <qdir.h>
#include <q3textstream.h>
#include <qdatetime.h>

// Constructeur avec dimensions séparées
RubicResults::RubicResults(const long xSize, const long ySize, const long zSize)
{
	mSize = LVect(xSize, ySize, zSize);
	mIsValid = true;

	mData = new RubicResults_Point**[xSize];
	if(mData == NULL)
	{
		mSize = LVect(0,0,0);
		mIsValid = false;
		return;
	}
	
	for(long i=0; i<xSize; i++)
	{
		mData[i] = new RubicResults_Point*[ySize];
		if(mData[i] == NULL)
		{
			mSize = LVect(0,0,0);
			mIsValid = false;
			return;
		}
		for(long j=0; j<ySize; j++)
		{
			mData[i][j] = new RubicResults_Point[zSize];
			if(mData[i][j] == NULL)
			{
				mSize = LVect(0,0,0);
				mIsValid = false;
				return;
			}
		}
	}
}

// Constructeur avec dimensions en LVect
RubicResults::RubicResults(const LVect& size)
{
	mSize = size;
	mIsValid = true;
	
	long xSize = size.getX();
	long ySize = size.getY();
	long zSize = size.getZ();

	mData = new RubicResults_Point**[xSize];
	if(mData == NULL)
	{
		mSize = LVect(0,0,0);
		mIsValid = false;
		return;
	}
	
	for(long i=0; i<xSize; i++)
	{
		mData[i] = new RubicResults_Point*[ySize];
		if(mData[i] == NULL)
		{
			mSize = LVect(0,0,0);
			mIsValid = false;
			return;
		}
		for(long j=0; j<ySize; j++)
		{
			mData[i][j] = new RubicResults_Point[zSize];
			if(mData[i][j] == NULL)
			{
				mSize = LVect(0,0,0);
				mIsValid = false;
				return;
			}
		}
	}
}

// Destructeur
RubicResults::~RubicResults()
{
	long xSize = mSize.getX();
	long ySize = mSize.getY();
	
	if(mData != NULL)
	{
		for(long i=0; i<xSize; i++)
		{
			if(mData[i] != NULL)
			{
				for(long j=0; j<ySize; j++)
				{
					if(mData[i][j] != NULL)
					{
						delete [] mData[i][j];
					}
				}
				delete [] mData[i];
			}
		}
		delete [] mData;
	}
	
	mIsValid = false;
}

// Opérateur d'accès aux données ave 3 entiers
RubicResults_Point** RubicResults::operator[](const long index)
{
	if(mIsValid)
		return mData[index];
	else
		return NULL;
}

// Opérateur d'accès aux données avec un LVect
RubicResults_Point* RubicResults::operator[](const LVect& index)
{
	if(mIsValid)
		return &(mData[index.getX()][index.getY()][index.getZ()]);
	else
		return NULL;
}

// Pour récupérer la taille de l'image
LVect RubicResults::getSize(void) const
{
	if(mIsValid)
		return mSize;
	else
		return LVect(0,0,0);
}

// Renvoie true ssi l'objet est utilisable
bool RubicResults::isValid(void) const
{
	return mIsValid;
}

// Ecrit les données dans un fichier ASCII
bool RubicResults::writeToASCII(const QString& fileName) const
{
	// Les informations sur le fichier
	QFileInfo info(fileName);
	// Le fichier lui-même
	QFile fic(fileName);
	// Gestionnaire de flux vers le fichier
	Q3TextStream stream(&fic);
	
	// On vérifie que l'objet contient des données valides
	if(! mIsValid)
	{
		cout << "Cet objet ne contient pas de données valides" << endl;
		return false;
	}
	// Si le fichier existe
	if( info.exists() )
	{
		// On vérifie que c'est bien un fichier
		if(! info.isFile() )
		{
			cout << fileName.toStdString() << " n'est pas un fichier" << endl;
			return false;
		}
		// On vérifie que l'on peut écrire dedans
		if(! info.isWritable() )
		{
			cout << "Impossible d'écrire dans " << fileName.toStdString() << endl;
			return false;
		}
	}
	
	// Ouverture du fichier en écriture avec éffacement du contenu
	if(! fic.open(QIODevice::WriteOnly | QIODevice::Truncate) )
	{
		cout << "Impossible d'ouvrir en écriture le fichier " << fileName.toStdString() << endl;
		return false;
	}
	
	// On peut commencer l'écriture
	
	// Ecriture de l'en-tête
	stream << " Fichier de sortie des résultats de calcul rubic." << endl;
	stream << " Début de l'écriture : " << QDateTime::currentDateTime().toString() << endl;
	stream << endl;
	stream << "Format du fichier:" << endl;
	stream << "\ti, j, k         -> coordonnées du voxern considéré." << endl;
	stream << "\tcorrel          -> coeff. de corrélation du voxern considéré." << endl;
	stream << "\tau, bu, ..., hw -> résultats du calcul du champ de déformation." << endl;
	stream << "\t                   Les valeurs \"XXX\" signifient qu'il s'est " << endl;
	stream << "\t                   produit une erreur dans le calcul du voxern." << endl;
	stream << endl;
	stream << "===============================================================================" << endl;
	stream << endl;
	
	// Première ligne du tableau
	stream << "[";
	stream.width(LARG_FLOAT_AFF);
	stream << "i" << "|";
	stream.width(LARG_FLOAT_AFF);
	stream << "j" << "|";
	stream.width(LARG_FLOAT_AFF);
	stream << "k" << "|";
	stream.width(LARG_FLOAT_AFF);
	stream << "correl" << "|";
	for(char u='u'; u <= 'w'; u++)
		{
			stream.width(LARG_FLOAT_AFF);
			stream << QString(QChar('h')) + QString(QChar(u)) << "|";
		}
	for(char a='a'; a<='g'; a++)
		for(char u='u'; u <= 'w'; u++)
		{
			stream.width(LARG_FLOAT_AFF);
			stream << QString(QChar(a)) + u << "|";
		}
	
	stream << endl;
	
	// Ecriture des valeurs
	long xSize = mSize.getX();
	long ySize = mSize.getY();
	long zSize = mSize.getZ();
	
	for(long i = 0; i < xSize; i++)
		for(long j = 0; j < ySize; j++)
			for(long k = 0; k < zSize; k++)
			{
				// Une ligne qui sépare les lignes de données
				for(int p=0; p < ((LARG_FLOAT_AFF + 1) * 28)+1; p++)
					stream << "-";
				stream << endl;
				
				// La ligne de données
					// Coordonnées
				stream << "[";
				stream.width(LARG_FLOAT_AFF);
				stream << i << "|";
				stream.width(LARG_FLOAT_AFF);
				stream << j << "|";
				stream.width(LARG_FLOAT_AFF);
				stream << k << "|";
				stream.width(LARG_FLOAT_AFF);
				stream << mData[i][j][k].Correl << "|";
					// Données
				stream.precision(PREC_FLOAT_AFF); stream.width(LARG_FLOAT_AFF);
				stream << mData[i][j][k].U[0];
				
				for(short p=1; p<24; p++)
				{
					stream << "|";
					stream.precision(PREC_FLOAT_AFF); stream.width(LARG_FLOAT_AFF);
					stream << mData[i][j][k].U[p];
				}
				
				stream <<  "]" << endl;
			}
	
	// Dernière ligne du tableau
	for(int p=0; p < ((LARG_FLOAT_AFF + 1) * 28)+1; p++)
		stream << "=";
	stream << endl;
		
	// Fin de l'écriture des données
	stream << flush;
	
	// Fermeture du fichier
	fic.close();
	
	// Tout est OK
	return true;
}

// Ecrit les données dans un fichier INP pour Abaqus
bool RubicResults::writeToINP(const QString& fileName, const long Lcur) const
{
	// Les informations sur le fichier
	QFileInfo info(fileName);
	// Le fichier lui-même
	QFile fic(fileName);
	// Gestionnaire de flux vers le fichier
	Q3TextStream stream(&fic);
	
	// On vérifie que l'objet contient des données valides
	if(! mIsValid)
	{
		cout << "Cet objet ne contient pas de données valides" << endl;
		return false;
	}
	// Si le fichier existe
	if( info.exists() )
	{
		// On vérifie que c'est bien un fichier
		if(! info.isFile() )
		{
			cout << fileName.toStdString() << " n'est pas un fichier" << endl;
			return false;
		}
		// On verifie que l'on peut ecrire dedans
		if(! info.isWritable() )
		{
			cout << "Impossible d'écrire dans " << fileName.toStdString() << endl;
			return false;
		}
		
	}
	
	// Ouverture du fichier en écriture avec éffacement du contenu
	if(! fic.open(QIODevice::WriteOnly | QIODevice::Truncate) )
	{
		cout << "Impossible d'ouvrir en écriture le fichier " << fileName.toStdString() << endl;
		return false;
	}
	
	// On peut commencer l'écriture	
	
	// En-tête
	stream << "*HEADING" << endl;
	stream << "*PREPRINT,ECHO=NO,HISTORY=NO,MODEL=NO" << endl;
	stream << "Rubic2  - Laboratoire de Mecanique des Contacts et des Solides - INSA de Lyon" << endl;
	
	
	long xSize = mSize.getX();
	long ySize = mSize.getY();
	long zSize = mSize.getZ();
	
	// Sortie de la matrice des noeuds
	stream << "*NODE, SYSTEM=R" << endl;
	LVect tmpCoord;
	long noeud = 1;
	for(long k =0; k <= zSize; k++)
		for(long j =0; j <= ySize; j++)
			for(long i =0; i <= xSize; i++)
			{
				stream << noeud << ",";
				
				tmpCoord = Lcur * LVect(i,j,k);
					
				stream << tmpCoord.getX() << ",";
				stream << tmpCoord.getY() << ",";
				stream << tmpCoord.getZ() << endl;
				
				noeud++;
			}
	
	// Matrice de connection (mailles)
	stream << "*ELEMENT,TYPE=C3D8,ELSET=MAIN" << endl;
	long maille = 1;
	for(long k =0; k < zSize; k++)
		for(long j =0; j < ySize; j++)
			for(long i =0; i < xSize; i++)
			{
				// On enlève les mailles non calculées
				if( mData[i][j][k].Correl < 0.0)
					continue;
				stream << maille << ", ";
				stream << ( ((ySize+1)*(xSize+1))* k    + (xSize+1)* j    + i + 1) << ", ";
				stream << ( ((ySize+1)*(xSize+1))* k    + (xSize+1)* j    + i + 2) << ", ";
				stream << ( ((ySize+1)*(xSize+1))* k    + (xSize+1)*(j+1) + i + 2) << ", ";
				stream << ( ((ySize+1)*(xSize+1))* k    + (xSize+1)*(j+1) + i + 1) << ", ";
				stream << ( ((ySize+1)*(xSize+1))*(k+1) + (xSize+1)*j     + i + 1) << ", ";
				stream << ( ((ySize+1)*(xSize+1))*(k+1) + (xSize+1)*j     + i + 2) << ", ";
				stream << ( ((ySize+1)*(xSize+1))*(k+1) + (xSize+1)*(j+1) + i + 2) << ", ";
				stream << ( ((ySize+1)*(xSize+1))*(k+1) + (xSize+1)*(j+1) + i + 1) << endl;
				
				maille++;
			}
	
	// Définition du calcul
	stream << "*SOLID SECTION,ELSET=MAIN,MATERIAL=M0000001" << endl;
	stream << "1.000E-03 ,   3" << endl;
	stream << "*MATERIAL,NAME=M0000001" << endl;
	stream << "*ELASTIC,TYPE=ISOTROPIC" << endl;
	stream << "2.0E+1 , 2.900E-01" << endl;
	stream << "*RESTART,WRITE" << endl;
	stream << "*STEP,NLGEOM" << endl;
	stream << "*STATIC" << endl;
	
	// Conditions aux limites avec les déplacements aux noeuds
	stream << "*BOUNDARY,OP=NEW" << endl;
	
	DVect24 u;
	int cntval;
	double Udef, Vdef, Wdef;
	short M, N, P;
	noeud = 1;
	for(long k =0; k <= zSize; k++)
		for(long j =0; j <= ySize; j++)
			for(long i =0; i <= xSize; i++)
			{
				// Valeur temporaire des déplacements
				Udef = Vdef = Wdef = 0.0;
				// Compteur pour faire la moyenne
				cntval = 0;
				// Moyennage du champ de déplacement au noeud
				for(short m=-1; m < 1; m++)
					for(short n=-1; n < 1; n++)
						for(short p=-1; p < 1; p++)
						{
							// On vérifie que l'on reste dans l'image
							if( (m+i >= xSize) || (n+j >= ySize) || (p+k >= zSize) )
								continue;
							if( (m+i < 0) || (n+j < 0) || (p+k < 0) )
								continue;
							
							// On ne tient compte du point que s'il est bon
							if(mData[m+i][n+j][p+k].Etat < 3)
							{
								u = mData[m+i][n+j][p+k].U;
								
								M = - m;
								N = - n;
								P = - p;
								
								Udef += u[0] + u[3]*M + u[6]*N + u[9]*P + u[12]*M*N + u[15]*M*P + u[18]*N*P + u[21]*M*N*P;

								Vdef += u[1] + u[4]*M + u[7]*N + u[10]*P + u[13]*(M*N) + u[16]*(M*P) + u[19]*(N*P) + u[22]*(M*N*P);
				
								Wdef += u[2] + u[5]*M + u[8]*N + u[11]*P + u[14]*(M*N) + u[17]*(M*P) + u[20]*(N*P) + u[23]*(M*N*P);
								
								cntval++;
							}
						}
				// On vérifie qu'on a au moins une valeur pour la moyenne
				if(cntval > 0)
				{
					Udef /= cntval;
					Vdef /= cntval;
					Wdef /= cntval;
					
					stream << noeud << ",1,1," << Udef << endl;
					stream << noeud << ",2,2," << Vdef << endl;
					stream << noeud << ",3,3," << Wdef << endl;
				}
				else
					stream << "**" << noeud << " : ERREUR !!!" << endl;
					
				noeud++;
			}

	// Fin des déplacements
	stream << "*END STEP" << endl;
	
	// Fermeture du fichier
	fic.close();
	
	return true;
}
