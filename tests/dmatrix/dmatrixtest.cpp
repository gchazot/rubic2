#include "dmatrix.h"
#include "dvect.h"
#include "lvect.h"
#include "constants.h"

#include <iostream>

#include <qstring.h>

using namespace std;

int main(int, char**)
{
	short i,j;
	DMatrix mat1, mat2, mat3, mat4;
	DVect vec1, vec3;
	LVect vec2;
	
	const double tab1[3][3] = {{12.0,6.0,1.0},{6.0,4.0,2.0},{1.0,2.0,5.0}};
	const double tab2[3][3] = {{1.0,2.0,3.0},{4.0,5.0,6.0},{7.0,8.0,9.0}};
	
	const double v1[3] = {1.2,3.4,5.6};
	const long v2[3] = {6,-8, 4};
	
	cout << "Matrice identité :" << endl;
	mat3 = DMatrix::Identite();
	cout << mat3.toString().toStdString() << endl;
	
	cout << "Matrice 1:" << endl;
	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			mat1[i][j] = tab1[i][j];
	cout << mat1.toString().toStdString() << endl;
	
	cout << "Matrice 2:" << endl;
	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			mat2[i][j] = tab2[i][j];
	cout << mat2.toString().toStdString() << endl;
	
	cout << "Somme de la matrice 1 et de la matrice 2 :" << endl;
	mat3 = mat1 + mat2;
	cout << mat3.toString().toStdString() << endl;
	
	cout << "Différence de la matrice 1 moins la matrice 2 :" << endl;
	mat3 = mat1 - mat2;
	cout << mat3.toString().toStdString() << endl;
	
	cout << "Produit de la matrice 1 et de la matrice 2 :" << endl;
	mat3 = mat1 * mat2;
	cout << mat3.toString().toStdString() << endl;
	
	cout << "Produit de la matrice 1 par 2.5 :" << endl;
	mat3 = mat1 * 2.5;
	cout << mat3.toString().toStdString() << endl;
	
	cout << "Produit de 2.5 par la matrice 1:" << endl;
	mat3 = 2.5 * mat1;
	cout << mat3.toString().toStdString() << endl;
	
	cout << "Division de la matrice 1 par 2.5 :" << endl;
	mat3 = mat1 / 2.5;
	cout << mat3.toString().toStdString() << endl;
	
	cout << "=============================================" << endl;
	cout << "=============================================" << endl;
	
	cout << "L'inverse de la matrice 1:" << endl;
	mat3 = mat1.inverse();
	cout << mat3.toString().toStdString() << endl;
	
	cout << "La transposée de la matrice 2:" << endl;
	mat3 = mat2.transpose();
	cout << mat3.toString().toStdString() << endl;
	
	cout << "La diagonale de la matrice 1 :" << endl;
	vec2 = mat1.getDiagonal();
	cout << vec2.toString().toStdString() << endl;
	
	cout << "Diagonalisons la matrice 1: " << endl;
	mat1.diagonalise( mat3, mat4 );
	cout << "  * La matrice diagonale :" << endl;
	cout << mat3.toString().toStdString() << endl;
	cout << "  * La matrice de passage :" << endl;
	cout << mat4.toString().toStdString() << endl;
	
	cout << "La trace de la matrice 1 :" << endl;
	cout << "\t" << mat1.getTrace() << endl;
	
	cout << "=============================================" << endl;
	cout << "=============================================" << endl;	
	
	cout << "Le vecteur 1 (double) :" << endl;
	for(i=0; i<3; i++)
		vec1[i] = v1[i];
	cout << vec1.toString().toStdString() << endl;
	
	cout << "Le vecteur 2 (long) :" << endl;
	for(i=0; i<3; i++)
		vec2[i] = v2[i];
	cout << vec2.toString().toStdString() << endl;
	
	cout << "Produit de la matrice 1 par le vecteur 1 :" << endl;
	cout << "  * 1ere solution : mat1 * vec1" << endl;
	vec3 = mat1 * vec1;
	cout << vec3.toString().toStdString() << endl;
	cout << "  * 2eme solution : mat1(vec1)" << endl;
	vec3 = mat1(vec1);
	cout << vec3.toString().toStdString() << endl;
	
	cout << "Produit de la matrice 1 par le vecteur 21 :" << endl;
	cout << "  * 1ere solution : mat1 * vec2" << endl;
	vec3 = mat1 * vec2;
	cout << vec3.toString().toStdString() << endl;
	cout << "  * 2eme solution : mat1(vec2)" << endl;
	vec3 = mat1(vec2);
	cout << vec3.toString().toStdString() << endl;
	
	
	return 0;
}
