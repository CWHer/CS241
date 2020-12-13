#include "mainwindow.h"
#include "welcomewidget.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.setWelcome();
    //    w.switchWindow();
    //    w.setupLayouts();
    //    w.show();
    //    w.show();
    return a.exec();
}
