#ifndef NYUB_REBASE_PANEL_HPP
#define NYUB_REBASE_PANEL_HPP
#include "lib.hpp"
#include "ui_panel.h"
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
public slots:
  void startRebase();
  void abort();

private:
  Ui::Form ui;
  Todo::TodoList m_init;
  Todo::TodoList m_todoList;
  std::shared_ptr<TodoListCallback> m_callback;
};

} // namespace rebase
} // namespace nyub
#endif
