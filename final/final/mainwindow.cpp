#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    db = new DataBase();
}

MainWindow::~MainWindow() {
    //
    delete ui;
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

    timeplot = new TimePlot(WIDTH, HEIGHT);
    timeplot->db = db;
    connect(this, &MainWindow::resetPlot, timeplot, &TimePlot::plotDone);
    timeplot->setupLayouts();
    timeplot->setupConnects();
    centralwidget->addTab(timeplot->main_widget, "Data-Time");

    infoplot = new InfoPlot(WIDTH, HEIGHT);
    infoplot->db = db;
    connect(this, &MainWindow::resetPlot, infoplot, &InfoPlot::plotDone);
    infoplot->setupLayouts();
    centralwidget->addTab(infoplot->main_widget, "Infomation");

    spatialplot = new SpatialPlot(WIDTH, HEIGHT);
    spatialplot->db = db;
    connect(this, &MainWindow::resetPlot, spatialplot, &SpatialPlot::plotDone);
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
    db->moveToThread(QThread::currentThread());
    painter->moveToThread(QThread::currentThread());
    show();
}
