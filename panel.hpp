#ifndef NYUB_REBASE_PANEL_HPP
#define NYUB_REBASE_PANEL_HPP
#include "lib.hpp"
#include "ui_panel.h"
#include <QKeyEvent>
#include <QLabel>
#include <vector>

namespace nyub {
namespace rebase {

//! @interface TodoListCallback
class TodoListCallback {
public:
  //! Updates the currently edited todo list
  //! @param results the currently edited todo list
  virtual void set(const nyub::rebase::Todo::TodoList &results) = 0;
  virtual ~TodoListCallback() {};
};

class PanelWidget : public QWidget {
  Q_OBJECT
public:
  explicit PanelWidget(QWidget *parent, const Todo::TodoList &init,
                       std::shared_ptr<TodoListCallback> callback);
  void setKind(std::string const &kind);
  void up();
  void down();
  void moveUp();
  void moveDown();
public slots:
  void startRebase();
  void abort();

protected:
  void keyReleaseEvent(QKeyEvent *keyPressed) override;

private:
  void update();
  Ui::Panel ui;
  Todo::TodoList m_init;
  Todo::TodoList m_todoList;
  std::shared_ptr<TodoListCallback> m_callback;
  size_t m_selected = 0;
};

} // namespace rebase
} // namespace nyub
#endif
