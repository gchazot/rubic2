#ifndef DEFSHOPWIN_H
#define DEFSHOPWIN_H

#include "smoothimgwinui.h"
#include "lvect.h"

class QString;
class DVect;
class LVect;
class DMatrix;
class RubicImage;

/// Fenètre principale de l'application defshop
class SmoothImgWin: public SmoothImgWinUI
{
Q_OBJECT
public:
	/// Constructeur
	SmoothImgWin(QWidget * parent = 0, const char * name = 0, bool modal = FALSE, Qt::WFlags f = 0);
	/// Destructeur
	~SmoothImgWin();

private:
	/// Taille de l'image chargée
	LVect mObjSize;
	
private:
	/// Charge dans un tableau l'image initiale.
	/** Alloue la mémoire du tableau et place dans les paramètres long les
	  * dimensions de l'image Renvoie true en cas de succès, false sinon.
	  */
	bool loadInitImage(RubicImage **img);
	/// Sauvegarde l'image donnée en paramètre.
	/** Renvoie false en cas d'échec.
	  */
	bool saveDefImage(RubicImage *img);
	
	/// Crée une image vide de dimensions données
	/** Alloue un tableau de dimensions données en paramètre et le
	  * remplit de 0. Renvoie true si réussit et false sinon.
	  */
	bool createBlackImg(RubicImage **img, LVect& size);
	
	/// Effectue le calcul de la déformée
	/** Calcule la déformée de l'image initiale dans deformee d'après la
	  * matrice de déformation donnée dans l'interface. Les dimensions des
	  * images sont données dans les trois derniers paramètres.
	  * Renvoie true en cas de succès, false sinon.
	  */
	bool compute(RubicImage *initiale, RubicImage *deformee);
	
private slots:
	/// Choisir l'image initiale
	/** Appelé lorsque le bouton "Choisir fichier" est clické
	  * pour l'image initiale.
	  */
	void selectInitFileName(void);
	/// Choisir l'image déformée
	/** Appelé lorsque le bouton "Choisir fichier" est clické
	  * pour l'image déformée.
	  */
	void selectDefFileName(void);
	/// Lancer la création de l'image
	/** Appelé lorsque le vouton "Créer image" est clické.
	  */
	void createImage(void);
	
};

#endif // DEFSHOPWIN_H
