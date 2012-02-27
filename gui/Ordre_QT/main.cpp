
#include <QApplication>
#include <QtGui>
#include "FenAff.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);


	FenAff fenetre;
    fenetre.show();

    return app.exec();
}
