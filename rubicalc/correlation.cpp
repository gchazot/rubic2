/** \file correlation.cpp
  * \brief Contient les définitions des fonctions utilisées par la corrélation.
  */

#include "fonctions.h"
#include "globals.h"

#include <cmath>
#include <iostream>
using namespace std;

double Correlation(const LVect& Pini, const DVect24& def)
{
// 	stdlog->DEBUGMESS("Correlation() :");
// 	stdlog->DEBUGMESS("  Pini : " + Pini.toString());
// 	stdlog->DEBUGMESS("  def : " + def.toString3());
	
	double coeff_num, coeff_den_def, coeff_den_ref;
	DVect24 der1, der2;
	double result = -1.0;
	
// 	DVect PiniD = (DVect) Pini;
	
	int retour = Coeff_Correlation( Pini, (DVect24&)def, false, &coeff_num, &coeff_den_def, &coeff_den_ref, (DVect24&)der1, (DVect24&)der2);
	 
	if( retour != 1 )
	{
		calclog->message(LOGUTIL_CRITICAL, "Erreur de calcul de correlation");
		
// 		stdlog->DEBUGMESS("Erreur dans Coeff_Correlation, valeur de retour : " + QString::number(retour));
		return -1.0;
	}
	
	if( (coeff_den_def != 0.0) && (coeff_den_ref != 0.0) )
	{
// 		stdlog->DEBUGMESS("  CAS 1");
		result = 1.0 - coeff_num / pow(coeff_den_def * coeff_den_ref, 0.5);
	}
	else if ( coeff_num != 0.0 )
	{
// 		stdlog->DEBUGMESS("  CAS 2");
		result = -1.0;
	}
	else
	{
		result = 0.0;
// 		stdlog->DEBUGMESS("  CAS 3");
	}
	
// 	stdlog->DEBUGMESS(Pini.toString() + def.toString3() );
// 	stdlog->DEBUGMESS("Coeff_Correlation OK : " + QString::number(result));
	return ( result );
}

int Coeff_Correlation(const LVect& Iv, DVect24& u, bool deriv, double *correl_num, double *correl_den_def, double *correl_den_ref, DVect24& derivCoeff_1, DVect24& derivCoeff_2)
{
// Iv: coordonnées du centre du voxern de l'image non déformée, dont on cherche la position (i.e. le champ de déplacement) dans l'image déformée
// deriv: vaut false lors de l'appel si l'on veut juste calculer le coefficient de corrélation, vaut true si on veut aussi calculer les dérivées
	long Ipat, Jpat, Kpat;
	double Udef, Vdef, Wdef;
	DVect coord_cur;
	DVect coordvv;
	LVect Dep_000;		// coordonnées entières du point courant dans l'image déformée
	double NG_Def_Vox, NG_Ref_Vox;
	DVect coordvv_Ref;
	LVect Dep_000_Ref;
	LVect coord_cur_Ref;
	double Lcur2, Lcur3;
	DVect24 dai_X;
	DVect24 dai_Y;
	DVect24 dai_Z;
	DVect24 derai_PixS;
	
	*correl_num = 0.0;
	*correl_den_def = 0.0;
	*correl_den_ref = 0.0;
	
	for(Ipat = 0; Ipat < Lcur; Ipat++)
	{
		for(Jpat = 0; Jpat < Lcur; Jpat++)
		{
			for(Kpat = 0; Kpat < Lcur; Kpat++)
			{
	
				Lcur2 = pow((double)Lcur,2);
				Lcur3 = pow((double)Lcur,3);
				
				Udef = u[0] + u[3]*Ipat/Lcur + u[6]*Jpat/Lcur + u[9]*Kpat/Lcur + u[12]*(Ipat*Jpat)/Lcur2 + u[15]*(Ipat*Kpat)/Lcur2 + u[18]*(Jpat*Kpat)/Lcur2 + u[21]*(Ipat*Jpat*Kpat)/Lcur3;
				
				Vdef = u[1] + u[4]*Ipat/Lcur + u[7]*Jpat/Lcur + u[10]*Kpat/Lcur + u[13]*(Ipat*Jpat)/Lcur2 + u[16]*(Ipat*Kpat)/Lcur2 + u[19]*(Jpat*Kpat)/Lcur2 + u[22]*(Ipat*Jpat*Kpat)/Lcur3;
				
				Wdef = u[2] + u[5]*Ipat/Lcur + u[8]*Jpat/Lcur + u[11]*Kpat/Lcur + u[14]*(Ipat*Jpat)/Lcur2 + u[17]*(Ipat*Kpat)/Lcur2 + u[20]*(Jpat*Kpat)/Lcur2 + u[23]*(Ipat*Jpat*Kpat)/Lcur3;
				
		// Calcul du niveau de gris dans l'image déformée
				// Coordonnées du point courant dans la déformée
				coordvv = DVect(Ipat, Jpat, Kpat) + (DVect)Iv + DVect(Udef, Vdef, Wdef) - (Lcur/2.0);
				
				// Partie entière des coordonnées du point courant dans la déformée
				Dep_000 = (LVect)(coordvv);
				// Coordonnées du point courant dans le voxern unitaire dans la déformée
				coord_cur = coordvv - (DVect)Dep_000;
				
				// Calcul de la valeur interpolée au point courant
				if(InterpolCubiq)
					NG_Def_Vox = defImg->interpoleCubique(coordvv);
				else
					NG_Def_Vox = defImg->interpoleLineaire(coordvv);
				
				if(NG_Def_Vox == 0)
				{
// 					stdlog->DEBUGMESS("Erreur dans le calcul de NG_Def_Vox, coordvv : " + coordvv.toString());
					return (3);
				}
				
		// Calcul du niveau de gris dans l'image initiale
				// Coordonnées du point courant dans l'image initiale
				coordvv_Ref = DVect(Ipat, Jpat, Kpat) + (DVect)Iv - (Lcur/2.0);

				// Partie entière des coordonnées du point courant dans l'image initiale
				Dep_000_Ref = (LVect)(coordvv_Ref);

				// Récupération de la valeur au point courant
				NG_Ref_Vox = refImg->getData(Dep_000_Ref);
				
				if(NG_Ref_Vox == 0)
				{
// 					stdlog->DEBUGMESS("Erreur dans le calcul de NG_Ref_Vox");
					return (5);
				}
				
		// Calcul des sous-parties du coefficient de corrélation
				*correl_num += NG_Def_Vox*NG_Ref_Vox;			// Somme des PixS * PixIIdp
				*correl_den_def += NG_Def_Vox*NG_Def_Vox;		// Somme des PixS^2
				*correl_den_ref += NG_Ref_Vox*NG_Ref_Vox;		// Somme des PixIIdp^2
				
				
		// Calcul des dérivées si demandé.
				if (deriv == true)
				{
				
					if( ! defImg->deriveCubique(coordvv, Ipat, Jpat, Kpat, Lcur, &derai_PixS) )
					{
						stdlog->DEBUGMESS("Erreur dans le calcul des dérivées");
						return 7;
					}
					
					for (int i=0; i<24; i++)
					{
						derivCoeff_1[i] += derai_PixS[i] * NG_Ref_Vox;		
	// 					cout << "derivCoeff1 " << *derivCoeff_1 << endl << flush;
						derivCoeff_2[i] += 2.0 * NG_Def_Vox * derai_PixS[i];	
					}
				}
				
			}
		}
	}
return (1);
}
