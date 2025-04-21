#include "lib.hpp"
#include "panel.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace nyub::rebase;

class TestCallback : public TodoListCallback {
public:
  virtual void set(const Todo::TodoList &_) { m_called++; }
  int m_called = 0;
};

class PanelWidgetTest : public testing::Test {
protected:
  int argc = 0;
  char **argv = nullptr;
  QApplication app = QApplication(argc, argv);
};

TEST_F(PanelWidgetTest, CallbackCalledWhenStartRebase) {
  auto spy = std::make_shared<TestCallback>();
  auto panel = PanelWidget(nullptr, Todo::TodoList{}, spy);
  panel.startRebase();
  ASSERT_EQ(spy->m_called, 1);
}
