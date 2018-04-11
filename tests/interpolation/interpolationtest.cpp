#include "rubicimage.h"

#include <iostream>
#include <cstdlib>
#include <cmath>
using namespace std;

// TAILLE DE L'IMAGE UTILISEE
#define TAILLE_IMAGE 10
// MINI DE LA ZONE ETUDIEE
#define MINZONE 1.0
// MAXI DE LA ZONE ETUDIEE
#define MAXZONE 8.0
// PAS D'ETUDE SUR LA ZONE
#define PASZONE 0.125
// Pi
#define PI 3.1416

// Fonction utilisée pour générer l'image
double fonction(double x, double y, double z)
{
// 	return sin(PI*x/TAILLE_IMAGE)*sin(PI*y/TAILLE_IMAGE)*cos(PI*z/TAILLE_IMAGE-PI/2.0)*253.0 + 1.1;
	return (x*y*z / pow(TAILLE_IMAGE, 3.0))*254.0 * 2.0 + 1.0;
}

int main(int, char**)
{
	short ii, ij, ik;
	double di, dj, dk;
	double resdouble;
	unsigned char reschar;
	
	long count = 0;
// 	long erreurs = 0;
	double erreurmoyabs = 0.0;
	double erreurmoyrel = 0.0;
	double erreurmax = -1000.0;
	double erreurmin = 1000.0;
	double erreurtmp;
	
	RubicImage image(TAILLE_IMAGE, TAILLE_IMAGE, TAILLE_IMAGE);
	if( ! image.isValid() )
	{
		cout << "Erreur lors de la création de l'image" << endl << flush;
		return 1;
	}

	for(ii=0; ii<TAILLE_IMAGE; ii++)
		for(ij=0; ij<TAILLE_IMAGE; ij++)
			for(ik=0; ik<TAILLE_IMAGE; ik++)
				image.setData( arrondi(fonction(ii,ij, ik)) , ii, ij, ik);
				
	for(di=MINZONE; di<MAXZONE; di+=PASZONE)
		for(dj=MINZONE; dj<MAXZONE; dj+=PASZONE)
			for(dk=MINZONE; dk<MAXZONE; dk+=PASZONE)
			{
				count++;
				
// 				cout << "Valeur interpolée en " << di << " , " << dj << " , " << dk << endl;
				
				resdouble = image.interpoleLineaire(di, dj, dk);
// 				resdouble = image.interpoleCubique(di, dj, dk);
				
				reschar = arrondi(resdouble);
				
// 				cout << "\tdouble : ";
// 				cout.width(10);
// 				cout << resdouble;
// 				cout << "\tchar :   ";
// 				cout.width(10);
// 				cout << (int)reschar;
// 				cout << "\t";
// 				cout.width(10);
// 				cout << fonction(di, dj, dk);
				
// 				if( ( reschar != ( arrondi(fonction(di, dj, dk))   ) )
// 				 && ( reschar != ( arrondi(fonction(di, dj, dk))+1 ) )
// 				 && ( reschar != ( arrondi(fonction(di, dj, dk))-1 ) )
// 				  )
// 				{
// 					erreurs++;
// 					erreurtmp = (int)reschar - (int)(fonction(di, dj, dk));
					erreurtmp = resdouble - fonction(di, dj, dk);
					erreurmoyabs += fabs(erreurtmp);
					erreurmoyrel += erreurtmp;
					if( erreurtmp > erreurmax )
						erreurmax = erreurtmp;
					if( erreurtmp < erreurmin )
						erreurmin = erreurtmp;
// 					cout << "\t\t#######";
// 				}
				
// 				cout << endl;
			}
	
	erreurmoyabs /= (double)count;
	erreurmoyrel /= (double)count;
	
	cout << "Nombre d'interpolations : " << count << endl;
// 	cout << "Nombre d'erreurs : " << erreurs << endl;
// 	cout << "% erreurs : " << (double)erreurs / (double)count * 100.0 << endl;
	cout << "Erreur moyenne (abs): " << erreurmoyabs << endl;
	cout << "Erreur moyenne (rel) : " << erreurmoyrel << endl;
	cout << "Erreur maximale : " << erreurmax << endl;
	cout << "Erreur minimale : " << erreurmin << endl;
	
	return 0;
	
}
