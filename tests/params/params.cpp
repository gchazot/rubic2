#include "params.h"
#include <qstring.h>

#include <iostream>

using namespace std;

int main(int, char**)
{
	// On crée un objet Params
	Params params;
	cout << "Objet Params créé." << endl;
	// On y ajoute des valeurs
	params.insertShort("group1", "shortval", 178);
	params.insertInt("group1", "intval", 172);
	params.insertLong("group1", "longval", 178l);
	params.insertFloat("group1", "floatval", 172.34);
	params.insertDouble("group1", "doubleval", 172.456567);
	params.insertBool("group1", "trueval", true);
	params.insertBool("group1", "falseval", false);
	params.insertString("group1", "stringval", "c:clamerde");
	
	params.insertShort("group2", "shortval", 278);
	params.insertInt("group2", "intval", 272);
	params.insertLong("group2", "longval", 278l);
	params.insertFloat("group2", "floatval", 272.34);
	params.insertDouble("group2", "doubleval", 272.456567);
	params.insertBool("group2", "trueval", true);
	params.insertBool("group2", "falseval", false);
	params.insertString("group2", "stringval", "c:ctlamerde");
	
	cout << "Valeurs ajoutées à l'objet Params" << endl;
	
	if(params.writeToFile("test.param", true))
		cout << "Fichier test.param écrit" << endl;
	else
	{
		cout << "Erreur en écrivant dans test.param" << endl;
		return 1;
	}
	
	cout << endl;
	
	Params params2;
	cout << "Nouvel objet Params créé." << endl;
	if(params2.readFromFile("test.param"))
		cout << "Objet initialisé d'après test.param" << endl;
	else
	{
		cout << "Erreur en iinitialisant l'objet d'après test.param" << endl;
		return 1;
	}
	
	if(params2.writeToFile("test2.param", true))
		cout << "Fichier test2.param écrit" << endl;
	else
	{
		cout << "Erreur en écrivant dans test2.param" << endl;
		return 1;
	}
	
	short sh;
	int in;
	long lo;
	float fl;
	double db;
	bool bo1, bo2;
	QString str;
	
	cout << "Lecture des paramètres du group1" << endl;
	if(
	params.getShort("group1", "shortval", &sh)	&& 
	params.getInt("group1", "intval", &in)		&&
	params.getLong("group1", "longval", &lo)	&&
	params.getFloat("group1", "floatval", &fl)	&&
	params.getDouble("group1", "doubleval", &db)	&&
	params.getBool("group1", "trueval", &bo1)	&&
	params.getBool("group1", "falseval", &bo2)	&&
	params.getString("group1", "stringval", &str)
	)
		cout <<  "Lecture OK" << endl;
	else
		cout << "Erreur Lecture" << endl;
	cout << "sh " << sh << endl;
	cout << "in " << in << endl;
	cout << "lo " << lo << endl;
	cout << "fl " << QString::number(fl, 'e', 6).toStdString() << endl;
	cout << "db " << QString::number(db, 'e', 10).toStdString() << endl;
	cout << "bo1 " << bo1 << endl;
	cout << "bo2 " << bo2 << endl;
	cout << "str " << str.toStdString() << endl;
	
	cout << endl;
	cout << "Lecture des paramètres du group2" << endl;
	if(
	params.getShort("group2", "shortval", &sh)	&& 
	params.getInt("group2", "intval", &in)		&&
	params.getLong("group2", "longval", &lo)	&&
	params.getFloat("group2", "floatval", &fl)	&&
	params.getDouble("group2", "doubleval", &db)	&&
	params.getBool("group2", "trueval", &bo1)	&&
	params.getBool("group2", "falseval", &bo2)	&&
	params.getString("group2", "stringval", &str)
	)
		cout <<  "Lecture OK" << endl;
	else
		cout << "Erreur Lecture" << endl;
	cout << "sh " << sh << endl;
	cout << "in " << in << endl;
	cout << "lo " << lo << endl;
	cout << "fl " << QString::number(fl, 'e', 6).toStdString() << endl;
	cout << "db " << QString::number(db, 'e', 10).toStdString() << endl;
	cout << "bo1 " << bo1 << endl;
	cout << "bo2 " << bo2 << endl;
	cout << "str " << str.toStdString() << endl;

	return 0;
}
