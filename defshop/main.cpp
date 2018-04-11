#include <qapplication.h>
#include "defshopwin.h"

/// Fonction principale
/** Crée l'application Qt et la fenètre principale.
  * Lance l'application.
  */
int main(int argc, char** argv)
{
    QApplication appli(argc, argv);

    DefShopWin dwin;
    appli.setMainWidget( &dwin );
    dwin.show();
    return appli.exec();
}
