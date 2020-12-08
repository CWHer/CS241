#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    db = new DataBase();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::setWelcome() {
    welcome = new WelcomeWidget();
    welcome->db = db;
    welcome->show();
    connect(welcome, &WelcomeWidget::switchwindow, this, &MainWindow::switchwindow);
}

void MainWindow::setupLayouts() {
    //
    //
    //
}

void MainWindow::setupConnects() {

    //
    //
    //
}
void MainWindow::switchwindow() {
    delete welcome;
    show();
};
