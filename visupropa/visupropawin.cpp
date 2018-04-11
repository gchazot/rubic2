#include "visupropawin.h"
#include "visupropagl.h"

#include "lvectuniquequeue.h"

#include <qtimer.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qslider.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qstring.h>
#include <qregexp.h>
#include <qmessagebox.h>
#include <q3textstream.h>
#include <q3filedialog.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qpixmap.h>
#include <qcolor.h>
#include <qpainter.h>
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <Q3ValueList>


#include <cmath>

#include <iostream>
using namespace std;

// Calcule l'intervalle du timer en fonction de la valeur du slider
int slider2timer(int slider)
{
	return (int)(200.0/logf((float)slider+1.0));
}

/// Constructeur
VisuPropaWin::VisuPropaWin( QWidget* parent, const char* name):VisuPropaWinUI(parent,name)
{
	// Création du timer pour la propagation temporelle
	playTimer = new QTimer(this, "timer");
	playTimer->stop();
	
	// intervale de 10 sec entre 2 évolutions
	interval = 10 * 1000;
	
	// Création de la visu OpenGL de la propagation
	visu = new VisuPropaGL(glFrame, "visuGL");
	
	// On place la visu dans un layout pour faire bien propre
	visuLayout = new Q3HBoxLayout(glFrame, 5, 0, "LayoutGL");
	visuLayout->addWidget(visu, 0, 0);
	
	// Initialisation de l'affichage des dimensions du résultat
	dimLabel->setText("Inconnu");
	
	// Connections de signaux
	connect( playTimer, SIGNAL(timeout()), this, SLOT(timerTick()) );
	connect( fileButton, SIGNAL(clicked()), this, SLOT(parcoursFichiers()) );
	connect( playSlider, SIGNAL(valueChanged(int)), this, SLOT(playSliderChanged(int)) );
	connect( playButton, SIGNAL(toggled(bool)), this, SLOT(playButtonToggled(bool)) );
	connect( timerSlider, SIGNAL(sliderMoved(int)), this, SLOT(timerSliderMoved(int)) );
	connect( correlBox, SIGNAL(toggled(bool)), this, SLOT(correlationClicked(bool)) );
	
	// Initialisation des listes de points et de coeffs. de corrélation
	vectList = NULL;
	correlList =NULL;
	
	// Dessin de l'échelle de couleurs
	QPixmap scalePixmap(20,400);
	scalePixmap.fill();
	QPainter painter(&scalePixmap);
	for(int j = 0; j < 400; j++)
	{
		painter.setPen(val2color((double)j / 399.0));
		painter.drawLine(0,j,20,j);
	}
	scaleLabel->setPixmap(scalePixmap);
}
	
	
/// Destructeur
VisuPropaWin::~VisuPropaWin()
{
	if(vectList != NULL)
		delete vectList;
	if(correlList != NULL)
		delete correlList;
}

/// Gestion du click sur play
void VisuPropaWin::playButtonToggled(bool state)
{
	// Calcul de l'intervalle en focntion du slider de vitesse
	interval = slider2timer( timerSlider->value() );
	
	// Changement de l'état du timer en focntion de l'état du bouton play
	if(state)
		playTimer->start(interval);
	else
		playTimer->stop();
}

/// Gestion du déplacement du slider
void VisuPropaWin::playSliderChanged(int value)
{
	// Mise à jour du point final de la visu
	visu->changeViewPoint(value);
}

/// Gestion des ticks du timer
void VisuPropaWin::timerTick(void)
{
	// On met à jour l'intervalle
	playTimer->changeInterval(interval);
		
	// On ajoute 1 au slider de progression ou on boucle au départ
	if( playSlider->value() < playSlider->maxValue() )
		playSlider->setValue(playSlider->value()+1);
	else
		playSlider->setValue(playSlider->minValue());
}

/// Gestion du slider du timer
void VisuPropaWin::timerSliderMoved(int value)
{
	// Calcul de l'intervalle
	interval = slider2timer(value);
	// Mise à jour du timer si il est déjà en fonctionnement
	if(playTimer->isActive())
		playTimer->changeInterval(interval);
}

/// Gestion de la case à cocher "Corrélation"
void VisuPropaWin::correlationClicked(bool checked)
{
	if( (vectList == NULL) || (correlList == NULL) )
		return;
		
	if(checked)
	{
		visu->setPointList( vectList, correlList );
		scaleLabel0->setText(QString::number(minCorrel));
		scaleLabel1->setText(QString::number( (maxCorrel - minCorrel)/2.0 ));
		scaleLabel2->setText(QString::number(maxCorrel));
	}
	else
	{
		visu->setPointList( vectList, NULL );
		scaleLabel0->setText(QString::number(1));
		scaleLabel1->setText(QString::number(vectList->getSize() / 2));
		scaleLabel2->setText(QString::number(vectList->getSize()));
	}
}

/// Parcourir les fichiers
void VisuPropaWin::parcoursFichiers(void)
{
	// Définition des format de fichier possibles
	QString types("Tous les fichiers (*)");
		
	// Affichage de la boite de dialogue d'enregistrement
	QString filename = Q3FileDialog::getOpenFileName(QString::null, types, this, "Save Dialog",
		"Choisir le nom de la nouvelle image...");
	
	// Si l'utilisateur a cliqué sur "Annuler", pas la peine d'aller plus loin
	if(filename.isNull())
		return;
		
	// On vérifie que les deux fichiers existent
        if( QFile::exists(filename) )
	{
		// On vérifie que l'on peut lire dans les deux fichiers.
		QFileInfo fileInfo(filename);
		if( ! fileInfo.isReadable() )
		{
			QMessageBox::critical(this, "Erreur de fichier", "Le fichier spécifié ne peut être lu",
				QMessageBox::Ok, QMessageBox::NoButton);
			return;
		}
	}
	else
	{
		QMessageBox::critical(this, "Erreur de fichier", "Le fichier spécifié n'existe pas",
			QMessageBox::Ok, QMessageBox::NoButton);
		return;
	}

	// Ouverture du fichier
	QFile file(filename);
	
	if(! file.open(QIODevice::ReadOnly) )
	{
		QMessageBox::critical(this, "Erreur de fichier", "Le fichier spécifié n'a pas pu être ouvert",
			QMessageBox::Ok, QMessageBox::NoButton);
		return;
	}
	
	fileLine->setText(filename);
	
	// Création des listes
	if(vectList != NULL)
	{
		delete vectList;
		vectList = NULL;
	}
	if(correlList != NULL)
	{
		delete correlList;
		correlList = NULL;
	}
	vectList = new LVectUniqueQueue();
	correlList = new Q3ValueList<double>;
	if( (vectList == NULL) || (correlList == NULL) )
	{
		QMessageBox::critical(this, "Erreur de mémoire", "Impossible de créer la liste des points en mémoire",
			QMessageBox::Ok, QMessageBox::NoButton);
		return;
	}
	
	// Lecture du fichier
	Q3TextStream stream(&file);
	QString line;
	LVect maxPnt(0,0,0);
	maxCorrel = -1.0;
	minCorrel = 2.0;
	
	long tmpX, tmpY, tmpZ;
	double tmpC;
	bool testX = false,testY = false,testZ = false, testC = false;
	
	int res;
	
	// Définition des expressions rationelles pour lire le fichier
	QRegExp re1("^[0-9]:");
	QRegExp re2("^[\\[]");
	QRegExp re3("^([0-9]+)");
	QRegExp re4("^[\\|]");
	QRegExp re5("^([\\]] [\\:])");
	QRegExp re6("^([+-]?(?:[0-9]+|[0-9]+[.]|[.][0-9]+|[0-9]+[.][0-9]+)(?:[eE][+-]?[0-9]+)?)");
	
	while(! (line = stream.readLine()).isNull() )
	{
		line = line.stripWhiteSpace();
		
		// "N:"
		res = re1.search(line);
		if( res == -1 )
		{
// 			cout << "erreur 1 :" << line << endl;
			continue;
		}
		line = line.remove(0,2).stripWhiteSpace();
		
		// "["
		res = re2.search(line);
		if( res == -1 )
		{
// 			cout << "erreur 2 : " << line << endl;
			continue;
		}
		line = line.remove(0,1).stripWhiteSpace();
		
		// X
		res = re3.search(line);
		if( res == -1 )
		{
// 			cout << "erreur 3 : " << line << endl;
			continue;
		}
		tmpX = re3.cap(1).toLong(&testX);
		line = line.remove(0,re3.cap(1).length()).stripWhiteSpace();
		
		// "|"
		res = re4.search(line);
		if( res == -1 )
		{
// 			cout << "erreur 4 : " << line << endl;
			continue;
		}
		line = line.remove(0,1).stripWhiteSpace();
		
		// Y
		res = re3.search(line);
		if( res == -1 )
		{
// 			cout << "erreur 3b : " << line << endl;
			continue;
		}
		tmpY = re3.cap(1).toLong(&testY);
		line = line.remove(0,re3.cap(1).length()).stripWhiteSpace();
		
		// "|"
		res = re4.search(line);
		if( res == -1 )
		{
// 			cout << "erreur 4b : " << line << endl;
			continue;
		}
		line = line.remove(0,1).stripWhiteSpace();
		
		// Z
		res = re3.search(line);
		if( res == -1 )
		{
// 			cout << "erreur 3c : " << line << endl;
			continue;
		}
		tmpZ = re3.cap(1).toLong(&testZ);
		line = line.remove(0,re3.cap(1).length()).stripWhiteSpace();
		
		// ":"
		res = re5.search(line);
		if( res == -1 )
		{
// 			cout << "erreur 5 : " << line << endl;
			continue;
		}
		line = line.remove(0,re5.cap(1).length()).stripWhiteSpace();
		
		// C
		res = re6.search(line);
		if( res == -1 )
		{
// 			cout << "erreur 6 : " << line << endl;
			continue;
		}
// 		cout << "OK 6 : " << line << endl;
		
		tmpC = re6.cap(1).toDouble(&testC);
		line = line.remove(0,re6.cap(1).length()).stripWhiteSpace();
		
		// On vérifie que les trois nombres ont été correctement convertis
		if(! (testX && testY && testZ && testC) )
		{
// 			cout << "erreur conversion" << endl;
			continue;
		}
		
// 		cout << "OK" << endl;
		
		// Création du point
		LVect tmpPnt( tmpX, tmpY, tmpZ );
		
		// On calcule le point de coordonnées max
		//if( (tmpPnt.getMax() >= maxPnt.getMax()) && (tmpPnt.getNorme() > maxPnt.getNorme()))
		//	maxPnt = tmpPnt;
		if( tmpPnt.getX() > maxPnt.getX() )
			maxPnt.setX(tmpPnt.getX());
		if( tmpPnt.getY() > maxPnt.getY() )
			maxPnt.setY(tmpPnt.getY());
		if( tmpPnt.getZ() > maxPnt.getZ() )
			maxPnt.setZ(tmpPnt.getZ());
			
		// On calcule la correl max et mini
		if(tmpC > maxCorrel)
			maxCorrel = tmpC;
		if( (tmpC < minCorrel) && (tmpC >= 0.0) )
			minCorrel = tmpC;

		// Si le point est déjà dans la liste, on le supprime et on le rajoute
		if(vectList->contains(tmpPnt))
		{
			correlList->remove( (*correlList)[vectList->findVecteur(tmpPnt)] );
			vectList->removeVecteur(tmpPnt);
		}
		// Ajout du point à la file
		vectList->addVecteur( tmpPnt );
		// Ajout de la valeur du coeff de corrélation
		correlList->append(tmpC);
	}
	
	// On vérifie que l'on a bien des points dans la liste
	if(vectList->getSize() > 0)
	{
		// On met à jour la liste dans la visualisation
		// Si la case "Correlation" est cochée, on met aussi les coeffs de corrélation
		if(correlBox->isChecked())
		{
			visu->setPointList( vectList, correlList );
			scaleLabel0->setText(QString::number(minCorrel));
			scaleLabel1->setText(QString::number( (maxCorrel - minCorrel)/2.0 ));
			scaleLabel2->setText(QString::number(maxCorrel));
		}
		else
		{
			visu->setPointList( vectList, NULL );
			scaleLabel0->setText(QString::number(1));
			scaleLabel1->setText(QString::number(vectList->getSize() / 2));
			scaleLabel2->setText(QString::number(vectList->getSize()));
		}

		// On met à jour la boite dans la visu
		visu->setSize(maxPnt);
		// On met à jour le slider de progression pour qu'il corresponde au nombre de points
		playSlider->setMaxValue(vectList->getSize());
		playSlider->setValue(vectList->getSize());
		// On met à jour l'affichage des dimensions
		dimLabel->setText(	QString::number(maxPnt.getX() + 1) + " ; "
				+	QString::number(maxPnt.getY() + 1) + " ; "
				+	QString::number(maxPnt.getZ() + 1));
	}
	else
	{
		// Message d'erreur si aucun point n'a été trouvé dans le fichier
		QMessageBox::critical(this, "Erreur de fichier", "Le fichier spécifié ne contient pas de données correctes.",
			QMessageBox::Ok, QMessageBox::NoButton);
		dimLabel->setText("Inconnu");
		return;
	}
	
	file.close();
}
