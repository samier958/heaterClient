#include "cheaterclient.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CHeaterClient heaterClient;
    heaterClient.show();

    return a.exec();
}
