#include <qapplication.h>
#include "visupropawin.h"

int main(int argc, char** argv)
{
    QApplication appli(argc, argv);

    VisuPropaWin vpwin;
    appli.setMainWidget( &vpwin );
    vpwin.show();
    return appli.exec();
}
