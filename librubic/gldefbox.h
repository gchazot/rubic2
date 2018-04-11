#ifndef GLDEFBOX_H
#define GLDEFBOX_H

/** \file gldefbox.h
  * \brief Contient la définition de la classe GLDefBox
  */
#include <qgl.h>
#include <qpoint.h>
//Added by qt3to4:
#include <QMouseEvent>

#include "dvect.h"
#include "lvect.h"
#include "dmatrix.h"

/// Widget de prévisualisation d'une déformation d'image
/** Ce widget permet d'afficher dans une fenetre OpenGL la déformation causée
  * par une matrice de déformation (F) et un vecteur de translation.
  * Le widget gère completement l'affichege, notament les évenements de la
  * souris nécessaires à la rotation, au zoom et à la translation de l'image
  * tridimensionelle.
  */
class GLDefBox : public QGLWidget
{
    Q_OBJECT

public:
	/// Constructeur
	GLDefBox( QWidget* parent, const char* name = "" );
	/// Destructeur
	~GLDefBox();

public slots:
	/// Modifie l'angle de rotation para rapport à l'axe X de la vue
	void setXRotation( int degrees );
	/// Modifie l'angle de rotation para rapport à l'axe Y de la vue
	void setYRotation( int degrees );
	/// Modifie l'angle de rotation para rapport à l'axe Z de la vue
	void setZRotation( int degrees );
	
	/// Modifie la matrice de déformation visualisée
	/** La matrice mat donnée en paramètre est la matrice F, gradient de la
	  * transformation, elle doit inclure les rotations si besoin est?
	  */
	void setDefMatrix( const DMatrix& mat);
	/// Modifie le vecteur de translation de l'objet
	/** Ce vecteur est utilisé pour déplacer la déformée après application
	  * de la matrice F.
	  */
	void setTransVect( const DVect& vec);
	/// Modifie les dimensions de l'objet représenté.
	/** Permet de modifier les proportions du parallélépipède rectangle
	  * non-déformé pour coller aux proportions de l'image. Ces dimensions
	  * sont "normées" pour que le rendu soit optimal.
	  */
	void setObjSize(const LVect& vec);

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
	
	/// Le gradient de la transformation appliqué à l'image
	DMatrix defMatrix;
	/// Le vecteur de translation appliqué à l'image
	DVect transVect;
	/// Les proportions de l'image (taille "normée")
	DVect objSize;
	
	/// La dimension maximale de l'image initiale
	double objMax;
};

#endif // GLDEFBOX_H
