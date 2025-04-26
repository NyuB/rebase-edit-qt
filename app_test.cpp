#include "lib.hpp"
#include "panel.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace nyub::rebase;

class TestCallback : public TodoListCallback {
public:
  virtual void set(const Todo::TodoList &todoList) {
    m_called.push_back(todoList);
  }
  void wasCalledWith(const Todo::TodoList &todoList) {
    ASSERT_THAT(m_called, testing::ElementsAre(todoList));
  }

private:
  std::vector<Todo::TodoList> m_called{};
};

class PanelWidgetTest : public testing::Test {
protected:
  int argc = 2;
  char _platform[11] = "-platform\0";
  char offscreen[11] = "offscreen\0";
  char *argv[2]{_platform, offscreen};
  QApplication app = QApplication(argc, argv);
};

TEST_F(PanelWidgetTest, StartRebase) {
  auto spy = std::make_shared<TestCallback>();
  const auto todoList = Todo::TodoList{};
  auto panel = PanelWidget(nullptr, todoList, spy);
  panel.startRebase();
  spy->wasCalledWith(todoList);
}

TEST_F(PanelWidgetTest, AbortRebase) {
  auto spy = std::make_shared<TestCallback>();
  const auto todoList = Todo::TodoList{};
  auto panel = PanelWidget(nullptr, todoList, spy);
  panel.abort();
  spy->wasCalledWith(todoList);
}

TEST_F(PanelWidgetTest, DisplaysTodoItems) {
  auto spy = std::make_shared<TestCallback>();
  auto panel = PanelWidget(nullptr,
                           Todo::TodoList{
                               Todo{
                                   .kind = "pick",
                                   .sha1 = "ABCDEF0123456789",
                                   .message = "Hey",
                                   .renamed = {},
                               },
                               Todo{
                                   .kind = "pick",
                                   .sha1 = "0123456789ABCDEF",
                                   .message = "Hola",
                                   .renamed = {},
                               },
                           },
                           spy);
  ASSERT_TRUE(panel.grab().save("resources/init.png"));
}
