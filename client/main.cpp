#include "rcs_client.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RcsClient w;
    w.show();
    return a.exec();
}
