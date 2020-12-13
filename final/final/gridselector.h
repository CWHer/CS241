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

  private:
    vector<QCheckBox *> button_group;
    QPushButton *apply_button;
};

#endif // GRIDSELECTOR_H
