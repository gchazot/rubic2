#include "lvect.h"

#include "dvect.h"

#include <iostream>

using namespace std;

// Fonction qui affiche les trois composantes d'un LVect sur une ligne
void aff(const LVect& vec)
{
	cout << "\t";
	cout << vec.getX() << "\t";
	cout << vec.getY() << "\t";
	cout << vec.getZ() << "\t";
	cout << endl;
}

int main()
{
	cout << "############################################################" << endl;
	cout << "######                      DEBUT                     ######" << endl;
	cout << "############################################################" << endl;
	cout << endl;

	// Test du constructeur par défaut
	LVect vec1;
	cout << "* Un vecteur nul:" << endl;
	aff(vec1);
	// Test du constructeur avec initialisation des composantes
	LVect vec2(42, 51, 69);
	cout << "* Un vecteur initialisé (42, 51, 69):" << endl;
	aff(vec2);
	// Test du constructeur de recopie
	LVect vec3 = vec2;
	cout << "* Un vecteur recopié:" << endl;
	aff(vec3);
	// Test du constructeur de recopie par pointeur
	LVect vec4(&vec2);
	cout << "* Un vecteur recopié par pointeur:" << endl;
	aff(vec4);

	// Test de la modification par fonctions
	vec1.setX(12);
	vec1.setY(45);
	vec1.setZ(78);
	cout << "* Un vecteur modifié par fonctions (12, 45, 78):" << endl;
	aff(vec1);
	
	// Test de l'addition de deux vecteurs
	vec4 = vec1 + vec2;
	cout << "* Addition de 2 vecteurs (54, 96, 147):" << endl;
	aff(vec4);

	// Test de la soustraction de deux vecteurs
	vec3 = vec2 - vec1;
	cout << "* Soustraction de 2 vecteurs (30, 6, -9):" << endl;
	aff(vec3);

	// Test du produit scalaire de deux vecteurs
	cout << "* Produit scalaire de deux vecteurs (8181):" << endl;
	cout << "\t" << vec1 * vec2 << endl;

	// Test de l'addition d'une constante à un vecteur
	vec3 = vec2 + 8;
	cout << "* Un vecteur auquel on a ajouté une constante (50, 59, 77):" << endl;
	aff(vec3);

	// Test de la soustraction d'une constante à un vecteur
	vec4 = vec2 - 8;
	cout << "* Un vecteur duquel on a soustrait une constante (34, 43, 61):" << endl;
	aff(vec4);

	// Test de la multiplication par une constante
	vec3 = vec2 * 3;
	cout << "* Un vecteur multiplié par une constante (126, 153, 207):" << endl;
	aff(vec3);
	vec3 = 3 * vec2;
	cout << "* Une constante multipliée par un vecteur (126, 153, 207):" << endl;
	aff(vec3);

	// Test de la division par une constante
	vec4 = vec2 / 3;
	cout << "* Un vecteur divisé par une constante (14, 17, 23):" << endl;
	aff(vec4);

	// Test de la négation d'un vecteur
	vec3 = -vec4;
	cout << "* L'opposé d'un vecteur (-14, -17, -23):" <<endl;
	aff(vec3);

	// Test de l'assignation/addition de deux vecteur
	vec1 += vec2;
	cout << "* Un vecteur additionné/assigné à un autre (54, 96, 147):" << endl;
	aff(vec1);
	
	// Test de l'assignation/soustraction de deux vecteur
	vec1 -= vec2;
	cout << "* Un vecteur soustrait/assigné à un autre (12, 45, 78):" << endl;
	aff(vec1);

	// Test de l'assignation/addition d'un vecteur et d'une constante
	vec2 += 8;
	cout << "* Un vecteur additionné/assigné d'une constante (50, 59, 77):" << endl;
	aff(vec2);
	
	// Test de l'assignation/soustraction d'un vecteur et d'une constante
	vec2 -= 8;
	cout << "* Un vecteur soustrait/assigné à un autre (42, 51, 69):" << endl;
	aff(vec2);

	// Test de l'assignation/multiplication par une constante
	vec2 *= 3;
	cout << "* Un vecteur multiplié/assigné par une constante (126, 153, 207):" << endl;
	aff(vec2);

	// Test de l'assignation/division par une constante
	vec2 /= 3;
	cout << "* Un vecteur divisé/assigné par une constante (42, 51, 69):" << endl;
	aff(vec2);
	
	// Test de l'affectation d'un vecteur dans un autre
	vec3 = vec2;
	cout << "* Un vecteur affecté d'un autre (42, 51, 69):" << endl;
	aff(vec3);
	
	// Test de l'affectation d'un tableau de 3 long
    vec3 = {11, 22, 33};
	cout << "* Un vecteur affecté d'un tableau (11, 22, 33):" << endl;
	aff(vec3);

	// Test de la récupération/modification par indices de tableau
	for(short i = 0; i < 3; i++)
		vec4[i] = vec3[i];
	cout << "* Un vecteur lu et modifié comme un tableau (11, 22, 33):" << endl;
	aff(vec4);

	// Test de la comparaison de deux vecteurs
	cout << "* Comparaison de deux vecteurs:" << endl;
	cout << "  - Vecteurs égaux (true):\t" << (vec3==vec4) << endl;
    vec4 = {11, 22, 34};
	cout << "  - Seule Z diffère (false):\t" << (vec3==vec4) << endl;
    vec4 = {11, 23, 33};
	cout << "  - Seule Y diffère (false):\t" << (vec3==vec4) << endl;
    vec4 = {12, 22, 33};
	cout << "  - Seule X diffère (false):\t" << (vec3==vec4) << endl;
    vec4 = {12, 23, 33};
	cout << "  - Seule Z correspond (false):\t" << (vec3==vec4) << endl;
    vec4 = {12, 22, 34};
	cout << "  - Seule Y correspond (false):\t" << (vec3==vec4) << endl;
    vec4 = {11, 23, 34};
	cout << "  - Seule X correspond (false):\t" << (vec3==vec4) << endl;
    vec4 = {12, 23, 34};
	cout << "  - Rien ne correspond (false):\t" << (vec3==vec4) << endl;

	// Test du calcul de la norme d'un vecteur
	cout << "* Norme d'un vecteur (41.158231254513353):" << endl;
	cout << "\t" << vec3.getNorme() << endl;

	// Test du calcul de la norme au carré d'un vecteur
	cout << "* Norme au carré d'un vecteur (1694):" << endl;
	cout << "\t" << vec3.getNorme2() << endl;

	// Test du calcul de la composante maxi d'un vecteur
    vec3 = {10, 25, -13};
	cout << "* Composante maxi d'un vecteur (25):" << endl;
	cout << "\t" << vec3.getMax() << endl;

	// Test du calcul de la composante mini d'un vecteur
	cout << "* Composante mini d'un vecteur (-13):" << endl;
	cout << "\t" << vec3.getMin() << endl;

	// Test du calcul de la somme des composantes d'un vecteur
	cout << "* Calcul de la somme des composantes d'un vecteur (22):" << endl;
	cout << "\t" << vec3.getSomme() << endl;

	// Test du calcul de la somme des valeurs absolues des composantes d'un vecteur
	cout << "* Calcul de la somme des valeurs absolues des composantes d'un vecteur (48):" << endl;
	cout << "\t" << vec3.getSommeAbs() << endl;

	// Test du calcul du produit des compsantes d'un vecteur
	cout << "* Calcul du produit des composantes d'un vecteur (-3250):" << endl;
	cout << "\t" << vec3.getProduit() << endl;

	// Test du calcul du produit des valeurs absolues des compsantes d'un vecteur
	cout << "* Calcul du produit des valeurs absolues des composantes d'un vecteur (3250):" << endl;
	cout << "\t" << vec3.getProduitAbs() << endl;


	// Test de la conversion d'un DVect en LVect
	DVect vec5(12.4, -53.8, 96.7);
	cout << "* Un vecteur de double (DVect):" << endl;
	cout << "\t" << vec5.getX() << "\t" << vec5.getY() << "\t" << vec5.getZ() << endl;
	vec4 = vec5;
	cout << "* Un vecteur double converti depuis un long (12, 53, 96):" << endl;
	aff(vec4);

	cout << endl;
	cout << "############################################################" << endl;
	cout << "######                       FIN                      ######" << endl;
	cout << "############################################################" << endl;
	cout << endl;
	return 0;
}
