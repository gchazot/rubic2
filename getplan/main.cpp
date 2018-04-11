#include <qapplication.h>
#include "getplanwin.h"

/// Fonction principale
/** Crée l'application Qt et la fenètre principale.
  * Lance l'application.
  */
int main(int argc, char** argv)
{
    QApplication appli(argc, argv);

    GetPlanWin gpwin;
    appli.setMainWidget( &gpwin );
    gpwin.show();
    return appli.exec();
}
