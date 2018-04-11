#include <qstring.h>
#include <qregexp.h>

#include <iostream>

using namespace std;

/** Ce fichier de test permet de valider la reconnaisance des nombres par des
 *  expressions rationnelles.
 */
int main(int, char**)
{
	QString nb[3];
	nb[0] = QString("1234");
	nb[1] = QString("-65403");
	nb[2] = QString("+876");
	
	QString vi[3];
	vi[0] = QString("1234.57");	
	vi[1] = QString("-234.86");
	vi[2] = QString("+287.65");
	
	QString ex[36];
	ex[0] = QString("34.62e5");
	ex[1] = QString("+34.62e5");
	ex[2] = QString("-34.62e5");
	ex[3] = QString("765e67");
	ex[4] = QString("-765e67");
	ex[5] = QString("+765e67");
	ex[6] = QString("765.e67");
	ex[7] = QString("-765.e67");
	ex[8] = QString("+765.e67");
	ex[9] = QString(".345e67");
	ex[10] = QString("-.345e67");
	ex[11] = QString("+.345e67");
	ex[12] = QString("34.62e+5");
	ex[13] = QString("+34.62e+5");
	ex[14] = QString("-34.62e+5");
	ex[15] = QString("765e+67");
	ex[16] = QString("-765e+67");
	ex[17] = QString("+765e+67");
	ex[18] = QString("765.e+67");
	ex[19] = QString("-765.e+67");
	ex[20] = QString("+765.e+67");
	ex[21] = QString(".345e+67");
	ex[22] = QString("-.345e+67");
	ex[23] = QString("+.345e-67");
	ex[24] = QString("34.62e-5");
	ex[25] = QString("+34.62e-5");
	ex[26] = QString("-34.62e-5");
	ex[27] = QString("765e-67");
	ex[28] = QString("-765e-67");
	ex[29] = QString("+765e-67");
	ex[30] = QString("765.e-67");
	ex[31] = QString("-765.e-67");
	ex[32] = QString("+765.e-67");
	ex[33] = QString(".345e-67");
	ex[34] = QString("-.345e-67");
	ex[35] = QString("+.345e-67");
	
	QString nan[13];
	nan[0] = QString("23 45");
	nan[1] = QString("-23 45");
	nan[2] = QString("+23 45");
	nan[3] = QString("-23e 45");
	nan[4] = QString("-23e 45");
	nan[5] = QString("-3e+-645");
	nan[6] = QString("-2+3e45");
	nan[7] = QString("-23e4+5");
	nan[8] = QString("-+23e45");
	nan[9] = QString("+-23e45");
	nan[10] = QString("-23e-+45");
	nan[11] = QString("23e-");
	nan[12] = QString("");
	
	QRegExp entier("^[+-]?[0-9]+$");
	QRegExp virgule("^[+-]?([0-9]+[.]|[.][0-9]+|[0-9]+[.][0-9]+)$");
	QRegExp expon("^[+-]?([0-9]+[.]|[.][0-9]+|[0-9]+[.][0-9]+|[0-9]+)[eE][+-]?[0-9]+$");
	
	
	cout << "###### ENTIERS ######" << endl;
	cout << "------   nb    ------" << endl;
	for(int i = 0; i <3; i++)
		cout << nb[i].toStdString() << "  \t" << nb[i].contains(entier) << endl;
	cout << "------   vi    ------" << endl;
	for(int i = 0; i <3; i++)
		cout << vi[i].toStdString() << "  \t" << vi[i].contains(entier) << endl;
	cout << "------   ex    ------" << endl;
	for(int i = 0; i <36; i++)
		cout << ex[i].toStdString() << "  \t" << ex[i].contains(entier) << endl;
	cout << "------   nan   ------" << endl;
	for(int i = 0; i <13; i++)
		cout << nan[i].toStdString() << "  \t" << nan[i].contains(entier) << endl;
	cout << endl;
	
	cout << "###### VIRGULE ######" << endl;
	cout << "------   nb    ------" << endl;
	for(int i = 0; i <3; i++)
		cout << nb[i].toStdString() << "  \t" << nb[i].contains(virgule) << endl;
	cout << "------   vi    ------" << endl;
	for(int i = 0; i <3; i++)
		cout << vi[i].toStdString() << "  \t" << vi[i].contains(virgule) << endl;
	cout << "------   ex    ------" << endl;
	for(int i = 0; i <36; i++)
		cout << ex[i].toStdString() << "  \t" << ex[i].contains(virgule) << endl;
	cout << "------   nan   ------" << endl;
	for(int i = 0; i <13; i++)
		cout << nan[i].toStdString() << "  \t" << nan[i].contains(virgule) << endl;
	cout << endl;
	
	cout << "### EXPONENTIELLE ###" << endl;
	cout << "------   nb    ------" << endl;
	for(int i = 0; i <3; i++)
		cout << nb[i].toStdString() << "  \t" << nb[i].contains(expon) << endl;
	cout << "------   vi    ------" << endl;
	for(int i = 0; i <3; i++)
		cout << vi[i].toStdString() << "  \t" << vi[i].contains(expon) << endl;
	cout << "------   ex    ------" << endl;
	for(int i = 0; i <36; i++)
		cout << ex[i].toStdString() << "  \t" << ex[i].contains(expon) << endl;
	cout << "------   nan   ------" << endl;
	for(int i = 0; i <13; i++)
		cout << nan[i].toStdString() << "  \t" << nan[i].contains(expon) << endl;
	cout << endl;
	
	return 0;
}
