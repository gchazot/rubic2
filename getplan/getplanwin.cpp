#include "getplanwin.h"
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
#include <qimage.h>
#include <qcolor.h>

// Constructeur par défaut
GetPlanWin::GetPlanWin(QWidget * parent, const char * name, bool modal, Qt::WFlags f):
	GetPlanWinUI(parent, name, modal, f)
{
	// Bouton pour quitter l'application
	connect( quitButton, SIGNAL(clicked()), this, SLOT(close()) 	);
	// Bouton pour lancer la création de l'image
	connect( createButton, SIGNAL(clicked()), this, SLOT(createImage()) );
	
	// Bouton de choix du nom de l'image initiale
	connect( initImgButton, SIGNAL(clicked()), this, SLOT(selectInitFileName()) );
	// Bouton de choix du nom de l'image déformée
	connect( defImgButton, SIGNAL(clicked()), this, SLOT(selectDefFileName()) );
	
	// Changement du plan désiré dans le combobox
	connect( planCombo, SIGNAL(activated(int)), this, SLOT(updateSpinBox(int)) );
	
	dimLabel->setText( "Inconnue" );
}

// Destructeur
GetPlanWin::~GetPlanWin()
{
}

// Charge dans un tableau l'image initiale. Alloue la mémoire du
// tableau. Renvoie true en cas de succès, false sinon
bool GetPlanWin::loadInitImage(RubicImage **img)
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
bool GetPlanWin::saveDefImage(QImage *img)
{
	// Récupération du nom de fichier
	QString filename = targetImgLine->text();
	
	if(filename.isNull())
		return false;
		
	// On récupère l'extension dans le nom de fichier
	QString extension= filename.right(filename.length()-filename.findRev('.')-1);
	extension = extension.lower(); // On passe tout en minuscules
	
	// Détermination/Vérification du format de fichier
	QString format = QString::null;
	if(extension == "bmp")
		format = "BMP";
	else
	{
		QMessageBox::warning(this, "Format inconnu",
			"Format de fichier inconnu: impossible d'enregistrer l'image",
			QMessageBox::Ok, QMessageBox::NoButton);
		return false;
	}
	
	// Maintenant on peut tenter d'enregistrer le fichier:
	if( ! img->save(filename, format, 100) )
	{
		QMessageBox::warning(this, "Erreur d'enregistrement",
			"Il s'est produit une erreur durant l'enregistrement",
			QMessageBox::Ok, QMessageBox::NoButton);
		return false;
	}
	
	// Enregistrement OK
	return true;
}

// Calcule la déformée de l'image initiale dans deformee d'après la
// matrice de déformation donnée dans l'interface.
// Renvoie true en cas de succès, false sinon
bool GetPlanWin::compute(RubicImage *initiale, QImage *plan, LVect *size)
{
	DVect inipos;
	LVect curpos(0, 0, 0);
	uint nivgris;
	
	long xSize = size->getX();
	long ySize = size->getY();
	long zSize = size->getZ();
	
	LVect offset(0,0,0);
	LVect tmpvec;
	
	uint axis = planCombo->currentItem();
	switch(axis)
	{
	case 0:
		offset.setX(planBox->value());
		break;
	case 1:
		offset.setY(planBox->value());
		break;
	case 2:
		offset.setZ(planBox->value());
		break;
	default:
		return false;
	}
	
	// Déformation de l'image
	// Pour chaque point de la déformée
	for(curpos[0] = 0; curpos[0] < xSize; curpos[0]++)
		for(curpos[1] = 0; curpos[1] < ySize; curpos[1]++)
			for(curpos[2] = 0; curpos[2] < zSize; curpos[2]++)
			{
				tmpvec = curpos + offset;
				nivgris = initiale->getData(tmpvec);
				
				switch(axis)
				{
				case 0:
					plan->setPixel( curpos.getY(), curpos.getZ(), nivgris );
					break;
				case 1:
					plan->setPixel( curpos.getX(), curpos.getZ(), nivgris );
					break;
				case 2:
					plan->setPixel( curpos.getX(), curpos.getY(), nivgris );
					break;
				default:
					return false;
				}
			}
	return true;
}

// Appelé lorsque le bouton "Choisir fichier" est clické
// pour l'image initiale	
void GetPlanWin::selectInitFileName(void)
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
	
	updateSpinBox( planCombo->currentItem() );
}

// Appelé lorsque le bouton "Choisir fichier" est clické
// pour l'image déformée	
void GetPlanWin::selectDefFileName(void)
{
	// Définition des format de fichier possibles
	QString types("Images Bitmap (*.bmp);;"
		"Tous les fichiers (*)");
	// Affichage de la boite de dialogue d'enregistrement
	QString filename = Q3FileDialog::getSaveFileName(QString::null, types, this, "Save Dialog",
		"Enregistrer l'image...");
	
	targetImgLine->clear();

	// Si l'utilisateur a cliqué sur "Annuler", pas la peine d'aller plus loin
	if(filename.isNull())
		return;
	
	// On vérifie que le fichier n'existe pas déjà
	// et on affiche une confirmation s'il existe
	if( QFile::exists(filename) )
		if( QMessageBox::warning(this, "Ecrasement de fichier",
			"Le fichier choisi existe déjà, il sera écrasé!\nEtes-vous certain de vouloir le remplacer?",
			QMessageBox::Yes | QMessageBox::Default,
			QMessageBox::No | QMessageBox::Escape) != QMessageBox::Yes )
			return;

	//On vérifie que le nom de fichier se termine bien par ".bmp"
	if( !filename.endsWith(".bmp", false) )
	{
		QMessageBox::information(this,"Extension du fichier",
			"Vous devez utiliser l'extension \".bmp\" pour enregistrer votre image. ",
			QMessageBox::Ok, QMessageBox::NoButton);
		return;
	}
	
	
	// On vérifie que l'on peut écrire dans le fichier.
	QFileInfo imgInfo(filename);
	if( imgInfo.exists() && !imgInfo.isWritable() )
	{
		QMessageBox::warning(this, "Ecriture dans le fichier",
			"Impossible d'écrire dans le fichier (droits d'accès).",
			QMessageBox::Ok, QMessageBox::NoButton);
		return;
	}
	
	// Tout est OK
	targetImgLine->setText(filename);
	
	return;
}

// Appelé lorsque le vouton "Créer image" est clické
void GetPlanWin::createImage(void)
{
	RubicImage *initial = NULL;
	QImage *plan = NULL;
	LVect imgSize;
	
	if( ! loadInitImage(&initial) )
	{
		return;
	}
	
	imgSize = initial->getSize();
	switch(planCombo->currentItem())
	{
	case 0:
		imgSize.setX(1);
		plan = new QImage(imgSize.getY(), imgSize.getZ(), 8, 256);
		break;
	case 1:
		imgSize.setY(1);
		plan = new QImage(imgSize.getX(), imgSize.getZ(), 8, 256);
		break;
	case 2:
		imgSize.setZ(1);
		plan = new QImage(imgSize.getX(), imgSize.getY(), 8, 256);
		break;
	default:
		return;
	}
	
	if(plan == NULL)
		return;
	
	// Initialisation de la palette
	for(int i = 0; i < 256; i++)
	{
		plan->setColor(i, qRgb(i,i,i));
	}
	// Remplissage de noir
	plan->fill(0u);
	
	
	if( ! compute(initial, plan, &imgSize) )
	{
		return;
	}
	
	if( ! saveDefImage(plan) )
	{
		return;
	}
	
	if(initial != NULL)
		delete initial;
	if(plan != NULL)
		delete plan;
		
	QMessageBox::information(this,"Réussite",
		"Le fichier du plan a été correctement créé",
		QMessageBox::Ok, QMessageBox::NoButton);
	
	return;
}

void GetPlanWin::updateSpinBox(int selection)
{
	switch(selection)
	{
	case 0:
		planBox->setMaxValue(mObjSize.getX());
		break;
	case 1:
		planBox->setMaxValue(mObjSize.getY());
		break;
	case 2:
		planBox->setMaxValue(mObjSize.getZ());
		break;
	default:
		return;
	}
}
