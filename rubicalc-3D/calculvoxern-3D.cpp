/** \file calculvoxern.cpp
7  * \brief Contient les définitions des fonctions de calcul sur un voxern.
  */

#include "globals.h"
#include "fonctions.h"
#include <cmath>
#include <iostream>

using namespace std;

double ChercheVoxern(LVect Pini, DVect24 uE, DVect24 *uS)
// Renvoie les coordonnées dans l'image déformée du point correspondant au point (IXv,IYv,IZv)=Iv de l'image non déformée

{

	stdlog->DEBUGMESS("Entrée dans ChercheVoxern pour le voxern : " + Pini.toString() + " avec initialisation U : " + uE.toString());


	//Coordonnées du point où le minimum est trouvé, et mise en mémoire pour l'itération suivante. Il est initialisé au point initial dans l'image déformée et vaut au final le point trouvé par les gradients conjugués
	DVect24 iter;
	DVect24 iter1;

	//Gradient, et mise en mémoire du gradient pour l'itération suivante
	DVect24 gradient;
	DVect24 gradientm1;

	//Direction
	DVect24 direction;
	
	//Variables pour Newton unidirectionnel
	double alpha, d_alpha;
	double d_phi, dd_phi; 	//Dérivées première et seconde de la corrélation selon la direction des gradients conjugués
	double d_phi1 = 0.0;		//Mise en mémoire du d_phi de l'itération précédente pour s'assurer que le Newton n'oscille pas
	DVect24 def, def1, alphaPt, d_alphaPt, def2;
	double corr_alphaPt; 	//Corrélation au point iter
	double corr_pdalpha, corr_mdalpha; // Corrélation près du point pour le calcul de la dérivée en différences finies dans le Newton
	double variation; 	//valeur correspondant à la précision du test de sortie du GC des sommets
	double variationVoxern; //valeur correspondant à la précision du test de sortie de la boucle/voxern
	
	//Variable indiquant à la fonction Coeff_Correlation si l'on veut le calcul des dérivées de la fonction de corrélation
	bool deriv;
	double correl_num, correl_den_def, correl_den_ref;
	DVect24 derivCoeff_1, derivCoeff_2;
	
	// Variables pour le passage à l'optimisation 3D
	LVect coordSommets[8];
	DVect24 resultatsSommets[8];
	short sommetEnCours = 0;
	for(short pX=0; pX<=1; pX++)
	for(short pY=0; pY<=1; pY++)
	for(short pZ=0; pZ<=1; pZ++)
	{
//		coordSommets[sommetEnCours] = Pini + Lcur * LVect(pX, pY, pZ) - Lcur/2;
		coordSommets[sommetEnCours] = Pini + Lcur * LVect(pX, pY, pZ);
// 		resultatsSommets[sommetEnCours] = uE;
		sommetEnCours++;
	}
	
	//Initialisation de def
 	def = uE;
 	// Initialisation de def2
 	def2 = def;
	
	// Nombre d'itération sur chaque sommet
	int i;
	// Nombre d'itérations sur le voxern
	int kk = 0;
	deriv = true;
	stdlog->DEBUGMESS("Entrée dans le gradient conjugué");
	
	// Pour l'interpolation linéaire
	DVect24 iter_pgrad, iter_mgrad, corr_pdelta, corr_mdelta;
	double delta = 1e-4;
	
	do // Boucle de convergence
	{
		// Boucle sur les 8 sommets
		for(sommetEnCours = 0; sommetEnCours <= 8; sommetEnCours++)
		{
			resultatsSommets[sommetEnCours] = def;
			// On réinitialise le compteur de boucles du gradient conjugué des sommets
			i = 0;
			do
			{
				//Calcul du gradient au point iter en différences finies centrées

				if (InterpolCubiq)
				{
					if( Coeff_Correlation(coordSommets[sommetEnCours], resultatsSommets[sommetEnCours], deriv, &correl_num, &correl_den_def, &correl_den_ref, derivCoeff_1, derivCoeff_2) != 1)
						// La correlation s'est mal passée, sortie d'erreur
						return -1.0;
						
					for (int j = 0; j < 3; j++)
					{
						gradient[j] = -(derivCoeff_1[j] * pow(correl_den_def*correl_den_ref,0.5) - correl_num * 0.5 * (1.0/(pow(correl_den_def*correl_den_ref,0.5))) * derivCoeff_2[j] * correl_den_ref) / (correl_den_def * correl_den_ref);	
					}
				}
				else
				{
					for (int j=0; j<3; j++)
					{
		
						iter_pgrad = resultatsSommets[sommetEnCours];
						iter_mgrad = resultatsSommets[sommetEnCours];
						iter_pgrad[j] += delta;
						iter_mgrad[j] -= delta;
						corr_pdelta[j] = Correlation(coordSommets[sommetEnCours], iter_pgrad);
						corr_mdelta[j] = Correlation(coordSommets[sommetEnCours], iter_mgrad);
						if( (corr_pdelta[j]<0.0) || (corr_mdelta[j]<0.0) )
							// La corrélation s'est mal passée, sortie d'erreur
							return -1.0;
					}
		
					gradient = (corr_pdelta - corr_mdelta) / (2.0*delta);
				}
		
		// 		stdlog->DEBUGMESS("gradient : " + gradient.toString());
		
				gradient.normer(1.0);		//on norme le gradient (norme finale de 1)
				
				if (i%3 == 0)
				{
					direction = -gradient;
				}
				else
				{
					direction = -gradient + (gradient.getNorme2()/gradientm1.getNorme2())*direction;	//Formule de Fletcher-Reeves
				}
		
				//Calcul du alpha par Newton unidimensionnel
				alpha = 0.0;
				
				direction.normer(1.0);		//on norme la direction (norme finale de 1)
				
		// 		stdlog->DEBUGMESS("direction : " + direction.toString());
				
				d_alphaPt = pasNewtonGradConj * direction;
				int k = 0;
				
		// 		stdlog->DEBUGMESS("Entrée dans le Newton pour la direction " + QString::number(i));
				
				do
				{
					alphaPt = resultatsSommets[sommetEnCours] + alpha * direction;
					
					corr_alphaPt = Correlation(Pini, alphaPt);
					corr_pdalpha = Correlation(Pini, alphaPt + d_alphaPt);
					corr_mdalpha = Correlation(Pini, alphaPt - d_alphaPt);
					
					// On vérifie que les coeffs de correlation sont valides
					if( (corr_alphaPt < 0.0) || (corr_pdalpha < 0.0) || (corr_mdalpha < 0.0) )
						// Sinon sortie avec erreur
						return -1.0;
							
		// 			d_phi = ( corr_pdalpha - corr_mdalpha ) / (2.0 * d_alphaPt.getNorme());
		// 			dd_phi = ( corr_pdalpha + corr_mdalpha - 2.0*corr_alphaPt ) / pow( d_alphaPt.getNorme(),2.0 );
					
					d_phi = ( corr_pdalpha - corr_mdalpha ) / (2.0 * pasNewtonGradConj);
					dd_phi = ( corr_pdalpha + corr_mdalpha - 2.0*corr_alphaPt ) / pow( pasNewtonGradConj,2.0 );
					
					if(fabs(dd_phi) > precNewtonGradConj)
						d_alpha = d_phi / fabs(dd_phi);
					else
					{
						stdlog->DEBUGMESS("Sortie Newton valeur dd_phi ");
						break;
					}
						
					if ((d_phi1 * d_phi) < 0.0)
						d_alpha = d_alpha/2.0;		//Pour éviter les oscillations du Newton, on divise le d_alpha si la dérivée change de signe entre deux itérations.
						
					alpha -= d_alpha;
					
					d_phi1 = d_phi;
					k++;
				}
				while( (fabs(d_alpha) > precNewtonGradConj) && (k < maxitNewtonGC) );
				
				def1 = resultatsSommets[sommetEnCours];		//mise en mémoire du iter précédent pour calculer la condition de sortie de boucle
				resultatsSommets[sommetEnCours] += alpha * direction;
			
// 				variation = (alpha * direction).getNorme();
		// 		variation = (alpha * direction).getMax();
		// 		variation = direction.getNorme() * corr_alphaPt; //correl_num;
		// 		variation = (alpha * direction).getNorme() / pasNewtonGradConj;
				
				gradientm1 = gradient;
				i++;
				
				stdlog->DEBUGMESS( "Newton " + QString::number(k) + " iterations, Iter GC: " + QString::number(i) );
				
			}while( (i < 12) ); //&& (variation > precGradConj) );
		} // Fin for 8
		
		// TODO: Calcul de def d'après les 8 sommets
		def.au = resultatsSommets[4].hu - resultatsSommets[0].hu;
		def.av = resultatsSommets[4].hv - resultatsSommets[0].hv;
		def.aw = resultatsSommets[4].hw - resultatsSommets[0].hw;
		def.bu = resultatsSommets[2].hu - resultatsSommets[0].hu;
		def.bv = resultatsSommets[2].hv - resultatsSommets[0].hv;
		def.bw = resultatsSommets[2].hw - resultatsSommets[0].hw;
		def.cu = resultatsSommets[1].hu - resultatsSommets[0].hu;
		def.cv = resultatsSommets[1].hv - resultatsSommets[0].hv;
		def.cw = resultatsSommets[1].hw - resultatsSommets[0].hw;
		def.du = resultatsSommets[6].hu + resultatsSommets[0].hu - resultatsSommets[4].hu - resultatsSommets[2].hu;
		def.dv = resultatsSommets[6].hv + resultatsSommets[0].hv - resultatsSommets[4].hv - resultatsSommets[2].hv;
		def.dw = resultatsSommets[6].hw + resultatsSommets[0].hw - resultatsSommets[4].hw - resultatsSommets[2].hw;
		def.eu = resultatsSommets[5].hu + resultatsSommets[0].hu - resultatsSommets[4].hu - resultatsSommets[1].hu;
		def.ev = resultatsSommets[5].hv + resultatsSommets[0].hv - resultatsSommets[4].hv - resultatsSommets[1].hv;
		def.ew = resultatsSommets[5].hw + resultatsSommets[0].hw - resultatsSommets[4].hw - resultatsSommets[1].hw;
		def.fu = resultatsSommets[3].hu + resultatsSommets[0].hu - resultatsSommets[2].hu - resultatsSommets[1].hu;
		def.fv = resultatsSommets[3].hv + resultatsSommets[0].hv - resultatsSommets[2].hv - resultatsSommets[1].hv;
		def.fw = resultatsSommets[3].hw + resultatsSommets[0].hw - resultatsSommets[2].hw - resultatsSommets[1].hw;
		def.gu = resultatsSommets[7].hu + resultatsSommets[1].hu + resultatsSommets[4].hu + resultatsSommets[2].hu - resultatsSommets[0].hu - resultatsSommets[6].hu - resultatsSommets[5].hu - resultatsSommets[3].hu;
		def.gv = resultatsSommets[7].hv + resultatsSommets[1].hv + resultatsSommets[4].hv + resultatsSommets[2].hv - resultatsSommets[0].hv - resultatsSommets[6].hv - resultatsSommets[5].hv - resultatsSommets[3].hv;
		def.gw = resultatsSommets[7].hw + resultatsSommets[1].hw + resultatsSommets[4].hw + resultatsSommets[2].hw - resultatsSommets[0].hw - resultatsSommets[6].hw - resultatsSommets[5].hw - resultatsSommets[3].hw;

		def.hu = def.hv = def.hw = 0.0;
		for(short pp = 0; pp<8; pp++)
		{
			def.hu += resultatsSommets[pp].hu;
			def.hv += resultatsSommets[pp].hv;
			def.hw += resultatsSommets[pp].hw;
		}
		def.hu /= 8.0;
		def.hv /= 8.0;
		def.hw /= 8.0;
		
		variationVoxern = (def-def2).getNorme();
		def2 = def;
	
		kk++;
	// Boucle sur le voxern
	}while( (kk < maxitGC) && (variationVoxern > precGradConj) );
	
	*uS = def;
	
	stdlog->DEBUGMESS("Sortie ChercheVoxern");
	
	// Cas d'une sortie aux itérations
// 	if(i >= maxitGC)
// 		return -0.5;
	
	// On renvoie la valeur la plus proche du coefficient de correlation sans en recalculer un nouveau
	return (Correlation(Pini, def));
}

