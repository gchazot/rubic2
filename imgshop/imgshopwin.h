#ifndef IMGSHOPWIN_H
#define IMGSHOPWIN_H

#include "imgshopwinui.h"

class QFile;

/// Fenètre prioncipale de l'application imgshop
class ImgShopWin: public ImgShopWinUI
{

Q_OBJECT

public:
	/// Constructeur
	ImgShopWin(QWidget * parent = 0, const char * name = 0, bool modal = FALSE, Qt::WFlags f = 0);
	/// Destructeur
	~ImgShopWin();
	
private:
	/// Créer une image noire dans le tableau donné
	/** Fonction qui crée une image noire (pleine de 0) dans le fichier donné
	  * en paramètre.
	  */
	void createBlackFile(unsigned char ****data);
	
	/// Créer une image blanche dans le tableau donné
	/** Fonction qui crée une image blanche (pleine de 1) dans le fichier
	  * donné en paramètre.
	  */
	void createWhiteFile(unsigned char ****data);
	
	/// Créer une image aléatoire dans le tableau donné
	/** Fonction qui crée une image aléatoire dans le fichier
	  *  donné en paramètre.
	  */
	void createRandomFile(unsigned char ****data);
	
	/// Créer une image grise dans le tableau donné
	/** Fonction qui crée une image grise dans le fichier donné en paramètre.
	  * Le niveau de gris est gonné en second paramètre.
	  */
	void createGreyFile(unsigned char ****data, char greyLevel);
	
	/// Créer une image "sinus" dans le tableau donné
	/** Fonction qui crée une image en forme de sinus : 
	  * sin(N*Pi*x/xmax)*sin(N*Pi*y/ymax)*sin(N*Pi*z/zmax).
	  */
	void createSinusFile(unsigned char ****data, int N);
	
	/// Créer une image "linéaire" dans le tableau donné
	/** Fonction qui crée une image "linéaire": 
	  * 255 * (x/xmax) * (y/ymax) * (z/zmax)
	  */
	void createLinearFile(unsigned char ****data);
	
 
private slots:
	/// Choisir le fichier à créer
	/** Appelé lorsque le bouton "Choisir fichier" est clické
	  */
	void selectFileName(void);
	/// Créer l'image
	/** Appelé lorsque le vouton "Créer image" est clické
	  */
	void createImage(void);
};

#endif // IMGSHOPWIN_H
