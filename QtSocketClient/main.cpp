#include "QSocketClient.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSocketClient w;
    w.show();

    return a.exec();
}
