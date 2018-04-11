#include <iostream>
#include <cmath>

using namespace std;

float** matrix(float mat[3][3])
{
	float** tmp = new float*[3];
	if(tmp == NULL)
		return NULL;
	
	for(int i = 0; i<3; i++)
	{
		tmp[i] = new float[3];
		if(tmp[i] == NULL)
		{
			for(int j = i-1; i>=0; i--)
				delete [] tmp[j];
			return NULL;
		}
	}
				
	
	for(int i = 0; i<3; i++)
		for(int j = 0; j<3; j++)
			tmp[i][j] = mat[i][j];
	
	return tmp;
}

float* vector(float vec[3])
{
	float* tmp = new float[3];
	if(tmp == NULL)
		return NULL;
	
	for(int i = 0; i<3; i++)
		tmp[i] = vec[i];
	
	return tmp;
}

void delmatrix(float** mat)
{
	for(int i = 0; i<3; i++)
		delete [] mat[i];
	delete [] mat;
}

void delvector(float* vec)
{
	delete [] vec;
}

// Macro effectuant une rotation pour la fonction jacobi
#define ROTATE(a,i,j,k,l) \
	{ \
		g = a[i][j]; \
		h = a[k][l]; \
		a[i][j] = g - s * (h + g * tau); \
		a[k][l] = h + s * (g - h * tau); \
	}

bool jacobi(float **a, float d[], float **v, int *nrot)
{
	int j, ip, iq, i;
	float tresh, theta, tau, t, sm, s, h, g, c;
	float b[3];
	float z[3];
	
	// On initialise v à la matrice identité
	for(ip = 0; ip < 3; ip++)
	{
		for(iq = 0; iq < 3; iq++)
			v[ip][iq] = 0.0;
		v[ip][ip] = 1.0;
	}
	
	// On initialise b et d avec la diagonale de a
	for(ip = 0; ip < 3; ip++)
	{
		b[ip] = d[ip] = a[ip][ip];
		z[ip] = 0.0;
	}
	
	*nrot = 0;
	for( i = 0; i < 50; i++)
	{
		// Somme des élements hors-diagonale
		sm = 0.0;
		for(ip=0; ip<2; ip++)
			for(iq=ip+1; iq<3; iq++)
				sm += fabs(a[ip][iq]);
		
		// Fin normale de la fonction, qui suppose une convergence
		// quadratique vers l'underflow
		if( sm == 0.0 )	// TODO: A corriger si l'underflow ne renvoie pas 0.0
			return true;
		
		if(i < 3)	// Pour les 3 premières itérations ...
			tresh = 0.2 * sm / 9.0;
		else		// ... Pour les autres
			tresh = 0.0;
		
		for(ip=0; ip<2; ip++)
		{
			for(iq=ip+1; iq<3; iq++)
			{
				g = 100.0 * fabs(a[ip][iq]);
				// Après 4 itérations, on passe la rotation si l'élement hors-diagonale est faible
				if(i > 3 && (float)(fabs(d[ip])+g) == (float)fabs(d[ip])
						&& (float)(fabs(d[iq])+g) == (float)fabs(d[iq]))
					a[ip][iq] = 0.0;
				else if (fabs(a[ip][iq]) > tresh)
				{
					h=d[iq]-d[ip];
					if((float)(fabs(h)+g) == (float)(fabs(h)))
						t = (a[ip][iq]) / h; //  t = 1/(2*theta)
					else
					{
						theta = 0.5 * h / (a[ip][iq]);
						t = 1.0/(fabs(theta)+sqrt(1.0+theta*theta));
						if(theta < 0.0)
							t = -t;
					}
					c = 1.0 / sqrt(1+t*t);
					s = t*c;
					tau = s/(1.0+c);
					h = t*a[ip][iq];
					z[ip] -= h;
					z[iq] += h;
					d[ip] -= h;
					d[iq] += h;
					a[ip][iq] = 0.0;
					
					for(j = 0; j <= ip-1; j++)
						ROTATE(a, j, ip, j, iq)
						
					for(j = ip+1; j<=iq-1; j++)
						ROTATE(a, ip, j, j, iq)
					
					for(j = iq+1; j<3; j++)
						ROTATE(a, ip, j, iq, j)
					
					for(j = 0; j < 3; j++)
						ROTATE(v, j, ip, j, iq)
					
					++(*nrot);
				}
			}
		}
		for(ip = 0; ip < 3; ip++)
		{
			b[ip] += z[ip];
			d[ip] = b[ip];
			z[ip] = 0.0;
		}
	}
	return false;
}

int main(int, char**)
{
	float a[3][3] = {{1.0,2.0,3.0},{2.0,4.0,5.0},{3.0,5.0,6.0}};
	float b[3][3] = {{0.0,0.0,0.0},{0.0,0.0,0.0},{0.0,0.0,0.0}};
	float c[3] = {0.0,0.0,0.0};
	
	float** M = matrix(a);
	float** vec = matrix(b);
	float*  val = vector(c);
	
	int rot;
	
	if( jacobi(M, val, vec, &rot) )
	{
		cout << "Calcul réussi : " << endl;
		cout << "Valeurs Propres : " << val[0] << "\t" << val[1] << "\t" << val[2] << endl;
		cout << "Matrice des vecteurs propres :" << endl;
		cout << vec[0][0] << "\t" << vec[0][1] << "\t" << vec[0][2] << endl;
		cout << vec[1][0] << "\t" << vec[1][1] << "\t" << vec[1][2] << endl;
		cout << vec[2][0] << "\t" << vec[2][1] << "\t" << vec[2][2] << endl;
	}
	else
	{
		cout << "Calcul échoué !" << endl;
	}
	
	delmatrix(M);
	delmatrix(vec);
	delvector(val);
	
	return 0;
}
