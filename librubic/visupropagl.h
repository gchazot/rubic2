/** \file visupropagl.h
  * \brief Contient la définition du widget VisuPropaGL
  */

/****************************************************************************
** $Id: visupropagl.h,v 1.5 2006/06/16 15:30:20 gchazot Exp $
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
****************************************************************************/

#ifndef GLDEFBOX_H
#define GLDEFBOX_H

#include <qgl.h>
#include <qpoint.h>
#include <q3valuelist.h>
//Added by qt3to4:
#include <QMouseEvent>

#include "lvectuniquequeue.h"
#include "lvect.h"
#include "dvect.h"

/// Widget de visualisation de la propagation du calcul
/** Ce widget permet d'afficher dans une fenetre OpenGL la propagation du calcul
  * dans rubicalc. Pour cela, il faut lui passer la liste des points dans
  * l'ordre où ils ont été calculés (avec un LVectUniqueQueue) et le point où
  * s'arreter. Il est également possible d'afficher les limites du calcul avec
  * une boite en passant au widget la taille de l'objet à afficher.
  */
class VisuPropaGL : public QGLWidget
{
Q_OBJECT

public:

	/// Constructeur
	VisuPropaGL( QWidget* parent, const char* name );
	/// Destructeur
	~VisuPropaGL();

public slots:
	/// Modifie l'angle de rotation para rapport à l'axe X de la vue
	void setXRotation( int degrees );
	/// Modifie l'angle de rotation para rapport à l'axe Y de la vue
	void setYRotation( int degrees );
	/// Modifie l'angle de rotation para rapport à l'axe Z de la vue
	void setZRotation( int degrees );

	/// Modifie les dimensions de l'objet représenté.
	/** Permet de modifier les proportions du parallélépipède rectangle
	  * encadrant tous les points calculés.
	  */
	void setSize(const LVect& vec);
	
	/// Modifie la liste des points à afficher et les coeffs. de corrélation correspondants
	/** Permet de modifier la liste des points à représenter. La liste est
	  * ordonnée pour représenter la propagation. La liste de double est
	  * la liste des coefficients de corrélation correspondants.
	  *
	  * Si newCorrelListe vaut NULL, alors la couleur affichée dépend de
	  * l'ordre de calcul, sinon cela correspond à la valeur du coefficient
	  * de corrélation.
	  */
	void setPointList(LVectUniqueQueue* newList, Q3ValueList<double> *newCorrelList);
	
	/// Modifie le dernier point à afficher
	/** Permet de modifier le point après lequel on s'arrète dans la
	  * représentation de la, propagation. Cela permet de n'afficher que
	  * les points calculés jusquà un instant donné.
	  */
	void changeViewPoint(unsigned long pointNumber);

protected:
	/// Initialisation du rendu OpenGL
	/** Cette fonction est appelée à l'initialisation de l'objet, mais
	  * aussi lors de la modification de la matrice de déformation, des
	  * dimensions de l'objet ou du vecteur de translation, afin de
	  * régénerer les objets représentés.
	  */
	void initializeGL();
	/// Déssine la zone OpenGL
	void paintGL();
	/// Appelée lors du redimensionnement de la zone OpenGL
	void resizeGL( int w, int h );
	
	/// Construit les objets à représenter
	/** Cette fonction construit la liste des objets 3D à représenter et
	  * renvoie un handle pour cette liste.
	  * Les objets construits sont:
	  * 	1) Le parallélépipède rectangle non-déformé
	  * 	2) Le parallélépipède rectangle déformé
	  * 	3) Les axes (X, Y et Z) colorés
	  */
	virtual GLuint makeObject();

	/// Gestion de l'evenement mousePressEvent
	/** Cet évenement est utilisé pour gérer les rotations, translations et
	  * le zoom de l'affichage OpenGL.
	  */
	virtual void mousePressEvent(QMouseEvent *);
	/// Gestion de l'evenement mouseMoveEvent
	/** Cet évenement est utilisé pour gérer les rotations, translations et
	  * le zoom de l'affichage OpenGL.
	  */
	virtual void mouseMoveEvent(QMouseEvent *);
	/// Gestion de l'evenement mouseReleaseEvent
	/** Cet évenement est utilisé pour gérer les rotations, translations et
	  * le zoom de l'affichage OpenGL.
	  */
	virtual void mouseReleaseEvent(QMouseEvent *);
	
	/// Gestion de l'évenement mouseDoubleClick
	/** Lorsque l'utilisateur double-clicke sur le widget, une boite de
	  * dialogue s'affiche, lui permettant d'enregistrer l'affichage dans
	  * une image.
	  */
	virtual void mouseDoubleClickEvent(QMouseEvent *);

signals:
	/// Emit lorsque la rotation de la vue autour de X a changé
	void xRotationChanged( int degrees );
	/// Emit lorsque la rotation de la vue autour de Y a changé
	void yRotationChanged( int degrees );
	/// Emit lorsque le zoom a changé
	void scaleChanged( float scale);
	/// Emit lorsque la translation selon X a changé
	void xTransChanged( float xtr );
	/// Emit lorsque la translation selon Y a changé
	void yTransChanged( float ytr );

private:
	/// Liste des objets OpenGL à déssiner
	GLuint object;
	
	/// Rotation autour de X
	GLfloat xRot;
	/// Rotation autour de Y
	GLfloat yRot;
	/// Rotation autour de Z
	GLfloat zRot;
	/// Facteur de zoom
	GLfloat scale;
	/// Translation selon X
	GLfloat xTrans;
	/// Translation selon Y
	GLfloat yTrans;
	
	/// Sauvegarde de la rotation autour de X
	GLfloat oldXRot;
	/// Sauvegarde de la rotation autour de Y
	GLfloat oldYRot;
	/// Sauvegarde du facteur de zoom
	GLfloat oldScale;
	/// Sauvegarde de la translation selon X
	GLfloat oldXTrans;
	/// Sauvegarde de la translation selon Y
	GLfloat oldYTrans;
	
	/// Sauvegarde de la position de la souris
	QPoint oldPoint;
	/// Bouton enfoncé (lors des déplacements de la souris)
	Qt::MouseButton mPressedButton;
	
	/// Taille du maillage représenté
	DVect objSize;
	
	/// Liste ordonnée des points à représenter
	LVectUniqueQueue* pointList;
	/// Liste ordonnée des valeurs des coefficients de corrélation
	Q3ValueList<double> *correlList;
	/// Maximum de la liste des valeurs des coeffs. de corrélation
	double maxCorrel;
	/// Le numéro du point où on en est dans la visu temporelle
	unsigned long curPoint;
};

/// Cycle de couleurs par défaut
#define DEFAULTCYCLE 0

/// Fontion associant une couleur à une valeur entre 0.0 et 1.0
/** Si la valeur est <0, renvoie du rouge
  * Différents cycles possibles (0: classique, 1: niveau gris, 2: amélioré)
  */
QColor val2color(double val, int cycle = DEFAULTCYCLE);

#endif // GLBOX_H
