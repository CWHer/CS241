#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setupLayouts();
    w.setupConnects();
    w.show();
    return a.exec();
}
