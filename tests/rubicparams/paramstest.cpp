#include "rubicparams.h"

#include <iostream>
using namespace std;

#include <qstring.h>

int main(int, char**)
{
  RubicParams params;
  QString text("test.params");
  params.setCmdFileName(text);

  text = "RefBidon.vgi";
  params.setVgiRefFileName(text);
  text = "DefBidon.vgi";
  params.setVgiDefFileName(text);
  text = "ResultatsBidon.inp";
  params.setInpOutputFileName(text);

  if(params.writeCommandFile())
     cout << "OK" << endl;
  else
    cout << "ERREUR" << endl;

  return 0;
}


