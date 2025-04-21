#include <optional>
#include <string>
#include <vector>
namespace nyub {
namespace rebase {

//! Input provided by git to the rebase editor, designate a commit to pick
struct RebaseFileEntry {
  using RebaseFile = std::vector<RebaseFileEntry>;
  std::string sha1;
  std::string message;
  static RebaseFileEntry parse(const std::string &gitLine);
  bool operator==(const RebaseFileEntry &other) const = default;
};

//! Work in progress on rebase items
struct Todo {
  using TodoList = std::vector<Todo>;
  std::string kind;
  std::string sha1;
  std::string message;
  std::optional<std::string> renamed;
  static Todo from(const RebaseFileEntry &fileEntry);
  bool operator==(const Todo &other) const = default;
};

//! Output given back to git by the rebase editor
using TodoFile = std::vector<std::string>;

TodoFile todoFile(const Todo::TodoList &todoList);

} // namespace rebase
} // namespace nyub
