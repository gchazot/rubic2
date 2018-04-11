#include "defshopwin.h"
#include "rubicfiles.h"
#include "rubicimage.h"
#include "dvect.h"
#include "lvect.h"
#include "dmatrix.h"
#include "gldefbox.h"

#include <cmath>
#include <iostream>
//Added by qt3to4:
#include <Q3HBoxLayout>
using namespace std;

#include <qpushbutton.h>
#include <q3filedialog.h>
#include <qmessagebox.h>
#include <qfile.h>
#include <qlineedit.h>
#include <qvalidator.h>
#include <qlayout.h>
#include <q3buttongroup.h>
#include <qlabel.h>

// Constructeur par défaut
DefShopWin::DefShopWin(QWidget * parent, const char * name, bool modal, Qt::WFlags f):
	DefShopWinUI(parent, name, modal, f)
{
	// Bouton pour quitter l'application
	connect( quitButton, SIGNAL(clicked()), this, SLOT(close()) 	);
	// Bouton pour lancer la création de l'image
	connect( createButton, SIGNAL(clicked()), this, SLOT(createImage()) );
	
	// Le validateur pour les données de la matrice de déformation
	mDefValidator = new QDoubleValidator(-1.0, 1.0, 5, this);
	def11Line->setValidator(mDefValidator);
	def12Line->setValidator(mDefValidator);
	def13Line->setValidator(mDefValidator);
	def22Line->setValidator(mDefValidator);
	def23Line->setValidator(mDefValidator);
	def33Line->setValidator(mDefValidator);
	// Initialisation des données de la matrice
	def11Line->setText("0.0");
	def12Line->setText("0.0");
	def13Line->setText("0.0");
	def22Line->setText("0.0");
	def23Line->setText("0.0");
	def33Line->setText("0.0");
	def21Line->setText("0.0");
	def31Line->setText("0.0");
	def32Line->setText("0.0");
	
	// Le validateur pour les angles de rotation
	mRotValidator = new QDoubleValidator(-180.0, 180.0, 5, this);
	rot1Line->setValidator(mRotValidator);
	rot2Line->setValidator(mRotValidator);
	rot3Line->setValidator(mRotValidator);
	// Initialisation des angles de rotation
	rot1Line->setText("0.0");
	rot2Line->setText("0.0");
	rot3Line->setText("0.0");
	
	// Le validateur pour les angles de rotation
	mTransValidator = new QDoubleValidator(-1000.0, 1000.0, 5, this);
	trans1Line->setValidator(mTransValidator);
	trans2Line->setValidator(mTransValidator);
	trans3Line->setValidator(mTransValidator);
	// Initialisation des angles de rotation
	trans1Line->setText("0.0");
	trans2Line->setText("0.0");
	trans3Line->setText("0.0");
	
	// Bouton de choix du nom de l'image initiale
	connect( initImgButton, SIGNAL(clicked()), this, SLOT(selectInitFileName()) );
	// Bouton de choix du nom de l'image déformée
	connect( defImgButton, SIGNAL(clicked()), this, SLOT(selectDefFileName()) );
	
	if ( !QGLFormat::hasOpenGL() ) {
		qWarning( "This system has no OpenGL support. Exiting." );
		mDefBox = NULL;
		return;
    	}
    
	// Affichage de la déformée
	mDefBox = new GLDefBox(glViewFrame, "defbox");
	Q3HBoxLayout* flayout = new Q3HBoxLayout( glViewFrame);
	flayout->addWidget( mDefBox, 1 );
	
	// Taille d'image bidon
	mDefBox->setObjSize( LVect(10,10,10) );
	
	// Pour mettre à jour l'aperçu de la déformée sur chaque changement de paramètre
	connect( def11Line, SIGNAL(textChanged(const QString&)), this, SLOT(updateDefBox(void)) );
	connect( def12Line, SIGNAL(textChanged(const QString&)), this, SLOT(updateDefBox(void)) );
	connect( def13Line, SIGNAL(textChanged(const QString&)), this, SLOT(updateDefBox(void)) );
	connect( def22Line, SIGNAL(textChanged(const QString&)), this, SLOT(updateDefBox(void)) );
	connect( def23Line, SIGNAL(textChanged(const QString&)), this, SLOT(updateDefBox(void)) );
	connect( def33Line, SIGNAL(textChanged(const QString&)), this, SLOT(updateDefBox(void)) );
	connect( rot1Line, SIGNAL(textChanged(const QString&)), this, SLOT(updateDefBox(void)) );
	connect( rot2Line, SIGNAL(textChanged(const QString&)), this, SLOT(updateDefBox(void)) );
	connect( rot3Line, SIGNAL(textChanged(const QString&)), this, SLOT(updateDefBox(void)) );
	connect( trans1Line, SIGNAL(textChanged(const QString&)), this, SLOT(updateDefBox(void)) );
	connect( trans2Line, SIGNAL(textChanged(const QString&)), this, SLOT(updateDefBox(void)) );
	connect( trans3Line, SIGNAL(textChanged(const QString&)), this, SLOT(updateDefBox(void)) );
}

// Destructeur
DefShopWin::~DefShopWin()
{
}

// Charge dans un tableau l'image initiale. Alloue la mémoire du
// tableau. Renvoie true en cas de succès, false sinon
bool DefShopWin::loadInitImage(RubicImage **img)
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
bool DefShopWin::saveDefImage(RubicImage *img)
{
	// On utilise la classe RubicFiles pour gérer les noms de fichier
	RubicFiles files;
	files.setVgiFileName(defImgLine->text());
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
bool DefShopWin::createBlackImg(RubicImage **img, LVect& size)
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
bool DefShopWin::compute(RubicImage *initiale, RubicImage *deformee)
{
	// Taille de l'image
	LVect size = deformee->getSize();

	// Le tenseur des déformations de Green-Lagrange
	DMatrix E;
	E[0][0] = def11Line->text().toDouble();
	E[0][1] = E[1][0] = def12Line->text().toDouble();
	E[0][2] = E[2][0] = def13Line->text().toDouble();
	E[1][1] = def22Line->text().toDouble();
	E[1][2] = E[2][1] = def23Line->text().toDouble();
	E[2][2] = def33Line->text().toDouble();
	
	// DEBUG
	cout << "* Matrice E:" << endl;
	cout << E.toString().toStdString() << endl;

	// La matrice de rotation
	DMatrix R;
	R = DMatrix::Rotation(  rot1Line->text().toDouble() * M_PI / 180.0,
	                        rot2Line->text().toDouble() * M_PI / 180.0,
	                        rot3Line->text().toDouble() * M_PI / 180.0 );
	
	// DEBUG
	cout << "* Matrice R:" << endl;
	cout << R.toString().toStdString() << endl;
	
	// Le vecteur de translation
	DVect Uadd(     trans1Line->text().toDouble(),
	                trans2Line->text().toDouble(),
	                trans3Line->text().toDouble() );
	
	// DEBUG
	cout << "* Vecteur Uadd:" << endl;
	cout << Uadd.toString().toStdString() << endl;

	
	// La matrice identité
	DMatrix I = DMatrix::Identite();
	// DEBUG
// 	cout << "* Matrice I:" << endl;
// 	cout << I.toString().toStdString() << endl;	
	
	// La matrice C dans le repère espace
	DMatrix C1 = 2.0 * E + I;
	// DEBUG
// 	cout << "* Matrice C1:" << endl;
// 	cout << C1.toString().toStdString() << endl;
	
	// La matrice C dans sa base propre (diagonale)
	DMatrix CI;
	// Les valeurs propres de C
	DVect Cval;
	// La matrice des vecteurs propres de C correspondante
	DMatrix Cvec;

	
	// Diagonalisation
	if(! C1.diagonalise(CI, Cvec) )
		return false;
	// DEBUG
// 	cout << "* Matrice Cvec:" << endl;
// 	cout << Cvec.toString().toStdString() << endl;
// 	cout << "* Matrice CI:" << endl;
// 	cout << CI.toString().toStdString() << endl;
	
	// La matrice U dans la base propre de C
	DMatrix UI;
	for(int i=0; i<3; i++)
		UI[i][i] = sqrt(CI[i][i]);
	// DEBUG
// 	cout << "* Matrice UI:" << endl;
// 	cout << UI.toString().toStdString() << endl;
	
	
	// La matrice F = U dans la base espace
	DMatrix U = R * Cvec.transpose() * UI * Cvec;
	// DEBUG
// 	cout << "* Matrice U:" << endl;
// 	cout << U.toString().toStdString() << endl;
	
	DMatrix F = R*U;
	// DEBUG
// 	cout << "* Matrice F:" << endl;
// 	cout << F.toString().toStdString() << endl;
	
	// L'inverse de F
	DMatrix Finv = F.inverse();
	// DEBUG
	cout << "* Matrice Finv:" << endl;
	cout << Finv.toString().toStdString() << endl;
	
	if(interpolGroup->selectedId() == 0)
		cout << "Interpolation trilinéaire." << endl;
	else
		cout << "Interpolation par splines cubiques." << endl;
	
	DVect inipos;
	LVect curpos(0, 0, 0);
	char nivgris;
	
	long xSize = size.getX();
	long ySize = size.getY();
	long zSize = size.getZ();
	
	// Déformation de l'image
	// Pour chaque point de la déformée
	for(curpos[0] = 0; curpos[0] < xSize; curpos[0]++)
		for(curpos[1] = 0; curpos[1] < ySize; curpos[1]++)
			for(curpos[2] = 0; curpos[2] < zSize; curpos[2]++)
			{
				// On caclcule le point correspondant dans l'ilmage initiale
				inipos = (DVect)curpos - Uadd;
				inipos = Finv * inipos;
				
				// On calcule la valeur interpolée correspondante
				if(interpolGroup->selectedId() == 0)
 					nivgris = arrondi(initiale->interpoleLineaire(inipos));
 				else
					nivgris = arrondi(initiale->interpoleCubique(inipos));
				
				// On enregistre la valeur dans la déformée
				deformee->setData(nivgris, curpos);
			}
	return true;
}

// Appelé lorsque le bouton "Choisir fichier" est clické
// pour l'image initiale	
void DefShopWin::selectInitFileName(void)
{
	// Définition des format de fichier possibles
	QString types("Images Rubic (*.vgi);;"
		"Tous les fichiers (*)");
	// Affichage de la boite de dialogue d'enregistrement
	QString filename = Q3FileDialog::getOpenFileName(QString::null, types, this, "Save Dialog",
		"Choisir le nom de la nouvelle image...");

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
		if(mDefBox != NULL)
		{
			if( files.readSizeFromVgi() )
			{
				mDefBox->setObjSize( LVect( files.getXSize(), files.getYSize(), files.getZSize() ) );
				dimLabel->setText( QString::number(files.getXSize()) + "x"
						 + QString::number(files.getYSize()) + "x"
						 + QString::number(files.getZSize()) );
			}
			else
				mDefBox->setObjSize( LVect(10,10,10) );
		}
	}
	else
	{
		// Pas Ok, on affiche un message d'erreur
		QMessageBox::warning(this,"Accès aux fichiers",
			"Impossible de lire le fichier vgi ou raw.",
			QMessageBox::Ok, QMessageBox::NoButton);
		defImgLine->clear();
		// Et on met une taille d'image bidon
		if(mDefBox != NULL)
			mDefBox->setObjSize( LVect(10,10,10) );
	}
}

// Appelé lorsque le bouton "Choisir fichier" est clické
// pour l'image déformée	
void DefShopWin::selectDefFileName(void)
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
		defImgLine->setText(filename);
	else
		defImgLine->clear();
	
	return;
}

// Appelé lorsque le vouton "Créer image" est clické
void DefShopWin::createImage(void)
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
		"Le fichier de la déformée a été correctement créé",
		QMessageBox::Ok, QMessageBox::NoButton);
	
	
	return;
}

// Met à jour la représentation de la déformée
void DefShopWin::updateDefBox(void)
{
	// Si il n'y a pas d'opengl, on ne fait rien
	if(mDefBox == NULL)
		return;
	
	// Le tenseur des déformations de Green-Lagrange
	DMatrix E;
	E[0][0] = def11Line->text().toDouble();
	E[0][1] = E[1][0] = def12Line->text().toDouble();
	E[0][2] = E[2][0] = def13Line->text().toDouble();
	E[1][1] = def22Line->text().toDouble();
	E[1][2] = E[2][1] = def23Line->text().toDouble();
	E[2][2] = def33Line->text().toDouble();

	// La matrice de rotation
	DMatrix R;
	R = DMatrix::Rotation(  rot1Line->text().toDouble() * M_PI / 180.0,
				rot2Line->text().toDouble() * M_PI / 180.0,
				rot3Line->text().toDouble() * M_PI / 180.0 );
	
	// Le vecteur de translation
	DVect Uadd(     trans1Line->text().toDouble(),
			trans2Line->text().toDouble(),
			trans3Line->text().toDouble() );

	// La matrice identité
	DMatrix I = DMatrix::Identite();
	
	// La matrice C dans le repère espace
	DMatrix C1 = 2.0 * E + I;
	
	// La matrice C dans sa base propre (diagonale)
	DMatrix CI;
	// Les valeurs propres de C
	DVect Cval;
	// La matrice des vecteurs propres de C correspondante
	DMatrix Cvec;

	// Diagonalisation
	if(! C1.diagonalise(CI, Cvec) )
		return;
	
	// La matrice U dans la base propre de C
	DMatrix UI;
	for(int i=0; i<3; i++)
		UI[i][i] = sqrt(CI[i][i]);

	// La matrice U dans la base espace
	DMatrix U = Cvec.transpose() * UI * Cvec;
	// La matrice F = R*U dans la base espace
	DMatrix F = R*U;
	
	// On met à jour l'aperçu de la déformation
	mDefBox->setDefMatrix(F);
	mDefBox->setTransVect(Uadd);
}
