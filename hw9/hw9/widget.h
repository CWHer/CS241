#ifndef WIDGET_H
#define WIDGET_H

#include <QChart>
#include <QChartView>
#include <QComboBox>
#include <QErrorMessage>
#include <QFileDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineSeries>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QSplineSeries>
#include <QTextStream>
#include <QVBoxLayout>
#include <algorithm>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class Widget : public QWidget {
    Q_OBJECT
    static const auto TOTLE_HOURS = 24;

  public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void setupLayouts();
    void setupConnects();
    void getFileName();
    void PlotData();

  private:
    int demands_data[TOTLE_HOURS];
    Ui::Widget *ui;
    QWidget *centralwidget;
    QGroupBox *groupBox;
    QHBoxLayout *outerlayout;
    QVBoxLayout *left_verticalLayout;
    QHBoxLayout *start_layout;
    QLabel *start_label;
    QComboBox *start_com;
    QSpacerItem *start_spacer;
    QHBoxLayout *end_layout;
    QLabel *end_label;
    QComboBox *end_com;
    QSpacerItem *end_spacer;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *select_layout;
    QPushButton *select_button;
    QPushButton *plot_button;
    QSpacerItem *select_spacer;
    QChartView *view;
    bool loaded;
};
#endif // WIDGET_H
