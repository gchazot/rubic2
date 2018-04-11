#include "imgshopwin.h"

#include "rubicfiles.h"

#include <q3filedialog.h>
#include <qfile.h>
#include <qlineedit.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <q3buttongroup.h>
#include <qspinbox.h>

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
using namespace std;

// Constructeur par défaut
ImgShopWin::ImgShopWin(QWidget * parent, const char * name, bool modal, Qt::WFlags f):
	ImgShopWinUI(parent, name, modal, f)
{
	// Bouton de choix du nom de l'image
	connect( fileButton, SIGNAL(clicked()), this, SLOT(selectFileName()) );
	// Bouton pour quitter l'application
	connect( quitButton, SIGNAL(clicked()), this, SLOT(close()) );
	// Bouton pour lancer la création de l'image
	connect( createButton, SIGNAL(clicked()), this, SLOT(createImage()) );
}

// Destructeur
ImgShopWin::~ImgShopWin()
{
}

// Fonction qui crée une image vide (pleine de 0) dans le fichier donné
// en paramètre.
void ImgShopWin::createBlackFile(unsigned char ****data)
{
	long xSize = xSizeBox->value();
	long ySize = ySizeBox->value();
	long zSize = zSizeBox->value();

	// Ecriture des données dans le tableau
	for(long i = 0; i < xSize; i++)
		for(long j = 0; j < ySize; j++)
			for(long k = 0; k < zSize; k++)
				for(short c = 0; c < 3; c++)
					data[i][j][k][c] = 0;
	return;
}

// Fonction qui crée une image pleine (pleine de 1) dans le fichier
// donné en paramètre.
void ImgShopWin::createWhiteFile(unsigned char ****data)
{
	long xSize = xSizeBox->value();
	long ySize = ySizeBox->value();
	long zSize = zSizeBox->value();

	// Ecriture des données dans le tableau
	for(long i = 0; i < xSize; i++)
		for(long j = 0; j < ySize; j++)
			for(long k = 0; k < zSize; k++)
				for(short c = 0; c < 3; c++)
					data[i][j][k][c] = ~0u;
	return;
}

// Fonction qui crée une image aléatoire dans le fichier
// donné en paramètre.
void ImgShopWin::createRandomFile(unsigned char ****data)
{
	long xSize = xSizeBox->value();
	long ySize = ySizeBox->value();
	long zSize = zSizeBox->value();
	
	// initialisation du générateur de nombres aléatoires
	srand(time(0));
	unsigned char val;

	// Ecriture des données dans le tableau
	for(long i = 0; i < xSize; i++)
		for(long j = 0; j < ySize; j++)
			for(long k = 0; k < zSize; k++)
			{
				val = (unsigned char)((rand() % 150) + 50);
				for(short c = 0; c < 3; c++)
					data[i][j][k][c] = val;
			}
	return;
}

// Fonction qui crée une image grise dans le fichier donné en paramètre.
// Le niveau de gris est gonné en second paramètre
void ImgShopWin::createGreyFile(unsigned char ****data, char greyLevel)
{
	long xSize = xSizeBox->value();
	long ySize = ySizeBox->value();
	long zSize = zSizeBox->value();

	// Ecriture des données dans le tableau
	for(long i = 0; i < xSize; i++)
		for(long j = 0; j < ySize; j++)
			for(long k = 0; k < zSize; k++)
				for(short c = 0; c < 3; c++)
					data[i][j][k][c] = greyLevel;
	return;
}

// Fonction qui crée une image en forme de sinus : 
// sin(N*Pi*x/xmax)*sin(N*Pi*y/ymax)*sin(N*Pi*z/zmax)
void ImgShopWin::createSinusFile(unsigned char ****data, int N)
{
	long xSize = xSizeBox->value();
	long ySize = ySizeBox->value();
	long zSize = zSizeBox->value();
	
	double pi = atan(1.0)*4.0;
	double val;

	// Ecriture des données dans le tableau
	for(long i = 0; i < xSize; i++)
		for(long j = 0; j < ySize; j++)
			for(long k = 0; k < zSize; k++)
				for(short c = 0; c < 3; c++)
				{
					val =	  sin( N * pi * (double)i / (double)(xSize-1) )
						* sin( N * pi * (double)j / (double)(ySize-1) )
						* sin( N * pi * (double)k / (double)(zSize-1) );
					data[i][j][k][c] = (unsigned char)(127.0f * val + 128.0f);
				}
	return;
}

// Fonction qui crée une image à évolution linéaire : 
// 255 * (x/xmax) * (y/ymax) * (z/zmax)
void ImgShopWin::createLinearFile(unsigned char ****data)
{
	long xSize = xSizeBox->value();
	long ySize = ySizeBox->value();
	long zSize = zSizeBox->value();
	
	double val;

	// Ecriture des données dans le tableau
	for(long i = 0; i < xSize; i++)
		for(long j = 0; j < ySize; j++)
			for(long k = 0; k < zSize; k++)
				for(short c = 0; c < 3; c++)
				{
					val =	  (double)i / (double)(xSize-1)
						+ (double)j / (double)(ySize-1)
						+ (double)k / (double)(zSize-1);
					data[i][j][k][c] = (unsigned char)(255.0f * val / 3.0);
				}
	return;
}

// Appelé lorsque le bouton "Choisir fichier" est clické
void ImgShopWin::selectFileName(void)
{
	// Définition des format de fichier possibles
	QString types("Images Rubic (*.vgi);;"
		"Tous les fichiers (*)");
	// Affichage de la boite de dialogue d'enregistrement
	QString filename = Q3FileDialog::getSaveFileName(QString::null, types, this, "Save Dialog",
		"Choisir le nom de la nouvelle image...");

	// Si l'utilisateur a cliqué sur "Annuler", pas la peine d'aller plus loin
	if(filename.isNull())
		return;

	//On vérifie que le nom de fichier se termine bien par ".vgi"
	if( !filename.endsWith(".vgi", false) )
	{
		QMessageBox::information(this,"Extension du fichier",
			QString("Vous devez utiliser l'extension \".vgi\" pour enregistrer votre image. ") +
			"Le fichier .raw sera créé avec le même nom mais l'extension sera remplacée.",
			QMessageBox::Ok, QMessageBox::NoButton);
		return;
	}
	
	// On utilise la classe RubicFiles pour gérer les noms de fichier
	RubicFiles files;
	files.setVgiFileName(filename);
	if(! files.setRawFileNameFromVgiFileName() )
	{
		QMessageBox::information(this,"Extension du fichier",
			"Erreur lors de la création du nom de fichier .raw d'après le .vgi.",
			QMessageBox::Ok, QMessageBox::NoButton);
		return;
	}
	
	// On vérifie que le fichier n'existe pas déjà
	// et on affiche une confirmation s'il existe
	if( files.checkFilesOverwrite(this) )
		fileLine->setText(filename);
	else
		fileLine->clear();
		
	// Si le fichier vgi existe déjà, et que l'on peut y lire la taille
	// de l'image, on met à jour la taille dans la fenetre
	if( files.readSizeFromVgi() )
	{
		xSizeBox->setValue(files.getXSize());
		ySizeBox->setValue(files.getYSize());
		zSizeBox->setValue(files.getZSize());
	}
	
	return;
}

// Appelé lorsque le bouton "Créer image" est clické
void ImgShopWin::createImage(void)
{
	//Création de l'objet RubicFiles pour la gestion des noms de fichier
	RubicFiles files;
	files.setVgiFileName(fileLine->text());
	if(! files.setRawFileNameFromVgiFileName() )
	{
		QMessageBox::critical(this,"Erreur de nom de fichier",
			"Erreur lors de la création des noms de fichiers.",
			QMessageBox::Ok, QMessageBox::NoButton );
		return;
	}
	
	files.setSize( xSizeBox->value(), ySizeBox->value(), zSizeBox->value() );
	
	// Allocation de la mémoire pour le tableau;
	unsigned char ****tab = new unsigned char***[files.getXSize()];
	if(tab == NULL)
	{
		QMessageBox::critical(this,"Erreur d'allocation mémoire",
			"Impossible d'allouer la mémoire pour le tableau dans imgshopwin.cpp",
			QMessageBox::Ok, QMessageBox::NoButton );
		return;
	}
	for(long i=0; i<files.getXSize(); i++)
	{
		tab[i] = new unsigned char**[files.getYSize()];
		if(tab[i] == NULL)
		{
			QMessageBox::critical(this,"Erreur d'allocation mémoire",
			"Impossible d'allouer la mémoire pour le tableau dans imgshopwin.cpp",
			QMessageBox::Ok, QMessageBox::NoButton );
			return;
		}
		for(long j=0; j<files.getYSize(); j++)
		{
			tab[i][j] = new unsigned char*[files.getZSize()];
			if(tab[i][j] == NULL)
			{
				QMessageBox::critical(this,"Erreur d'allocation mémoire",
				"Impossible d'allouer la mémoire pour le tableau dans imgshopwin.cpp",
				QMessageBox::Ok, QMessageBox::NoButton );
				return;
			}
			for(long k=0; k<files.getZSize(); k++)
			{
				tab[i][j][k] = new unsigned char[3];
				if(tab[i][j][k] == NULL)
				{
					QMessageBox::critical(this,"Erreur d'allocation mémoire",
					"Impossible d'allouer la mémoire pour le tableau dans imgshopwin.cpp",
					QMessageBox::Ok, QMessageBox::NoButton );
					return;
				}
			}
		}
	}

	// Création de l'image
	switch(imgTypeGroup->selectedId())
	{
		case 0:
			cout << "Création d'une image noire" << endl;
			createBlackFile(tab);
			break;
		case 1:
			cout << "Création d'une image aléatoire" << endl;
			createRandomFile(tab);
			break;
		case 2:
			cout << "Création d'une image blanche" << endl;
			createWhiteFile(tab);
			break;
		case 3:
			cout << "Création d'une image grise : " << greyValueBox->value() << endl;
			createGreyFile(tab, (char)greyValueBox->value());
			break;
		case 4:
			cout << "Création d'une image \"sinus\" : " << sinusValueBox->value() << endl;
			createSinusFile(tab, sinusValueBox->value());
			break;
		case 5:
			cout << "Création d'une image \"lineaire\"" << endl;
			createLinearFile(tab);
			break;
				
		default:	
			QMessageBox::critical(this,"Erreur d'execution",
				"Erreur de gestion du ButtonGroup imgTypeGroup dans imgshopwin.cpp",
				QMessageBox::Ok, QMessageBox::NoButton );
			files.delRawFile(tab);
			return;
	}
	
	//Ecriture du tableau créé dans le fichier
	if(! files.writeRawFile((const unsigned char ****)tab, true) )
	{
		QMessageBox::critical(this,"Erreur d'écriture",
			"Erreur lors de l'écriture du fichier :\n" 
			+ files.getRawFileName(),
			QMessageBox::Ok, QMessageBox::NoButton );
		files.delRawFile(tab);
		return;
	}
	files.delRawFile(tab);
	
	// Ecriture du fichier vgi
	if(! files.writeVgiFile(true) )
	{
		QMessageBox::critical(this,"Erreur d'écriture",
			"Erreur lors de l'écriture du fichier :\n"
			+ files.getVgiFileName(),
			QMessageBox::Ok, QMessageBox::NoButton );
		return;
	}
	
	QMessageBox::information(this, "Image créée",
		"L'image a bien été créée dans les fichier vgi et raw.",
		QMessageBox::Ok, QMessageBox::NoButton );
	return;
}
