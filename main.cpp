#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include "appresource.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AppResource::initAppRes();

    XMainWindow w;
    w.show();

    return a.exec();
}
