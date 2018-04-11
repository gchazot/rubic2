/** \file gldefbox.cpp
  * \brief Contient la définition des fonctions membres de GLDefBox
  * Ce fichier est basé sur les exemples OpenGL de Qt3
  */

#include "gldefbox.h"

#include <cmath>

#include <qstring.h>
#include <qmessagebox.h>
#include <qpixmap.h>
#include <q3filedialog.h>
//Added by qt3to4:
#include <QMouseEvent>

#if defined(Q_CC_MSVC)
#pragma warning(disable:4305) // init: truncation from const double to float
#endif

/// Facteur de grossissement de l'objet pour l'affichage
#define SIZEFACT 2.0

using namespace Qt;

GLDefBox::GLDefBox( QWidget* parent, const char* name )
    : QGLWidget( parent, name )
{
	xRot = 30.0;		// default object rotation
	yRot = 330.0;
	zRot = 0.0;
	scale = 0.8;		// default object scale
	xTrans = -0.3;		// default object translation
	yTrans = -0.1;		
	object = 0;
	
	// Initialisation des sauvegardes
	oldXRot = oldYRot = 0.0;
	oldScale = 1.0;
	oldXTrans = oldYTrans = 0.0;
	oldPoint = QPoint(0,0);
	mPressedButton = NoButton;
	
	// Initialisation du gradient de la transformation	
	defMatrix = DMatrix::Identite();
	// Initialisation de la translation appliquée à la déformée
	transVect = DVect(0.0,0.0,0.0);
	
	// Initialisation des proportions de l'objet
	objSize = DVect(1.0,1.0,1.0) * SIZEFACT;
	objMax = 1.0;
}

GLDefBox::~GLDefBox()
{
	makeCurrent();
	glDeleteLists( object, 1 );
}

void GLDefBox::paintGL()
{
	// Efface l'affichage
	glClear( GL_COLOR_BUFFER_BIT );
	
	// Charge une matrice identité pour la transformation
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

void GLDefBox::initializeGL()
{
	qglClearColor( black ); 	// Let OpenGL clear to black
	
	// If not called just after construction
	if(object != 0)
		glDeleteLists( object, 1 );
		
	object = makeObject();		// Generate an OpenGL display list
	glShadeModel( GL_FLAT );
}

void GLDefBox::resizeGL( int w, int h )
{
	glViewport( 0, 0, (GLint)w, (GLint)h );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	//glFrustum( -1.0, 1.0, -1.0, 1.0, 5.0, 15.0 );	// Pour un rendu projeté
	glOrtho( -1.5, 1.5, -1.5, 1.5, 0.0, 100.0 );	// Pour un rendu orthogonal
	glMatrixMode( GL_MODELVIEW );
}

GLuint GLDefBox::makeObject()
{
	// Définition du paralélépipèg de rectangle non-déformé ABCDEFGH
	DVect A(0.0,		0.0,		0.0		);
	DVect B(objSize.getX(),	0.0,		0.0		);
	DVect C(objSize.getX(),	objSize.getY(),	0.0		);
	DVect D(0.0,		objSize.getY(),	0.0		);
	DVect E(0.0,		0.0,		objSize.getZ()	);
	DVect F(objSize.getX(),	0.0,		objSize.getZ()	);
	DVect G(objSize.getX(),	objSize.getY(),	objSize.getZ()	);
	DVect H(0.0,		objSize.getY(),	objSize.getZ()	);
	
	// Définition du repère affiché (O,X,Y,Z)
	DVect O(0.0,0.0,0.0);
	DVect X(1.5,0.0,0.0);
	DVect Y(0.0,1.5,0.0);
	DVect Z(0.0,0.0,1.5);
	
	GLuint list;
	
	list = glGenLists( 1 );
	
	glNewList( list, GL_COMPILE );
	
	/////////// Dessin du cube NON-déformé
	qglColor( gray );		      // Shorthand for glColor3f or glIndex
	
	glLineWidth( 1.0 );
	
	// Base ABCD
	glBegin( GL_LINE_LOOP );
	glVertex3f( DVECT23F(A) );
	glVertex3f( DVECT23F(B) );
	glVertex3f( DVECT23F(C) );
	glVertex3f( DVECT23F(D) );
	glEnd();
	
	// base EFGH
	glBegin( GL_LINE_LOOP );
	glVertex3f( DVECT23F(E) );
	glVertex3f( DVECT23F(F) );
	glVertex3f( DVECT23F(G) );
	glVertex3f( DVECT23F(H) );
	glEnd();
	
	// Lignes AE BF CG et DH
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

	/////////// Dessin du cube déformé
	
	// Application de la transformation à chaque point ABCDEFGH
	A = defMatrix * A + transVect;
	B = defMatrix * B + transVect;
	C = defMatrix * C + transVect;
	D = defMatrix * D + transVect;
	E = defMatrix * E + transVect;
	F = defMatrix * F + transVect;
	G = defMatrix * G + transVect;
	H = defMatrix * H + transVect;
	
	qglColor( white );		      // Shorthand for glColor3f or glIndex
	
	glLineWidth( 1.0 );
	
	// Base ABCD
	glBegin( GL_LINE_LOOP );
	glVertex3f( DVECT23F(A) );
	glVertex3f( DVECT23F(B) );
	glVertex3f( DVECT23F(C) );
	glVertex3f( DVECT23F(D) );
	glEnd();
	
	// Base EFGH
	glBegin( GL_LINE_LOOP );
	glVertex3f( DVECT23F(E) );
	glVertex3f( DVECT23F(F) );
	glVertex3f( DVECT23F(G) );
	glVertex3f( DVECT23F(H) );
	glEnd();
	
	// Lignes AE BF CG et DH
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
	qglColor( red );
	glBegin( GL_LINES );
	glVertex3f( DVECT23F(O) ); 
	glVertex3f( DVECT23F(X) );
	glEnd();
	
	// Axe Y
	qglColor( green );
	glBegin( GL_LINES );
	glVertex3f( DVECT23F(O) ); 
	glVertex3f( DVECT23F(Y) );
	glEnd();
	
	// Axe Z
	qglColor( blue );
	glBegin( GL_LINES );
	glVertex3f( DVECT23F(O) ); 
	glVertex3f( DVECT23F(Z) );
	glEnd();
	
	// Fin de la liste d'objets
	glEndList();
	
	return list;
}

void GLDefBox::setXRotation( int degrees )
{
    xRot = (GLfloat)(degrees % 360);
    updateGL();
}

void GLDefBox::setYRotation( int degrees )
{
    yRot = (GLfloat)(degrees % 360);
    updateGL();
}

void GLDefBox::setZRotation( int degrees )
{
    zRot = (GLfloat)(degrees % 360);
    updateGL();
}

// mat est la matrice F !!
void GLDefBox::setDefMatrix( const DMatrix& mat)
{
	defMatrix = mat;
	// On recalcule les objets OpenGL
	initializeGL();
	// On redéssine la visualisation
	updateGL();
}

void GLDefBox::setTransVect( const DVect& vec)
{
	transVect = vec / objMax;
	// On recalcule les objets OpenGL
	initializeGL();
	// On redéssine la visualisation
	updateGL();
}
void GLDefBox::setObjSize(const LVect& vec)
{
	objSize = (DVect)vec;
	objMax = objSize.getMax();
	
	// On proportionne l'image pour que la dimension max soit 1.0
	objSize = objSize * SIZEFACT / objMax;
	
	// On recalcule les objets OpenGL
	initializeGL();
	// On redéssine la visualisation
	updateGL();
}

void GLDefBox::mousePressEvent(QMouseEvent *ev)
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

void GLDefBox::mouseMoveEvent(QMouseEvent *ev)
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
	
void GLDefBox::mouseReleaseEvent(QMouseEvent * ev)
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

void GLDefBox::mouseDoubleClickEvent(QMouseEvent *)
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
