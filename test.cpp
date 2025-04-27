#include "lib.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <limits>
#include <optional>

using testing::Eq;
using testing::Values;
using namespace nyub::rebase;

TEST(RebaseFileEntry, Parse) {
  std::string gitLine = "pick Sha1 message";
  auto entry = RebaseFileEntry::parse(gitLine);
  auto expected = RebaseFileEntry{.sha1 = "Sha1", .message = "message"};
  ASSERT_EQ(entry, expected);
}

TEST(RebaseFileEntry, ParseInvalidKind) {
  std::string invalidKindLine = "unknown Sha1 message";
  ASSERT_EQ(RebaseFileEntry::parse(invalidKindLine),
            std::optional<RebaseFileEntry>{});
}

TEST(RebaseFileEntry, ParseMissingMessage) {
  std::string missingMessage = "pick Sha1";
  ASSERT_EQ(RebaseFileEntry::parse(missingMessage),
            std::optional<RebaseFileEntry>{});
}

TEST(RebaseFileEntry, ParseMessageWithSpaces) {
  std::string longMessage = "pick Sha1 A very long message with spaces";
  auto expected = RebaseFileEntry{.sha1 = "Sha1",
                                  .message = "A very long message with spaces"};
  ASSERT_EQ(RebaseFileEntry::parse(longMessage), expected);
}

TEST(Todo, FromFileEntry) {
  auto fileEntry = RebaseFileEntry{.sha1 = "Sha1", .message = "message"};
  auto todo = Todo::from(fileEntry);
  auto expected =
      Todo{.kind = "pick", .sha1 = "Sha1", .message = "message", .renamed = {}};
  ASSERT_EQ(todo, expected);
}

TEST(TodoList, SwapZero) {
  auto emptyTodoList = Todo::TodoList{};
  ASSERT_EQ(Todo::swap(emptyTodoList, 0, 1), emptyTodoList);
}

TEST(TodoList, SwapOne) {
  auto singleItemTodoList = Todo::TodoList{Todo{
      .kind = "pick", .sha1 = "Sha1", .message = "message", .renamed = {}}};
  ASSERT_EQ(Todo::swap(singleItemTodoList, 0, 1), singleItemTodoList);
}

TEST(TodoList, SwapTwo) {
  auto singleItemTodoList = Todo::TodoList{
      Todo{.kind = "pick", .sha1 = "A", .message = "MsgA", .renamed = {}},
      Todo{.kind = "pick", .sha1 = "B", .message = "MsgB", .renamed = {}},
  };
  ASSERT_THAT(
      Todo::swap(singleItemTodoList, 0, 1),
      testing::ElementsAre(
          Todo{.kind = "pick", .sha1 = "B", .message = "MsgB", .renamed = {}},
          Todo{.kind = "pick", .sha1 = "A", .message = "MsgA", .renamed = {}}
          //,
          ));
}

TEST(TodoList, SwapMany) {
  auto singleItemTodoList = Todo::TodoList{
      Todo{.kind = "pick", .sha1 = "A", .message = "MsgA", .renamed = {}},
      Todo{.kind = "pick", .sha1 = "B", .message = "MsgB", .renamed = {}},
      Todo{.kind = "pick", .sha1 = "C", .message = "MsgC", .renamed = {}},
      Todo{.kind = "pick", .sha1 = "D", .message = "MsgD", .renamed = {}},
      Todo{.kind = "pick", .sha1 = "E", .message = "MsgE", .renamed = {}},
  };

  ASSERT_THAT(
      Todo::swap(singleItemTodoList, 1, 4),
      testing::ElementsAre(
          Todo{.kind = "pick", .sha1 = "A", .message = "MsgA", .renamed = {}},
          Todo{.kind = "pick", .sha1 = "E", .message = "MsgE", .renamed = {}},
          Todo{.kind = "pick", .sha1 = "C", .message = "MsgC", .renamed = {}},
          Todo{.kind = "pick", .sha1 = "D", .message = "MsgD", .renamed = {}},
          Todo{.kind = "pick", .sha1 = "B", .message = "MsgB", .renamed = {}}
          //,
          ));
}

TEST(TodoList, SwapEdge) {
  auto twoItemsTodoList = Todo::TodoList{
      Todo{.kind = "pick", .sha1 = "A", .message = "MsgA", .renamed = {}},
      Todo{.kind = "pick", .sha1 = "B", .message = "MsgB", .renamed = {}},
  };

  const auto inBoundIndex = 1;
  const auto outOfBoundIndex = 2;

  ASSERT_EQ(Todo::swap(twoItemsTodoList, outOfBoundIndex, inBoundIndex),
            twoItemsTodoList);
  ASSERT_EQ(Todo::swap(twoItemsTodoList, inBoundIndex, outOfBoundIndex),
            twoItemsTodoList);
}

TEST(TodoFileEntry, FromTodoZero) {
  auto todos = Todo::TodoList{};
  ASSERT_EQ(todoFile(todos), TodoFile{});
}

TEST(TodoFileEntry, FromTodoOne) {
  auto todo =
      Todo{.kind = "pick", .sha1 = "Sha1", .message = "message", .renamed = {}};
  auto expected = TodoFile{"pick Sha1 message"};
  ASSERT_EQ(todoFile(Todo::TodoList{todo}), expected);
}

TEST(TodoFileEntry, FromTodoRenamed) {
  auto todo = Todo{.kind = "pick",
                   .sha1 = "Sha1",
                   .message = "message",
                   .renamed = "renamed"};
  auto expected =
      TodoFile{"pick Sha1 message", "exec git commit --amend -m 'renamed'"};
  ASSERT_EQ(todoFile(Todo::TodoList{todo}), expected);
}

struct AddTestParam {
  int left;
  int right;
  int expected;
};
class AddTest : public testing::TestWithParam<AddTestParam> {};

TEST_P(AddTest, Add) {
  auto param = GetParam();
  ASSERT_EQ(param.left + param.right, param.expected);
}

INSTANTIATE_TEST_SUITE_P(HappyPaths, AddTest,
                         Values(AddTestParam{1, 2, 3}, AddTestParam{2, 1, 3}));
INSTANTIATE_TEST_SUITE_P(ExtremeValues, AddTest,
                         Values(AddTestParam{1, -1, 0},
                                AddTestParam{INT_MAX, 1, INT_MIN}));
