/** \file visupropagl.cpp
  * \brief Contient la définition des membres du widget VisuPropaGL
  */

/****************************************************************************
** $Id: visupropagl.cpp,v 1.6 2006/06/20 09:32:32 gchazot Exp $
**
** Copyright (C) 1992-2005 Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

/****************************************************************************
**
** This is a simple QGLWidget displaying an openGL wireframe box
**
** The OpenGL code is mostly borrowed from Brian Pauls "spin" example
** in the Mesa distribution
**
****************************************************************************/

#include "visupropagl.h"

#include <cmath>

#include <qstring.h>
#include <qpixmap.h>
#include <qmessagebox.h>
#include <q3filedialog.h>
#include <qcolor.h>
//Added by qt3to4:
#include <Q3ValueList>
#include <QMouseEvent>

using namespace Qt;

#if defined(Q_CC_MSVC)
#pragma warning(disable:4305) // init: truncation from const double to float
#endif

/// Facteur de grossissement de l'objet pour l'affichage
#define SIZEFACT 2.0

// Fontion associant une couleur à une valeur entre 0.0 et 1.0
/* Si la valeur est <0, renvoie du rouge
 * Différents cycles possibles (0: classique, 1: niveau gris, 2: amélioré)
 */
inline QColor val2color(double val, int cycle)
{
	const double nbisoelt = 100.0;
	double red, green, blue;
	double tmp, x;
	
	if( (val > 1.0) || (val < 0.0) )
		return Qt::yellow;
	tmp = (double)((long)(val*nbisoelt)) / nbisoelt;
	switch(cycle)
	{
	case 0:
		x = 1020.0 * tmp;
		if(x < 510.0)
		{
			red = 0.0;
			green = x;
			blue = 510.0 - x;
		}
		else
		{
			red = x - 510.0;
			green = 1020.0 - x;
			blue = 0.0;
		}
		red /= 2.0;
		green /= 2.0;
		blue /= 2.0;
		break;
	case 1:
		red = green = blue = tmp * 255.0;
		break;
	case 2:
		red = green = blue = 255.0;
		if(tmp < 1.0/7.0)
		{
			red = 0.0;
			green = 7.0 * tmp * 255.0;
		}
		else if (tmp < 2.0/7.0)
		{
			red = 0.0;
			blue = (2.0 - 7.0 * tmp) * 255.0;
		}
		else if (tmp < 3.0/7.0)
		{
			red = (7.0 * tmp - 2.0) * 255.0;
			blue = 0.0;
		}
		else if (tmp < 4.0/7.0)
		{
			green = (4.0 - 7.0 * tmp) * 255.0;
			blue = 0.0;
		}
		else if (tmp < 5.0/7.0)
		{
			green = 0.0;
			blue = (7.0 * tmp - 4.0) *255.0;
		}
		else if (tmp < 6.0/7.0)
		{
			green = 0.0;
			red = (6.0/7.0 - tmp) * 255.0 * 7.0;
			blue = (6.0/7.0 - tmp) * 255.0 * 7.0;
		}
		else
		{
			green = (tmp - 6.0/7.0) * 255.0 * 7.0;
			red = (tmp - 6.0/7.0) * 255.0 * 7.0;
			blue = (tmp - 6.0/7.0) * 255.0 * 7.0;
		}
		break;
	default:
		red = green = blue = 255.0;
		break;
	}
	return qRgb((int)red, (int)green, (int)blue);
}

VisuPropaGL::VisuPropaGL( QWidget* parent, const char* name )
    : QGLWidget( parent, name )
{
	xRot = 30.0;            // default object rotation
	yRot = 330.0;
	zRot = 0.0;
	scale = 0.8;            // default object scale
	xTrans = -0.3;          // default object translation
	yTrans = -0.1;
	object = 0;
	
	// Initialisation des sauvegardes
	oldXRot = oldYRot = 0.0;
	oldScale = 1.0;
	oldXTrans = oldYTrans = 0.0;
	oldPoint = QPoint(0,0);
	
	// Initialisation des proportions de l'objet
	objSize = DVect(1.0,1.0,1.0) * SIZEFACT;
	
	pointList = NULL;
	correlList = NULL;
}


/*!
  Release allocated resources
*/

VisuPropaGL::~VisuPropaGL()
{
	makeCurrent();
	// Destruction des objets graphiques
	glDeleteLists( object, 1 );
}


/*!
  Paint the box. The actual openGL commands for drawing the box are
  performed here.
*/

void VisuPropaGL::paintGL()
{
	// Efface l'affichage
	glClear( GL_COLOR_BUFFER_BIT );
	
	// ????
	glLoadIdentity();
	// Translation de l'affichage
	glTranslatef( xTrans, yTrans, -10.0 );
	// Mise au zoom de l'affichage
	glScalef( scale, scale, scale );
	
	// Rotation de l'affichage
	glRotatef( xRot, 1.0, 0.0, 0.0 );
	glRotatef( yRot, 0.0, 1.0, 0.0 );
	glRotatef( zRot, 0.0, 0.0, 1.0 );
	
	// Rendu des objets
	glCallList( object );
}


/*!
  Set up the OpenGL rendering state, and define display list
*/

void VisuPropaGL::initializeGL()
{
	qglClearColor( black ); 		// Let OpenGL clear to black
	
	// If not called just after construction
        if(object != 0)
                glDeleteLists( object, 1 );

	object = makeObject();		// Generate an OpenGL display list
	glShadeModel( GL_FLAT );
}



/*!
  Set up the OpenGL view port, matrix mode, etc.
*/

void VisuPropaGL::resizeGL( int w, int h )
{
	glViewport( 0, 0, (GLint)w, (GLint)h );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	//glFrustum( -1.0, 1.0, -1.0, 1.0, 5.0, 15.0 );	// Pour un rendu projeté
	glOrtho( -1.5, 1.5, -1.5, 1.5, 0.0, 100.0 );	// Pour un rendu orthogonal
	glMatrixMode( GL_MODELVIEW );
}


/*!
  Generate an OpenGL display list for the object to be shown, i.e. the box
*/

GLuint VisuPropaGL::makeObject()
{
	// Définition des sommets du cube
	DVect A(0.0,		0.0,		0.0		);
	DVect B(objSize.getX(),	0.0,		0.0		);
	DVect C(objSize.getX(),	objSize.getY(),	0.0		);
	DVect D(0.0,		objSize.getY(),	0.0		);
	DVect E(0.0,		0.0,		objSize.getZ()	);
	DVect F(objSize.getX(),	0.0,		objSize.getZ()	);
	DVect G(objSize.getX(),	objSize.getY(),	objSize.getZ()	);
	DVect H(0.0,		objSize.getY(),	objSize.getZ()	);
	
	DVect O(0.0,0.0,0.0);
	DVect X(1.5,0.0,0.0);
	DVect Y(0.0,1.5,0.0);
	DVect Z(0.0,0.0,1.5);
	
	GLuint list;
	
	list = glGenLists( 1 );
	
	glNewList( list, GL_COMPILE );
	
	// Dessin du cube encadrant l'image
	
	qglColor( gray );		      // Shorthand for glColor3f or glIndex
	
	glLineWidth( 1.0 );
	
	glBegin( GL_LINE_LOOP );
	glVertex3f( DVECT23F(A) );
	glVertex3f( DVECT23F(B) );
	glVertex3f( DVECT23F(C) );
	glVertex3f( DVECT23F(D) );
	glEnd();
	
	glBegin( GL_LINE_LOOP );
	glVertex3f( DVECT23F(E) );
	glVertex3f( DVECT23F(F) );
	glVertex3f( DVECT23F(G) );
	glVertex3f( DVECT23F(H) );
	glEnd();
	
	glBegin( GL_LINES );
	
	glVertex3f( DVECT23F(A) );
	glVertex3f( DVECT23F(E) );
	
	glVertex3f( DVECT23F(B) );
	glVertex3f( DVECT23F(F) );
	
	glVertex3f( DVECT23F(C) );
	glVertex3f( DVECT23F(G) );
	
	glVertex3f( DVECT23F(D) );
	glVertex3f( DVECT23F(H) );
	
	glEnd();
	
	// Dessin des axes fixes
	glLineWidth( 1.5 );
	
	// Axe X
	qglColor( blue );
	glBegin( GL_LINES );
	glVertex3f( DVECT23F(O) ); 
	glVertex3f( DVECT23F(X) );
	glEnd();
	
	// Axe X
	qglColor( green );
	glBegin( GL_LINES );
	glVertex3f( DVECT23F(O) ); 
	glVertex3f( DVECT23F(Y) );
	glEnd();
	
	// Axe X
	qglColor( red );
	glBegin( GL_LINES );
	glVertex3f( DVECT23F(O) ); 
	glVertex3f( DVECT23F(Z) );
	glEnd();
	
	// Dessin les points
	if( pointList != NULL )
	{
		LVect* tmp;
		unsigned long size = pointList->getSize();
// 		int coul;
		glPointSize( 5.0 );
		glBegin(GL_POINTS);
		for(unsigned long i = 0; (i < curPoint) && (i < size ); i++)
		{
			// Calcul de la couleur du point
			// rouge: origine
			if(i == 0)
				qglColor( white );
			else
			{
				if(correlList == NULL)
				{
// 					coul = 175 * i / size + 80;
// 					qglColor( qRgba( 128, coul, coul, 75 ) );
					qglColor( val2color((double)i / (double)size) );
				}
				else
				{
// 					coul = (int)((*correlList)[i] / maxCorrel * 175.0 + 80.0);
// 					qglColor( qRgba( coul, 255-coul, 100, 75 ) );
					qglColor( val2color((*correlList)[i]/maxCorrel) );
				}
			}
			
			tmp = pointList->getVecteur(i);
			// Dessine un point
			if(tmp != NULL)
				glVertex3f( DVECT23F((*tmp)) );
		}
		glEnd();
	}
	
	// Fin de la liste d'objets
	glEndList();
	
	return list;
}


/*!
  Set the rotation angle of the object to \e degrees around the X axis.
*/

void VisuPropaGL::setXRotation( int degrees )
{
    xRot = (GLfloat)(degrees % 360);
    updateGL();
}


/*!
  Set the rotation angle of the object to \e degrees around the Y axis.
*/

void VisuPropaGL::setYRotation( int degrees )
{
    yRot = (GLfloat)(degrees % 360);
    updateGL();
}


/*!
  Set the rotation angle of the object to \e degrees around the Z axis.
*/

void VisuPropaGL::setZRotation( int degrees )
{
    zRot = (GLfloat)(degrees % 360);
    updateGL();
}

void VisuPropaGL::setSize(const LVect& vec)
{
	objSize = (DVect)vec;
	objSize = objSize;// * SIZEFACT / objSize.getMax();
	
	// On recalcule les objets OpenGL
	initializeGL();
	// On redéssine la visualisation
	updateGL();
}

void  VisuPropaGL::setPointList(LVectUniqueQueue* newList, Q3ValueList<double> *newCorrelList)
{
	// Si une liste avait dééjà été donnée
	if(pointList != NULL)
		delete pointList;
	// Création de la nouvelle liste
	pointList = new LVectUniqueQueue( *newList );
	
	if(pointList == NULL)
		return;
	
	if(correlList != NULL)
	{
		delete correlList;
		correlList = NULL;
	}
	
	if(newCorrelList != NULL)
	{
		correlList = new Q3ValueList<double>(*newCorrelList);
		
		if(correlList == NULL)
			return;
		
		maxCorrel = -2.0;
		Q3ValueList<double>::Iterator it;
		for ( it = correlList->begin(); it != correlList->end(); ++it )
			if( (*it) > maxCorrel )
				maxCorrel = *it;
	}
	
	// On recalcule les objets OpenGL
	initializeGL();
	// On redéssine la visualisation
	updateGL();

}

void VisuPropaGL::changeViewPoint(unsigned long pointNumber)
{
	// Mise à jour du point
	curPoint = pointNumber;
	
	// On recalcule les objets OpenGL
	initializeGL();
	// On redéssine la visualisation
	updateGL();
}

void VisuPropaGL::mousePressEvent(QMouseEvent *ev)
{
	// Sauvegarde du point de départ de la souris
	oldPoint = ev->pos();
	// Sauvegarde du bouton appuyé
	mPressedButton = ev->button();
	
	// Sauvegarde des rotations lors du départ
	oldXRot = xRot;
	oldYRot = yRot;
	
	// Sauvegarde du zoom lors du départ
	oldScale = scale;
	
	// Sauvegarde des translations lors du départ
	oldXTrans = xTrans;
	oldYTrans = yTrans;
	
	// On accepte l'évenement (il ne sera pas transmit au parent)
	ev->accept();
}

void VisuPropaGL::mouseMoveEvent(QMouseEvent *ev)
{
	// Déplacement de la souris depuis le départ
	QPoint move;
	// Variables temporaires pour le calcul des nouvelles rotations
	int newXRot, newYRot;
	
	// Selon le bouton enfoncé
	switch(mPressedButton)
	{
	// Si aucun bouton, on ignore l'évenement et on s'arrète
	case NoButton:
		ev->ignore();
		return;
	// Si le bouton gauche, on fait une rotation
	case LeftButton:
		// Calcul du déplacement de la souris depuis l'enfoncement du bouton
		move = ev->pos() - oldPoint;
		// Un déplacement de toute la largeur de la vue fait tourner de 220°
		move.setX( move.x() * 220 / width() );
		// Un déplacement de toute la hauteur de la vue fait tourner de 220°
		move.setY( move.y() * 220 / height() );
	
		// Calcul du nouvel angle de rotation
		newXRot = (int)oldXRot + move.y();
		newYRot = (int)oldYRot + move.x();
	
		// Un modulo (%) qui reste entre 0 et 359°
		while( newXRot >= 360)
			newXRot -= 360;
		while( newXRot < 0)
			newXRot += 360;
		while( newYRot >= 360)
			newYRot -= 360;
		while( newYRot < 0)
			newYRot += 360;
	
		// Mise à jour des angles de rotation
		xRot = (GLfloat)newXRot;
		yRot = (GLfloat)newYRot;
	
		// On emet les signaux de changement d'angle
		emit( xRotationChanged(newXRot) );
		emit( yRotationChanged(newYRot) );
		break;
	
	case MidButton:
		// Calcul du déplacement de la souris depuis l'enfoncement du bouton
		move = ev->pos() - oldPoint;
		// Calcul du nouveau facteur de zoom
		scale = oldScale * powf( 0.98, (float)move.y() );
		// On emet le signal de changement de zoom
		emit( scaleChanged(scale) );
	
		break;
	
	case RightButton:
		// Calcul du déplacement de la souris depuis l'enfoncement du bouton
		move = ev->pos() - oldPoint;
		// Calcul des nouvelles coordonnées de la translation
		xTrans = oldXTrans + 0.01 * (GLfloat)move.x();
		yTrans = oldYTrans - 0.01 * (GLfloat)move.y();
		// On emet les signaux de changement de translation
		emit( xTransChanged(xTrans) );
		emit( yTransChanged(yTrans) );
		break;
	default:
		// On ne devrait jamais passer par ici
		return;
	}
	
	// Si un changemet à été fait, on met à jour l'affichage
	updateGL();
	// On accepte l'évenement (il ne sera pas transmis au parent)
	ev->accept();
}
	
void VisuPropaGL::mouseReleaseEvent(QMouseEvent * ev)
{
	// Réinitialisation de toutes les sauvegardes
	oldPoint = QPoint(0,0);
	mPressedButton = NoButton;
	
	oldXRot = oldYRot = 0.0;
	oldScale = 1.0;
	oldXTrans = oldYTrans = 0.0;
	
	// On accepte l'évenement (il ne sera pas transmis au parent)
	ev->accept();
}


void VisuPropaGL::mouseDoubleClickEvent(QMouseEvent *)
{
	// Définition des format de fichier possibles
	QString types("Toutes les Images (*.bmp *.jpg *.jpeg *.png);;"
		"Images Bitmap (*.bmp);;"
		"Images Jpeg (*.jpg *.jpeg);;"
		"Images PNG (*.png);;"
		"Tous les fichiers (*)");
	// Affichage de la boite de dialogue d'enregistrement
	QString filename = Q3FileDialog::getSaveFileName(QString::null, types, this, "Save Dialog",
		"Enregistrer l'image...");

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
	
	// On récupère l'extension dans le nom de fichier
	QString extension= filename.right(filename.length()-filename.findRev('.')-1);
	extension = extension.lower(); // On passe tout en minuscules
	
	// Détermination du format de fichier
	QString format = QString::null;
	if(extension == "bmp")
		format = "BMP";
	else if( (extension == "jpeg") || (extension == "jpg") )
		format = "JPEG";
	else if( extension == "png" )
		format = "PNG";
	else
	{
		QMessageBox::warning(this, "Format inconnu",
			"Format de fichier inconnu: impossible d'enregistrer l'image",
			QMessageBox::Ok, QMessageBox::NoButton);
		return;
	}
	// Maintenant on peut tenter d'enregistrer le fichier:
	if( ! renderPixmap().save(filename, format) )
		QMessageBox::warning(this, "Erreur d'enregistrement",
			"Il s'est produit une erreur durant l'enregistrement",
			QMessageBox::Ok, QMessageBox::NoButton);
	
	// Fin de la fonction
	return;
}
