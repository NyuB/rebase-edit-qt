#include "lib.hpp"
#include "panel.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace nyub::rebase;

class TestCallback : public TodoListCallback {
public:
  virtual void set(const Todo::TodoList &) { m_called++; }
  int m_called = 0;
};

class PanelWidgetTest : public testing::Test {
protected:
  int argc = 2;
  char _platform[11] = "-platform\0";
  char offscreen[11] = "offscreen\0";
  char *argv[2]{_platform, offscreen};
  QApplication app = QApplication(argc, argv);
};

TEST_F(PanelWidgetTest, CallbackCalledWhenStartRebase) {
  auto spy = std::make_shared<TestCallback>();
  auto panel = PanelWidget(nullptr, Todo::TodoList{}, spy);
  panel.startRebase();
  ASSERT_EQ(spy->m_called, 1);
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
  ASSERT_TRUE(panel.grab().save("resources/debug.png"));
}
