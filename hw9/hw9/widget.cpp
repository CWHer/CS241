#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget) {
    loaded = false;
    ui->setupUi(this);
}

Widget::~Widget() { delete ui; }

void Widget::setupLayouts()

{
    if (this->objectName().isEmpty()) this->setObjectName(QString::fromUtf8("MainWindow"));
    int init_w = 1000, init_h = 600;
    this->resize(init_w, init_h);
    outerlayout = new QHBoxLayout(this);

    //    part1
    //    left part
    left_verticalLayout = new QVBoxLayout();
    left_verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));

    //    layout of start time
    start_layout = new QHBoxLayout();
    start_layout->setObjectName(QString::fromUtf8("start_layout"));

    start_label = new QLabel();
    start_label->setObjectName(QString::fromUtf8("start_label"));
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(start_label->sizePolicy().hasHeightForWidth());
    start_label->setSizePolicy(sizePolicy);
    start_label->setMinimumWidth(100);
    start_layout->addWidget(start_label);

    start_com = new QComboBox();
    start_com->setObjectName(QString::fromUtf8("start_com"));
    for (int i = 0; i < TOTLE_HOURS; ++i)
        start_com->addItem(QString::number(i));
    start_com->setMinimumWidth(80);
    start_layout->addWidget(start_com);

    start_spacer = new QSpacerItem(100, 80, QSizePolicy::Expanding, QSizePolicy::Minimum);
    start_layout->addItem(start_spacer);

    left_verticalLayout->addLayout(start_layout);

    //    layout of end time
    end_layout = new QHBoxLayout();
    end_layout->setObjectName(QString::fromUtf8("end_layout"));
    end_label = new QLabel();
    end_label->setObjectName(QString::fromUtf8("end_label"));
    sizePolicy.setHeightForWidth(end_label->sizePolicy().hasHeightForWidth());
    end_label->setSizePolicy(sizePolicy);
    end_label->setMinimumWidth(100);
    end_layout->addWidget(end_label);

    end_com = new QComboBox();
    end_com->setObjectName(QString::fromUtf8("end_com"));
    for (int i = 0; i < TOTLE_HOURS; ++i)
        end_com->addItem(QString::number(i));
    end_com->setMinimumWidth(80);
    end_layout->addWidget(end_com);

    end_spacer = new QSpacerItem(100, 80, QSizePolicy::Expanding, QSizePolicy::Minimum);
    end_layout->addItem(end_spacer);

    left_verticalLayout->addLayout(end_layout);

    //    spacer bar
    verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    left_verticalLayout->addItem(verticalSpacer);

    //    select button
    select_layout = new QHBoxLayout();
    select_layout->setObjectName(QString::fromUtf8("select_layout"));

    select_button = new QPushButton();
    select_button->setObjectName(QString::fromUtf8("select_button"));
    select_layout->addWidget(select_button);

    //    plot button
    plot_button = new QPushButton();
    select_layout->addWidget(plot_button);

    select_spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    select_layout->addItem(select_spacer);

    left_verticalLayout->addLayout(select_layout);
    //    left_verticalLayout->SetMinimumSize(100, 50);

    outerlayout->addLayout(left_verticalLayout);

    //    part2
    //    right part
    view = new QChartView();
    view->setMinimumSize(init_w * 3 / 4, init_h);
    view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    outerlayout->addWidget(view);

    //    set texts
    this->setWindowTitle("taxi demands in Chengdu");
    start_label->setText("start with");
    end_label->setText("end with");
    select_button->setText("select .csv file");
    plot_button->setText("plot data");
}

void Widget::getFileName() {
    QString file_name = QFileDialog::getOpenFileName();
    if (file_name.isNull()) return;
    // read data
    QFile file(file_name);
    if (!file.open(QFile::ReadOnly)) return;
    QTextStream fin(&file);
    QString temp;
    fin >> temp;
    for (int i = 0; i < TOTLE_HOURS; ++i) {
        int x;
        char ch;
        fin >> x >> ch;
        fin >> demands_data[i];
    }
    loaded = true;
}
void Widget::PlotData() {
    if (!loaded) {
        QErrorMessage *error = new QErrorMessage();
        error->showMessage("file hasn't been loaded!");
        return;
    }
    int start_time = start_com->currentText().toInt();
    int end_time = end_com->currentText().toInt();
    if (start_time >= end_time) {
        QErrorMessage *error = new QErrorMessage();
        error->showMessage("start must be less than end");
        return;
    }

    //    plot data using qchart
    QLineSeries *line = new QLineSeries();
    for (int i = start_time; i <= end_time; ++i)
        line->append(i, demands_data[i]);

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(line);
    chart->createDefaultAxes();

    view->setChart(chart);
    this->show();
}
void Widget::setupConnects() {
    connect(select_button, &QPushButton::clicked, this, &Widget::getFileName);
    connect(plot_button, &QPushButton::clicked, this, &Widget::PlotData);
}
