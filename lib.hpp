#ifndef NYUB_REBASE_LIB_HPP
#define NYUB_REBASE_LIB_HPP

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
  static std::optional<RebaseFileEntry> parse(const std::string &gitLine);
  bool operator==(const RebaseFileEntry &other) const = default;

  friend std::ostream &operator<<(std::ostream &os,
                                  const RebaseFileEntry &entry);
};

//! Work in progress on rebase items
struct Todo {
  using TodoList = std::vector<Todo>; // TODO dedicated class
  std::string kind;                   // TODO use typed enum/variant
  std::string sha1;
  std::string message;
  std::optional<std::string> renamed;
  static Todo from(const RebaseFileEntry &fileEntry);
  static TodoList from(const RebaseFileEntry::RebaseFile &file);
  static TodoList swap(const TodoList &todoList, size_t index_a,
                       size_t index_b);
  static TodoList withKind(const TodoList &todoList, size_t index,
                           std::string const &kind);
  bool operator==(const Todo &other) const = default;

private:
  Todo withKind(std::string const &newKind);
};

//! Output given back to git by the rebase editor
using TodoFile = std::vector<std::string>;

TodoFile todoFile(const Todo::TodoList &todoList);

} // namespace rebase
} // namespace nyub
#endif // NYUB_REBASE_LIB_HPP
