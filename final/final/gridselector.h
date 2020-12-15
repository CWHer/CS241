#ifndef GRIDSELECTOR_H
#define GRIDSELECTOR_H

#include "common.h"

class GridSelector : public QDialog {
    Q_OBJECT
  public:
    GridSelector(QWidget *parent = nullptr);
    // store grid chosen
    vector<int> grid_id;
    void setupLayouts();
    void setupConnects();
    void selectGrid();
    void paintEvent(QPaintEvent *) override;

  private:
    vector<QCheckBox *> button_group;
    QPushButton *apply_button;
    static const int WIDTH = 800;
    static const int HEIGHT = 840;
    static const int EDGE_SIZE = 10;
};

#endif // GRIDSELECTOR_H
