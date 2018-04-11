#include <qapplication.h>
#include "smoothimgwin.h"

/// Fonction principale
/** Crée l'application Qt et la fenètre principale.
  * Lance l'application.
  */
int main(int argc, char** argv)
{
    QApplication appli(argc, argv);

    SmoothImgWin siwin;
    appli.setMainWidget( &siwin );
    siwin.show();
    return appli.exec();
}
