#include "lib.hpp"
#include "panel.hpp"
#include <QKeyEvent>
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

TEST_F(PanelWidgetTest, MoveUpDown) {
  auto spy = std::make_shared<TestCallback>();
  auto panel = PanelWidget(nullptr,
                           Todo::TodoList{
                               Todo{
                                   .kind = "pick",
                                   .sha1 = "A",
                                   .message = "AAA",
                                   .renamed = {},
                               },
                               Todo{
                                   .kind = "pick",
                                   .sha1 = "B",
                                   .message = "BBB",
                                   .renamed = {},
                               },
                               Todo{
                                   .kind = "pick",
                                   .sha1 = "C",
                                   .message = "CCC",
                                   .renamed = {},
                               },
                           },
                           spy);
  ASSERT_TRUE(panel.grab().save("resources/moveUpDown_00_init.png"));
  panel.moveDown();
  ASSERT_TRUE(panel.grab().save("resources/moveUpDown_01_down.png"));
  panel.moveDown();
  ASSERT_TRUE(panel.grab().save("resources/moveUpDown_02_down_down.png"));
  panel.moveUp();
  ASSERT_TRUE(panel.grab().save("resources/moveUpDown_04_down_down_up.png"));
  panel.startRebase();
  spy->wasCalledWith(Todo::TodoList{
      Todo{
          .kind = "pick",
          .sha1 = "B",
          .message = "BBB",
          .renamed = {},
      },
      Todo{
          .kind = "pick",
          .sha1 = "A",
          .message = "AAA",
          .renamed = {},
      },
      Todo{
          .kind = "pick",
          .sha1 = "C",
          .message = "CCC",
          .renamed = {},
      },
  });
}

TEST_F(PanelWidgetTest, FixupPick) {
  auto spy = std::make_shared<TestCallback>();
  auto panel = PanelWidget(nullptr,
                           Todo::TodoList{
                               Todo{
                                   .kind = "pick",
                                   .sha1 = "A",
                                   .message = "AAA",
                                   .renamed = {},
                               },
                               Todo{
                                   .kind = "pick",
                                   .sha1 = "B",
                                   .message = "BBB",
                                   .renamed = {},
                               },
                               Todo{
                                   .kind = "pick",
                                   .sha1 = "C",
                                   .message = "CCC",
                                   .renamed = {},
                               },
                           },
                           spy);
  ASSERT_TRUE(panel.grab().save("resources/fixupPick_00_init.png"));
  panel.setKind("fixup");
  ASSERT_TRUE(panel.grab().save("resources/fixupPick_01_fixup.png"));
  panel.down();
  panel.setKind("fixup");
  ASSERT_TRUE(panel.grab().save("resources/fixupPick_02_down_fixup.png"));
  panel.setKind("pick");
  ASSERT_TRUE(panel.grab().save("resources/fixupPick_03_down_fixup_pick.png"));
  panel.startRebase();
  spy->wasCalledWith(Todo::TodoList{
      Todo{
          .kind = "fixup",
          .sha1 = "A",
          .message = "AAA",
          .renamed = {},
      },
      Todo{
          .kind = "pick",
          .sha1 = "B",
          .message = "BBB",
          .renamed = {},
      },
      Todo{
          .kind = "pick",
          .sha1 = "C",
          .message = "CCC",
          .renamed = {},
      },
  });
}
