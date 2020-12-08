#include "welcomewidget.h"

WelcomeWidget::WelcomeWidget(QWidget *parent)
    : QWidget(parent) {
    setupLayouts();
    setupConnects();
}
// ***********************
// todo: add background img and beautify
void WelcomeWidget::setupLayouts() {
    this->resize(WIDTH, HEIGHT);
    this->setWindowTitle("see it!");
    this->setFont(QFont("consolas", 8));
    QVBoxLayout *outerlayout = new QVBoxLayout(this);

    // top
    QHBoxLayout *toplayout = new QHBoxLayout();
    toplayout->addItem(new QSpacerItem(WIDTH / 5, HEIGHT / 3, QSizePolicy::Expanding, QSizePolicy::Expanding));
    QLabel *toplabel = new QLabel();
    toplabel->setText("visualize taxi data\n select data folder and begin");
    toplabel->setMinimumWidth(WIDTH);
    toplabel->setMinimumHeight(HEIGHT / 3);
    toplabel->setFont(QFont("consolas", 12));
    toplayout->addWidget(toplabel);
    toplayout->addItem(new QSpacerItem(WIDTH / 5, HEIGHT / 3, QSizePolicy::Expanding, QSizePolicy::Expanding));
    outerlayout->addLayout(toplayout);

    // mid
    QHBoxLayout *midlayout = new QHBoxLayout();
    midlayout->addItem(new QSpacerItem(WIDTH / 5, HEIGHT / 6, QSizePolicy::Expanding, QSizePolicy::Expanding));
    selectfolder = new QPushButton();
    selectfolder->setText("open folder");
    selectfolder->setMinimumWidth(WIDTH / 5);
    selectfolder->setMinimumHeight(HEIGHT / 6);
    selectfolder->setFont(QFont("consolas", 12));
    midlayout->addWidget(selectfolder);
    midlayout->addItem(new QSpacerItem(WIDTH / 5, HEIGHT / 6, QSizePolicy::Expanding, QSizePolicy::Expanding));
    outerlayout->addLayout(midlayout);

    // bottom
    QHBoxLayout *bottomlayout = new QHBoxLayout();
    bottomlayout->addItem(new QSpacerItem(WIDTH / 5, HEIGHT / 5, QSizePolicy::Expanding, QSizePolicy::Expanding));
    loadbar = new QProgressBar();
    loadbar->setRange(0, 100);
    loadbar->setValue(0);
    loadbar->setFormat("%p%");
    loadbar->setMinimumWidth(WIDTH / 5 * 3);
    loadbar->setMinimumHeight(WIDTH / 10);
    bottomlayout->addWidget(loadbar);
    bottomlayout->addItem(new QSpacerItem(WIDTH / 5, HEIGHT / 5, QSizePolicy::Expanding, QSizePolicy::Expanding));
    outerlayout->addLayout(bottomlayout);
}

void WelcomeWidget::setupConnects() {
    // connect button with get_path()
    connect(selectfolder, &QPushButton::clicked, this, &WelcomeWidget::getPath);
}

void WelcomeWidget::getPath() {
    // get folder paht and store in folder_path
    folder_path = QFileDialog::getExistingDirectory();
    if (!folder_path.isNull()) {
        loadData();
    }
}

// load data to database
// using another thread
void WelcomeWidget::loadData() {
    LoadThread *loader = new LoadThread;
    loader->init(folder_path, db);
    connect(loader, &LoadThread::setValue, this, &WelcomeWidget::setBarValue);
    connect(loader, &LoadThread::switchwindow, this, &WelcomeWidget::switchwindow);
    loader->start();
}

void WelcomeWidget::setBarValue(double value) {
    // change loadbar progress
    loadbar->setValue(value);
}
