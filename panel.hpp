#include "ui_panel.h"

class PanelWidget : public QWidget {
  Q_OBJECT
public:
  explicit PanelWidget(QWidget *parent);
private slots:
  void funky();

private:
  Ui::Form ui;
  bool m_isFunky = false;
};