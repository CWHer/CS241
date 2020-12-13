#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    db = new DataBase();
}

void MainWindow::setWelcome() {
    welcome = new WelcomeWidget();
    welcome->db = db;
    welcome->show();
    connect(welcome, &WelcomeWidget::switchWindow, this, &MainWindow::switchWindow);
}

void MainWindow::setupLayouts() {
    QFont font = QFont("consolas", 10);
    this->resize(WIDTH, HEIGHT);
    this->setWindowTitle("see it");
    this->setFont(font);

    // centralwidget begin
    centralwidget = new QTabWidget();

    timeplot = new TimePlot(WIDTH, HEIGHT, db, QThread::currentThread());
    connect(timeplot, &TimePlot::resetPlot, this, &MainWindow::resetPlot);
    timeplot->setupLayouts();
    timeplot->setupConnects();
    centralwidget->addTab(timeplot->main_widget, "Data-Time");

    infoplot = new InfoPlot(WIDTH, HEIGHT, db, QThread::currentThread());
    connect(infoplot, &InfoPlot::resetPlot, this, &MainWindow::resetPlot);
    infoplot->setupLayouts();
    infoplot->setupConnects();
    centralwidget->addTab(infoplot->main_widget, "Infomation");

    spatialplot = new SpatialPlot(WIDTH, HEIGHT, db, QThread::currentThread());
    connect(spatialplot, &SpatialPlot::resetPlot, this, &MainWindow::resetPlot);
    spatialplot->setupLayouts();
    centralwidget->addTab(spatialplot->main_widget, "SpatialMap");

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

void MainWindow::resetPlot(xxxPlot *painter) {
    //    db->moveToThread(QThread::currentThread());
    //    painter->moveToThread(QThread::currentThread());
}
