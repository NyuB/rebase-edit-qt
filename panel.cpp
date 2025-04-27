#include "panel.hpp"
#include <QString>
#include <iostream>

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
  connect(ui.todoList, QListWidget::itemChanged, this,
          PanelWidget::itemChanged);
  update();
}

std::string itemText(const Todo &item) {
  if (!item.renamed.has_value()) {
    return std::format("{} {} {}", item.kind, item.sha1, item.message);
  } else {
    return std::format("{} {} {} (renamed)", item.kind, item.sha1,
                       item.renamed.value());
  }
}

void PanelWidget::update() {
  ui.todoList->clear();
  for (const auto &item : m_todoList) {
    auto widget = new QListWidgetItem(QString::fromStdString(itemText(item)),
                                      ui.todoList);
    widget->setFlags(widget->flags() | Qt::ItemIsEditable);
  }
  if (m_todoList.size() > 0) {
    const auto selectedBackGround =
        ui.todoList->item(m_selected)->background().color();
    ui.todoList->item(m_selected)->setBackground(selectedBackGround.lighter());
  }
}

void PanelWidget::setKind(std::string const &kind) {
  m_todoList = Todo::withKind(m_todoList, m_selected, kind);
  update();
}

void PanelWidget::up() {
  if (m_selected != 0)
    m_selected--;
  update();
}

void PanelWidget::down() {
  m_selected = std::min(m_selected + 1, m_todoList.size() - 1);
  update();
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

void PanelWidget::startRename() {
  if (m_todoList.empty())
    return;
  m_renaming = true;
  ui.todoList->editItem(ui.todoList->item(m_selected));
}

void PanelWidget::rename(std::string const &newName) {
  m_todoList = Todo::renamedTo(m_todoList, m_selected, newName);
  update();
}

// Detect end of editing from QListWidget
void PanelWidget::itemChanged(QListWidgetItem *item) {
  // /!\ DO NOT call update() from this method this since it would trigger
  // itemChanged() again
  if (m_renaming && static_cast<size_t>(ui.todoList->row(item)) ==
                        m_selected) { // Only act if the eent correspond to the
                                      // end of a current edit
    m_renaming = false;
    const auto &selected = m_todoList[m_selected];
    // Since the entire item text is editable, strip the meta prefix from the
    // renamed text
    const auto prefix = std::format("{} {} ", selected.kind, selected.sha1);
    m_todoList = Todo::renamedTo(
        m_todoList, m_selected,
        item->text().replace(QString::fromStdString(prefix), "").toStdString());
  }
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
  case Qt::Key_F:
    setKind("fixup");
    break;
  case Qt::Key_S:
    setKind("squash");
    break;
  case Qt::Key_P:
    setKind("pick");
    break;
  case Qt::Key_R:
  case Qt::Key_F2:
    startRename();
    break;
  }
  QWidget::keyPressEvent(keyPressed);
}
} // namespace rebase
} // namespace nyub
