#ifndef VISUPROPAWIN_H
#define VISUPROPAWIN_H

#include "visupropawinui.h"

#include <q3valuelist.h>
//Added by qt3to4:
#include <Q3GridLayout>
#include <Q3HBoxLayout>

class QTimer;
class Q3GridLayout;
class VisuPropaGL;
class LVectUniqueQueue;

/// Fenetre de visualisation de la propagation
class VisuPropaWin: public VisuPropaWinUI
{
Q_OBJECT
public:
	/// Constructeur
	VisuPropaWin( QWidget* parent = 0, const char* name = 0 );
	/// Destructeur
	~VisuPropaWin();
	
public slots:
	/// Gestion du click sur play
	void playButtonToggled(bool state);
	/// Gestion du déplacement du slider
	void playSliderChanged(int value);
	
	/// Gestion des ticks du timer
	void timerTick(void);
	/// Gestion du slider du timer
	void timerSliderMoved(int value);	
	
	/// Gestion de la case à cocher "Corrélation"
	void correlationClicked(bool checked);
	
	/// Parcourir les fichiers
	void parcoursFichiers(void);
	
private:
	/// Timer pour la visualisation temporelle
	QTimer* playTimer;
	
	/// La visualisation de la propagation
	VisuPropaGL* visu;
	/// La disposition pour la visu
	Q3HBoxLayout* visuLayout;
	
	/// L'intervalle pour faire avancer la propagation
	int interval;
	
	/// Stockage de la liste des points
	LVectUniqueQueue* vectList;
	/// Stockage des valeurs du coefficient de corrélation
	Q3ValueList<double> *correlList;
	/// Valeur mini de la correlation
	double minCorrel;
	/// Valeur maxi de la correlation
	double maxCorrel;
};
#endif //VISUPROPAWIN_H
