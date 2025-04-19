#include "panel.hpp"

PanelWidget::PanelWidget(QWidget *parent) {
  ui.setupUi(this);
  connect(ui.pushButton, QPushButton::clicked, this, PanelWidget::funky);
}

void PanelWidget::funky() {
  if (m_isFunky) {
    ui.pushButton->setText("Boring");
  } else {
    ui.pushButton->setText("Yeeepeee");
  }
  m_isFunky = !m_isFunky;
}
