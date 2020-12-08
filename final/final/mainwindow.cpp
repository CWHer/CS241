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

// data filter + graph
//  + top switchbar
void MainWindow::setupLayouts() {
    QFont font = QFont("consolas", 10);
    this->resize(WIDTH, HEIGHT);
    this->setWindowTitle("see it");
    this->setFont(font);

    centralwidget = new QWidget();
    QHBoxLayout *outer_layout = new QHBoxLayout(centralwidget);
    // left side
    // data filter
    QTreeWidget *left_side = new QTreeWidget();
    left_side->setHeaderLabel("selector");
    left_side->setFont(font);
    // add date
    QTreeWidgetItem *date = new QTreeWidgetItem(left_side);
    date->setText(0, "date");
    date->setFont(0, font);
    date->setFlags(date->flags() | Qt::ItemIsAutoTristate);
    for (int i = 0; i < 10; ++i) {
        auto ch = new QTreeWidgetItem(date);
        ch->setText(0, "QString(i)");
        ch->setFont(0, font);
        ch->setCheckState(0, Qt::Unchecked);
    }
    // add item
    QTreeWidgetItem *item = new QTreeWidgetItem(left_side);
    item->setText(0, "item");
    item->setFont(0, font);
    item->setFlags(date->flags() | Qt::ItemIsAutoTristate);
    for (int i = 0; i < 10; ++i) {
        auto ch = new QTreeWidgetItem(item);
        ch->setText(0, "QString(i)");
        ch->setFont(0, font);
        ch->setCheckState(0, Qt::Unchecked);
    }

    left_side->setMinimumWidth(WIDTH / 5);
    left_side->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    outer_layout->addWidget(left_side);

    // right side
    // graph and operations
    outer_layout->addLayout(mainlayout);

    // top switchbar

    this->setCentralWidget(centralwidget);
}

void MainWindow::setSpaticalPlot() {}

void MainWindow::setTimePlot() {}

void MainWindow::setInfoPlot() {}

void MainWindow::setupConnects() {

    //
    //
    //
}

void MainWindow::switchwindow() {
    delete welcome;
    show();
};
