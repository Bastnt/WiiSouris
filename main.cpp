#include <QApplication>
#include <windows.h>
#include "FenetrePrincipale.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    Fenetre mafen;
    mafen.show();


    return app.exec();
}
