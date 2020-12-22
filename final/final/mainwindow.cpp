#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    db = new DataBase();
}

void MainWindow::setWelcome() {
    welcome = new WelcomeWidget();
    welcome->db = db;
    welcome->setupLayouts();
    welcome->setupConnects();
    welcome->show();
    connect(welcome, &WelcomeWidget::switchWindow, this, &MainWindow::switchWindow);
}

void MainWindow::setupLayouts() {
    QFont font = QFont("consolas", 10);
    this->setFixedSize(WIDTH, HEIGHT);
    this->setWindowTitle("Analysis and Visualization of Online Ride-Hailing Order Data");
    this->setFont(font);
    this->setWindowIcon(QIcon("../taxi.svg"));

    // centralwidget begin
    centralwidget = new QTabWidget();

    spatialplot = new SpatialPlot(WIDTH, HEIGHT, db, QThread::currentThread());
    //    connect(spatialplot, &SpatialPlot::resetPlot, this, &MainWindow::resetPlot);
    spatialplot->setupLayouts();
    spatialplot->setupConnects();
    centralwidget->addTab(spatialplot->main_widget, "SpatialMap");

    timeplot = new TimePlot(WIDTH, HEIGHT, db, QThread::currentThread());
    //    connect(timeplot, &TimePlot::resetPlot, this, &MainWindow::resetPlot);
    timeplot->setupLayouts();
    timeplot->setupConnects();
    centralwidget->addTab(timeplot->main_widget, "Order-Time");

    infoplot = new InfoPlot(WIDTH, HEIGHT, db, QThread::currentThread());
    //    connect(infoplot, &InfoPlot::resetPlot, this, &MainWindow::resetPlot);
    infoplot->setupLayouts();
    infoplot->setupConnects();
    centralwidget->addTab(infoplot->main_widget, "Infomation");

    routeplan = new RoutePlan(WIDTH, HEIGHT, db, QThread::currentThread());
    routeplan->setupLayouts();
    routeplan->setupConnects();
    centralwidget->addTab(routeplan->main_widget, "Route Plan");

    this->setCentralWidget(centralwidget);
    // centralwidget end
}

void MainWindow::setupConnects() {

    //
    //
    //
}

void MainWindow::switchWindow() {
    delete welcome;
    setupLayouts();
    setupConnects();
    show();
};

// void MainWindow::resetPlot(xxxPlot *painter) {
//    //    db->moveToThread(QThread::currentThread());
//    //    painter->moveToThread(QThread::currentThread());
//}
