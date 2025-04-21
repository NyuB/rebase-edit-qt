#include "panel.hpp"
#include <QString>

namespace nyub {
namespace rebase {

PanelWidget::PanelWidget(QWidget *parent, const Todo::TodoList &init,
                         std::shared_ptr<TodoListCallback> callback)
    : m_callback(callback), m_todoList(init) {
  ui.setupUi(this);
  for (const auto &item : init) {
    new QListWidgetItem(
        QString((item.kind + " " + item.sha1 + " " + item.message).c_str()),
        ui.todoList);
  }
  connect(ui.pushButton, QPushButton::clicked, this, PanelWidget::startRebase);
}

void PanelWidget::startRebase() {
  m_callback->set(m_todoList);
  close();
}
} // namespace rebase
} // namespace nyub
