#include "panel.hpp"
#include <QString>

namespace nyub {
namespace rebase {

PanelWidget::PanelWidget(QWidget *, const Todo::TodoList &init,
                         std::shared_ptr<TodoListCallback> callback)
    : m_init(init), m_todoList(init), m_callback(callback) {
  ui.setupUi(this);
  for (const auto &item : init) {
    new QListWidgetItem(
        QString((item.kind + " " + item.sha1 + " " + item.message).c_str()),
        ui.todoList);
  }
  connect(ui.startRebaseButton, QPushButton::clicked, this,
          PanelWidget::startRebase);
  connect(ui.abortRebaseButton, QPushButton::clicked, this, PanelWidget::abort);
}

void PanelWidget::abort() {
  m_callback->set(m_init);
  close();
}

void PanelWidget::startRebase() {
  m_callback->set(m_todoList);
  close();
}
} // namespace rebase
} // namespace nyub
