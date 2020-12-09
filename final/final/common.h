#ifndef COMMON_H
#define COMMON_H

#include <QChartView>
#include <QCheckBox>
#include <QDir>
#include <QErrorMessage>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QRegExp>
#include <QString>
#include <QTabWidget>
#include <QTextStream>
#include <QThread>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <memory>
#include <utility>
#include <vector>
QT_CHARTS_USE_NAMESPACE
using std::make_pair;
using std::make_shared;
using std::shared_ptr;
using std::vector;
using Pos = std::pair<double, double>;

static const auto DAY_NUM = 15;
struct tour {
    QString id;
    int start, end;
    Pos ori, dest;
    double fee;
};

static const auto GRID_NUM = 10;
struct grid {
    int id;
    Pos x1, x2, y1, y2;
};

#endif // COMMON_H
