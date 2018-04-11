/** \file scanning.cpp
  * \brief Contient la définition des fonctions utilisées pour le scanning.
  * Contient notamment la fonction executeScanning().
  */

#include "lvect.h"
#include "dvect.h"
#include "dvect24.h"

#include "fonctions.h"
#include "globals.h"
#include "scanningthread.h"

#include <q3semaphore.h>

bool executeScanning(void)
{
	stdlog->DEBUGMESS("executeScanning()");
	
	// Calcul des coordonnées du point initial (centre du voxern initial) d'après
	// les coordonnées du voxern
	LVect Pini = (*resultats)[Ivoxern]->Coord;
	// On arrondi au voxel entier inférieur
	Pini += Lcur/2;
	
	stdlog->DEBUGMESS("Point de départ calculé : " + Pini.toString() );
	
	// Les 8 somments dans l'image initiale
	LVect Aini(Pini - Lcur / 2 * LVect( 1, -1, -1));
	LVect Bini(Pini - Lcur / 2 * LVect( 1,  1, -1));
	LVect Cini(Pini - Lcur / 2 * LVect(-1,  1, -1));
	LVect Dini(Pini - Lcur / 2 * LVect(-1, -1, -1));
	LVect Eini(Pini - Lcur / 2 * LVect( 1, -1,  1));
	LVect Fini(Pini - Lcur / 2 * LVect( 1,  1,  1));
	LVect Gini(Pini - Lcur / 2 * LVect(-1,  1,  1));
	LVect Hini(Pini - Lcur / 2 * LVect(-1, -1,  1));
	
	// Le champ de déplacement initial
	DVect U3( U24[0], U24[1], U24[2] );
	// Les 8 somments dans l'image déformée
	DVect Adef = Aini + U3;
	DVect Bdef = Bini + U3;
	DVect Cdef = Cini + U3;
	DVect Ddef = Dini + U3;
	DVect Edef = Eini + U3;
	DVect Fdef = Fini + U3;
	DVect Gdef = Gini + U3;
	DVect Hdef = Hini + U3;
	
	// Valeurs de la meilleure correlation
	double corA, corB, corC, corD, corE, corF, corG, corH;
	
	// Scanning sur les 8 sommets du cube, un thread pour chaque

	// Sémaphore gérant le nombre de threads maxi executables en même temps
	QSemaphore scanSem(MaxThreads);
	
	ScanningThread threadA(Aini, Adef, &Adef, &corA, &scanSem, "A");
		threadA.start();
	ScanningThread threadB(Bini, Bdef, &Bdef, &corB, &scanSem, "B");
		threadB.start();
	ScanningThread threadC(Cini, Cdef, &Cdef, &corC, &scanSem, "C");
		threadC.start();
	ScanningThread threadD(Dini, Ddef, &Ddef, &corD, &scanSem, "D");
		threadD.start();
	ScanningThread threadE(Eini, Edef, &Edef, &corE, &scanSem, "E");
		threadE.start();
	ScanningThread threadF(Fini, Fdef, &Fdef, &corF, &scanSem, "F");
		threadF.start();
	ScanningThread threadG(Gini, Gdef, &Gdef, &corG, &scanSem, "G");
		threadG.start();
	ScanningThread threadH(Hini, Hdef, &Hdef, &corH, &scanSem, "H");
		threadH.start();
	
	while(! (threadA.isFinished() && threadB.isFinished() && threadC.isFinished() && threadD.isFinished()
	      && threadE.isFinished() && threadF.isFinished() && threadG.isFinished() && threadH.isFinished() ))
	{
		// On vérifie toutes les secondes si tous les threads sont terminés.
		QThread::currentThread()->wait(1);
	}
	
	if( corA < 0.0 )
	{
		stdlog->DEBUGMESS("Erreur Correlation / A, corA = " + QString::number(corA));
		return false;
	}
	if( corB < 0.0 )
	{
		stdlog->DEBUGMESS("Erreur Correlation / B, corB = " + QString::number(corB));
		return false;
	}
	if( corC < 0.0 )
	{
		stdlog->DEBUGMESS("Erreur Correlation / C, corC = " + QString::number(corC));
		return false;
	}
	if( corD < 0.0 )
	{
		stdlog->DEBUGMESS("Erreur Correlation / D, corD = " + QString::number(corD));
		return false;
	}
	if( corE < 0.0 )
	{
		stdlog->DEBUGMESS("Erreur Correlation / E, corE = " + QString::number(corE));
		return false;
	}
	if( corF < 0.0 )
	{
		stdlog->DEBUGMESS("Erreur Correlation / F, corF = " + QString::number(corF));
		return false;
	}
	if( corG < 0.0 )
	{
		stdlog->DEBUGMESS("Erreur Correlation / G, corG = " + QString::number(corG));
		return false;
	}
	if( corH < 0.0 )
	{
		stdlog->DEBUGMESS("Erreur Correlation / H, corH = " + QString::number(corH));
		return false;
	}
	
	// Calcul des coefficients du champ de déplacement pour ce cube
	// d'après les formules dans rubic.
	
	DVect24 tmpVal; // Variable temporaire
	
	tmpVal.au = (+ Adef[0] + Bdef[0] - Cdef[0] - Ddef[0] + Edef[0] + Fdef[0] - Gdef[0] - Hdef[0]) / 4.0;
	tmpVal.bu = (- Adef[0] + Bdef[0] + Cdef[0] - Ddef[0] - Edef[0] + Fdef[0] + Gdef[0] - Hdef[0]) / 4.0;
	tmpVal.cu = (- Adef[0] - Bdef[0] - Cdef[0] - Ddef[0] + Edef[0] + Fdef[0] + Gdef[0] + Hdef[0]) / 4.0;
	tmpVal.du = (- Adef[0] + Bdef[0] - Cdef[0] + Ddef[0] - Edef[0] + Fdef[0] - Gdef[0] + Hdef[0]) / 2.0;
	tmpVal.eu = (- Adef[0] - Bdef[0] + Cdef[0] + Ddef[0] + Edef[0] + Fdef[0] - Gdef[0] - Hdef[0]) / 2.0;
	tmpVal.fu = (+ Adef[0] - Bdef[0] - Cdef[0] + Ddef[0] - Edef[0] + Fdef[0] + Gdef[0] - Hdef[0]) / 2.0;
	tmpVal.gu = (+ Adef[0] - Bdef[0] + Cdef[0] - Ddef[0] - Edef[0] + Fdef[0] - Gdef[0] + Hdef[0]);
	tmpVal.hu = (+ Adef[0] + Bdef[0] + Cdef[0] + Ddef[0] + Edef[0] + Fdef[0] + Gdef[0] + Hdef[0]) / 8.0;
	
	tmpVal.av = (+ Adef[1] + Bdef[1] - Cdef[1] - Ddef[1] + Edef[1] + Fdef[1] - Gdef[1] - Hdef[1]) / 4.0;
	tmpVal.bv = (- Adef[1] + Bdef[1] + Cdef[1] - Ddef[1] - Edef[1] + Fdef[1] + Gdef[1] - Hdef[1]) / 4.0;
	tmpVal.cv = (- Adef[1] - Bdef[1] - Cdef[1] - Ddef[1] + Edef[1] + Fdef[1] + Gdef[1] + Hdef[1]) / 4.0;
	tmpVal.dv = (- Adef[1] + Bdef[1] - Cdef[1] + Ddef[1] - Edef[1] + Fdef[1] - Gdef[1] + Hdef[1]) / 2.0;
	tmpVal.ev = (- Adef[1] - Bdef[1] + Cdef[1] + Ddef[1] + Edef[1] + Fdef[1] - Gdef[1] - Hdef[1]) / 2.0;
	tmpVal.fv = (+ Adef[1] - Bdef[1] - Cdef[1] + Ddef[1] - Edef[1] + Fdef[1] + Gdef[1] - Hdef[1]) / 2.0;
	tmpVal.gv = (+ Adef[1] - Bdef[1] + Cdef[1] - Ddef[1] - Edef[1] + Fdef[1] - Gdef[1] + Hdef[1]);
	tmpVal.hv = (+ Adef[1] + Bdef[1] + Cdef[1] + Ddef[1] + Edef[1] + Fdef[1] + Gdef[1] + Hdef[1]) / 8.0;
	
	tmpVal.aw = (+ Adef[2] + Bdef[2] - Cdef[2] - Ddef[2] + Edef[2] + Fdef[2] - Gdef[2] - Hdef[2]) / 4.0;
	tmpVal.bw = (- Adef[2] + Bdef[2] + Cdef[2] - Ddef[2] - Edef[2] + Fdef[2] + Gdef[2] - Hdef[2]) / 4.0;
	tmpVal.cw = (- Adef[2] - Bdef[2] - Cdef[2] - Ddef[2] + Edef[2] + Fdef[2] + Gdef[2] + Hdef[2]) / 4.0;
	tmpVal.dw = (- Adef[2] + Bdef[2] - Cdef[2] + Ddef[2] - Edef[2] + Fdef[2] - Gdef[2] + Hdef[2]) / 2.0;
	tmpVal.ew = (- Adef[2] - Bdef[2] + Cdef[2] + Ddef[2] + Edef[2] + Fdef[2] - Gdef[2] - Hdef[2]) / 2.0;
	tmpVal.fw = (+ Adef[2] - Bdef[2] - Cdef[2] + Ddef[2] - Edef[2] + Fdef[2] + Gdef[2] - Hdef[2]) / 2.0;
	tmpVal.gw = (+ Adef[2] - Bdef[2] + Cdef[2] - Ddef[2] - Edef[2] + Fdef[2] - Gdef[2] + Hdef[2]);
	tmpVal.hw = (+ Adef[2] + Bdef[2] + Cdef[2] + Ddef[2] + Edef[2] + Fdef[2] + Gdef[2] + Hdef[2]) / 8.0;
	
	// Stockage du résultat du scanning
	U24 = tmpVal;
	
	// Sortie du champ de déplacement dans le log
	calclog->message(LOGUTIL_INFO, "Resultat scanning : " + tmpVal.toString() );
	calclog->message(LOGUTIL_INFO, "  correlation A : " + QString::number(corA) );
	calclog->message(LOGUTIL_INFO, "  correlation B : " + QString::number(corB) );
	calclog->message(LOGUTIL_INFO, "  correlation C : " + QString::number(corC) );
	calclog->message(LOGUTIL_INFO, "  correlation D : " + QString::number(corD) );
	calclog->message(LOGUTIL_INFO, "  correlation E : " + QString::number(corE) );
	calclog->message(LOGUTIL_INFO, "  correlation F : " + QString::number(corF) );
	calclog->message(LOGUTIL_INFO, "  correlation G : " + QString::number(corG) );
	calclog->message(LOGUTIL_INFO, "  correlation H : " + QString::number(corH) );
	calclog->message(LOGUTIL_INFO, "");
	calclog->message(LOGUTIL_INFO, "###########################################");
	calclog->message(LOGUTIL_INFO, "");
	
	// DEBUG
	stdlog->DEBUGMESS("  tmpVal : " + tmpVal.toString() );
	stdlog->DEBUGMESS("  corA : " + QString::number(corA) );
	stdlog->DEBUGMESS("  corB : " + QString::number(corB) );
	stdlog->DEBUGMESS("  corC : " + QString::number(corC) );
	stdlog->DEBUGMESS("  corD : " + QString::number(corD) );
	stdlog->DEBUGMESS("  corE : " + QString::number(corE) );
	stdlog->DEBUGMESS("  corF : " + QString::number(corF) );
	stdlog->DEBUGMESS("  corG : " + QString::number(corG) );
	stdlog->DEBUGMESS("  corH : " + QString::number(corH) );
	
	return true;
}


double scanningPoint(const LVect& iniPnt, const DVect& defPnt, DVect *minPoint)
{
	// Calcul des bornes "réelles" du scanning
	DVect minScanning = defPnt - (DVect)iniPnt - (tailleZoneScanning / 2.0);
	DVect maxScanning = defPnt - (DVect)iniPnt + (tailleZoneScanning / 2.0);
	
	// Les pas d'incrémentation
	DVect24 pasX, pasY, pasZ;
	pasX[0] = pasScanning;
	pasY[1] = pasScanning;
	pasZ[2] = pasScanning;
	
	// Le mini de corrélation
	double minCorrel = 1.1;
	// La position du mini
	DVect minCorrelPoint;
	
	// Coefficient de corrélation temporaire
	double tmpCorrel;
	
	// Le point de calcul en cours (relatif à iniPnt)
	DVect24 curPos;

	// C'est parti !!
	for( curPos[0] = minScanning[0] ; curPos[0] < maxScanning[0]; curPos += pasX )
		for( curPos[1] = minScanning[1] ; curPos[1] < maxScanning[1]; curPos += pasY )
			for( curPos[2] = minScanning[2] ; curPos[2] < maxScanning[2]; curPos += pasZ )
			{
// 				stdlog->DEBUGMESS("  " + curPos.toString3());

				tmpCorrel = Correlation(iniPnt, curPos);
				if(tmpCorrel < 0.0)
				{
// 					stdlog->DEBUGMESS("  curPos : " + curPos.toString3());
// 					stdlog->DEBUGMESS("  sortie de Correlation : " + QString::number(tmpCorrel));
// 					stdlog->DEBUGMESS("");
					continue;
				}
				if(tmpCorrel < minCorrel)
				{
					minCorrel = tmpCorrel;
					minCorrelPoint[0] = curPos[0];
					minCorrelPoint[1] = curPos[1];
					minCorrelPoint[2] = curPos[2];
				}
			}
			
	// On vérifie que l'on a trouvé une valeur valide
	if( (minCorrel < 0.0) || (minCorrel > 1.0) )
		// On renvoie la valeur d'erreur
		return -1.0;
	
	// Le mini est stocké dans minPoint
	*minPoint = minCorrelPoint;

	// On renvoie la valeur du mini
	return minCorrel;
}
