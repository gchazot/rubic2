#ifndef DEFSHOPWIN_H
#define DEFSHOPWIN_H

#include "getplanwinui.h"
#include "lvect.h"

class QString;
class QDoubleValidator;
class QImage;

class DVect;
class LVect;
class DMatrix;
class RubicImage;

/// Fenètre principale de l'application defshop
class GetPlanWin: public GetPlanWinUI
{
Q_OBJECT
public:
	/// Constructeur
	GetPlanWin(QWidget * parent = 0, const char * name = 0, bool modal = FALSE, Qt::WFlags f = 0);
	/// Destructeur
	~GetPlanWin();

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
	bool saveDefImage(QImage *img);
	
	/// Récupère le plan demandé
	/** Récupère le plan de l'image initiale paramétré dans l'interface et
	  * le place dans l'objet QImage.
	  */
	bool compute(RubicImage *initiale, QImage *plan, LVect *size);
	
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
	/// Mettre à jour les bornes selon le plan voulu
	void updateSpinBox(int selection);
	
};

#endif // DEFSHOPWIN_H
