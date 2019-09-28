#include "VisSkeleton.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VisSkeleton w;
    w.show();
    return a.exec();
}
