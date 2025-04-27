#include "panel.hpp"
#include <QString>

namespace nyub {
namespace rebase {

PanelWidget::PanelWidget(QWidget *, const Todo::TodoList &init,
                         std::shared_ptr<TodoListCallback> callback)
    : m_init(init), m_todoList(init), m_callback(callback) {
  ui.setupUi(this);
  ui.todoList->setFocusPolicy(Qt::FocusPolicy::NoFocus);
  ui.todoList->setSelectionMode(QAbstractItemView::NoSelection);
  ui.startRebaseButton->setFocusPolicy(Qt::FocusPolicy::NoFocus);
  ui.abortRebaseButton->setFocusPolicy(Qt::FocusPolicy::NoFocus);
  connect(ui.startRebaseButton, QPushButton::clicked, this,
          PanelWidget::startRebase);
  connect(ui.abortRebaseButton, QPushButton::clicked, this, PanelWidget::abort);
  update();
}

void PanelWidget::update() {
  ui.todoList->clear();
  for (const auto &item : m_todoList) {
    new QListWidgetItem(
        QString((item.kind + " " + item.sha1 + " " + item.message).c_str()),
        ui.todoList);
  }
}

void PanelWidget::up() {
  if (m_selected != 0)
    m_selected--;
}

void PanelWidget::down() {
  m_selected = std::min(m_selected + 1, m_todoList.size() - 1);
}

void PanelWidget::moveUp() {
  if (m_selected == 0)
    return;
  m_todoList = Todo::swap(m_todoList, m_selected, m_selected - 1);
  up();
  update();
}

void PanelWidget::moveDown() {
  if (m_selected >= m_todoList.size())
    return;
  m_todoList = Todo::swap(m_todoList, m_selected, m_selected + 1);
  down();
  update();
}

void PanelWidget::startRebase() {
  m_callback->set(m_todoList);
  close();
}

void PanelWidget::abort() {
  m_callback->set(m_init);
  close();
}

void PanelWidget::keyReleaseEvent(QKeyEvent *keyPressed) {
  switch (keyPressed->key()) {
  case Qt::Key_Up:
    if (keyPressed->modifiers().testFlag(Qt::AltModifier)) {
      moveUp();
    } else {
      up();
    }
    break;
  case Qt::Key_Down:
    if (keyPressed->modifiers().testFlag(Qt::AltModifier)) {
      moveDown();
    } else {
      down();
    }
    break;
  }
  QWidget::keyPressEvent(keyPressed);
}
} // namespace rebase
} // namespace nyub
