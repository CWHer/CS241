#include "mainwindow.h"
#include "welcomewidget.h"

#include <QApplication>

int main(int argc, char *argv[]) {

    QString l_time = "11 Sep 2014 14:31:50";
    QDateTime l_dt = QDateTime::fromString(l_time, "dd MMM yyyy hh:mm:ss");
    uint l_timestamp = l_dt.toTimeSpec(Qt::UTC).toTime_t();

    auto i = 60;
    QString time_day = (i / 60 < 10 ? "0" : "") + QString::number(i / 60) + ":";
    time_day += (i / 60 < 10 ? "0" : "") + QString::number(i % 60) + ":00";
    QString time_str = QString::number(10) + " Sep 2016 " + time_day;

    time_str = "11 Sep 2014 14:31:50";
    QDateTime time_now = QDateTime::fromString(time_str, "dd MMM yyyy hh:mm:ss");
    //    QDateTime time_now = QLocale(QLocale::Italian, QLocale::Italy).toDateTime(time_str, "dd MMM yyyy hh:mm:ss");
    time_now.setTimeSpec(Qt::UTC);
    auto start_time = time_now.toTime_t();

    QApplication a(argc, argv);
    MainWindow w;
    w.setWelcome();
    //    w.switchWindow();
    //    w.setupLayouts();
    //    w.show();
    //    w.show();
    return a.exec();
}
