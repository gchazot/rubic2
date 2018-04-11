#include <qapplication.h>
#include "imgshopwin.h"

/// Fonction principale de l'application imgshop
/** Crée l'application Qt et la fenètre principale de l'application imgshop
  * et lance l'application.
  */
int main(int argc, char** argv)
{
    QApplication appli(argc, argv);

    ImgShopWin iswin;
    appli.setMainWidget( &iswin );
    iswin.show();
    return appli.exec();
}
