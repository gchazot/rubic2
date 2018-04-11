#include "smoothimgwin.h"
#include "rubicfiles.h"
#include "rubicimage.h"
#include "dvect.h"
#include "lvect.h"
#include "dmatrix.h"

#include <cmath>
#include <iostream>
using namespace std;

#include <qpushbutton.h>
#include <q3filedialog.h>
#include <qmessagebox.h>
#include <qfile.h>
#include <qlineedit.h>
#include <q3buttongroup.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qspinbox.h>

// Constructeur par défaut
SmoothImgWin::SmoothImgWin(QWidget * parent, const char * name, bool modal, Qt::WFlags f):
	SmoothImgWinUI(parent, name, modal, f)
{
	// Bouton pour quitter l'application
	connect( quitButton, SIGNAL(clicked()), this, SLOT(close()) 	);
	// Bouton pour lancer la création de l'image
	connect( createButton, SIGNAL(clicked()), this, SLOT(createImage()) );
	
	// Bouton de choix du nom de l'image initiale
	connect( initImgButton, SIGNAL(clicked()), this, SLOT(selectInitFileName()) );
	// Bouton de choix du nom de l'image déformée
	connect( defImgButton, SIGNAL(clicked()), this, SLOT(selectDefFileName()) );
	
	dimLabel->setText( "Inconnue" );
}

// Destructeur
SmoothImgWin::~SmoothImgWin()
{
}

// Charge dans un tableau l'image initiale. Alloue la mémoire du
// tableau. Renvoie true en cas de succès, false sinon
bool SmoothImgWin::loadInitImage(RubicImage **img)
{
	// On utilise la classe RubicFiles pour gérer les noms de fichier
	RubicFiles files;
	files.setVgiFileName(initImgLine->text());
	if(! files.setRawFileNameFromVgiFileName() )
	{
		QMessageBox::information(this,"Extension du fichier",
			"Erreur lors de la création du nom de fichier .raw d'après le .vgi.",
			QMessageBox::Ok, QMessageBox::NoButton);
		return false;
	}
	
	// On vérifie que l'on peut lire dans les fichiers
	if( ! files.checkFilesRead() )
	{
		QMessageBox::warning(this,"Accès aux fichiers",
			"Impossible de lire le fichier vgi ou raw.",
			QMessageBox::Ok, QMessageBox::NoButton);
		return false;
	}
	
	// Lecture des dimensions de l'image
	if( ! files.readSizeFromVgi() )
	{
		QMessageBox::warning(this,"Lecture des dimensions",
			"Impossible de lire les dimensions de l'image dans le fichier vgi.",
			QMessageBox::Ok, QMessageBox::NoButton);
		return false;
	}
	
	// Allocation de la mémoire et lecture du tableau
	if( ! files.readRawFile(img) )
	{
		QMessageBox::critical(this,"Erreur de lecture",
			"Une erreur s'est produite lors de la lecture du fichier .raw.",
			QMessageBox::Ok, QMessageBox::NoButton);
		return false;
	}
	
	// C'est Ok
	return true;
}

// Sauvegarde l'image donnée en paramètre. Renvoie false en cas d'échec
bool SmoothImgWin::saveDefImage(RubicImage *img)
{
	// On utilise la classe RubicFiles pour gérer les noms de fichier
	RubicFiles files;
	files.setVgiFileName(targetImgLine->text());
	files.setXSize( img->getXSize() );
	files.setYSize( img->getYSize() );
	files.setZSize( img->getZSize() );
	
	if( ! files.setRawFileNameFromVgiFileName() )
	{
		QMessageBox::information(this,"Extension du fichier",
			"Erreur lors de la création du nom de fichier .raw d'après le .vgi.",
			QMessageBox::Ok, QMessageBox::NoButton);
		return false;
	}
	
	// On vérifie que l'on peut écrire dans les fichiers
	if( ! files.checkFilesWrite() )
	{
		QMessageBox::warning(this,"Accès aux fichiers",
			"Impossible d'écrire dans le fichier vgi ou raw.",
			QMessageBox::Ok, QMessageBox::NoButton);
		return false;
	}
	
	// Ecriture du tableau
	if( ! files.writeVgiFile(true) )
	{
		QMessageBox::critical(this,"Erreur d'écriture",
			"Une erreur s'est produite lors de l'écriture du fichier .vgi.",
			QMessageBox::Ok, QMessageBox::NoButton);
		return false;
	}
	
	if( ! files.writeRawFile(img, true) )
	{
		QMessageBox::critical(this,"Erreur d'écriture",
			"Une erreur s'est produite lors de l'écriture du fichier .raw.",
			QMessageBox::Ok, QMessageBox::NoButton);
		return false;
	}
	
	// C'est Ok
	return true;
}

// Alloue un tableau de la dimension de l'image initiale et le
// remplit de 0. Renvoie true si réussit et false sinon
bool SmoothImgWin::createBlackImg(RubicImage **img, LVect& size)
{
	
	// Allocation de la mémoire pour le tableau;
	*img = new RubicImage(size);
	
	long xSize = size.getX();
	long ySize = size.getY();
	long zSize = size.getZ();
	
	// Remplissage du tableau
	for(long i=0; i<xSize; i++)
		for(long j=0; j<ySize; j++)
			for(long k=0; k<zSize; k++)
				(*img)->setData(0, i, j, k);

	return true;
}

// Calcule la déformée de l'image initiale dans deformee d'après la
// matrice de déformation donnée dans l'interface.
// Renvoie true en cas de succès, false sinon
bool SmoothImgWin::compute(RubicImage *initiale, RubicImage *deformee)
{
	// Taille de l'image
	LVect size = deformee->getSize();

	DVect inipos;
	LVect curpos(0, 0, 0);
	LVect offset(0, 0, 0);
	LVect tmpvec;
	long nivgris;
	int count;
	int zone = sizeBox->value();
	zone = (zone - (zone % 2))/2;
	
	long xSize = size.getX();
	long ySize = size.getY();
	long zSize = size.getZ();
	
	// Déformation de l'image
	// Pour chaque point de la déformée
	for(curpos[0] = 0; curpos[0] < xSize; curpos[0]++)
		for(curpos[1] = 0; curpos[1] < ySize; curpos[1]++)
			for(curpos[2] = 0; curpos[2] < zSize; curpos[2]++)
			{
				nivgris = 0;
				count = 0;
				
				for(offset[0] = -zone; offset[0] < zone; offset[0]++)
					for(offset[1] = -zone; offset[1] < zone; offset[1]++)
						for(offset[2] = -zone; offset[2] < zone; offset[2]++)
						{
							if(offset.getNorme() > zone)
								continue;
							tmpvec = curpos + offset;
							if( (tmpvec.getMin() < 0) || (tmpvec.getX() >= xSize)
								|| (tmpvec.getY() >= ySize) || (tmpvec.getZ() >= zSize) )
								continue;
							nivgris += initiale->getData(tmpvec);
							count++;
						}
				if(count > 0)
					nivgris = nivgris / count;
				else
					nivgris = 0;
						
				
				// On enregistre la valeur dans la déformée
				deformee->setData(nivgris, curpos);
			}
	return true;
}

// Appelé lorsque le bouton "Choisir fichier" est clické
// pour l'image initiale	
void SmoothImgWin::selectInitFileName(void)
{
	// Définition des format de fichier possibles
	QString types("Images Rubic (*.vgi);;"
		"Tous les fichiers (*)");
	// Affichage de la boite de dialogue d'enregistrement
	QString filename = Q3FileDialog::getOpenFileName(QString::null, types, this, "Save Dialog",
		"Choisir le nom de la nouvelle image...");

	dimLabel->setText( "Inconnue" );
	mObjSize = LVect(-1,-1,-1);
	initImgLine->clear();
	
	// Si l'utilisateur a cliqué sur "Annuler", pas la peine d'aller plus loin
	if(filename.isNull())
		return;
	
	//On vérifie que le nom de fichier se termine bien par ".vgi"
	if( !filename.endsWith(".vgi", false) )
	{
		QMessageBox::information(this,"Extension du fichier",
			QString("Vous devez utiliser l'extension \".vgi\" pour enregistrer votre image. ") +
			"Le fichier .raw sera lu avec le même nom mais l'extension sera remplacée.",
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
	
	// On vérifie que les fichier existent et sont lisibles.
	if( files.checkFilesRead() )
	{
		// Ok, on garde le nom d'image
		initImgLine->setText(filename);
		// Et on récupère la taille de l'image
		if( files.readSizeFromVgi() )
		{
			dimLabel->setText( QString::number(files.getXSize()) + "x"
						+ QString::number(files.getYSize()) + "x"
						+ QString::number(files.getZSize()) );
			mObjSize = files.getSize();
		}
	}
	else
	{
		// Pas Ok, on affiche un message d'erreur
		QMessageBox::warning(this,"Accès aux fichiers",
			"Impossible de lire le fichier vgi ou raw.",
			QMessageBox::Ok, QMessageBox::NoButton);
	}
}

// Appelé lorsque le bouton "Choisir fichier" est clické
// pour l'image déformée	
void SmoothImgWin::selectDefFileName(void)
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
		targetImgLine->setText(filename);
	else
		targetImgLine->clear();
	
	return;
}

// Appelé lorsque le vouton "Créer image" est clické
void SmoothImgWin::createImage(void)
{
	RubicImage *initial = NULL;
	RubicImage *deforme = NULL;
	LVect imgSize;
	
	if( ! loadInitImage(&initial) )
	{
		return;
	}
	
	imgSize = initial->getSize();
	
	if( ! createBlackImg(&deforme, imgSize) )
	{
		return;
	}
	
	if( ! compute(initial, deforme) )
	{
		return;
	}
	
	if( ! saveDefImage(deforme) )
	{
		return;
	}
	
	if(initial != NULL)
		delete initial;
	if(deforme != NULL)
		delete deforme;
		
	QMessageBox::information(this,"Réussite",
		"Le fichier lissé a été correctement créé",
		QMessageBox::Ok, QMessageBox::NoButton);
	
	
	return;
}
