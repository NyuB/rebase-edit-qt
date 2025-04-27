#include "lib.hpp"
#include <algorithm>
#include <format>

namespace nyub {
namespace rebase {
Todo Todo::from(const RebaseFileEntry &fileEntry) {
  return Todo{
      .kind = "pick",
      .sha1 = fileEntry.sha1,
      .message = fileEntry.message,
      .renamed = {},
  };
}

Todo::TodoList Todo::from(const RebaseFileEntry::RebaseFile &file) {
  auto result = TodoList{};
  result.reserve(file.size());
  for (const auto &item : file) {
    result.push_back(from(item));
  }
  return result;
}

Todo::TodoList Todo::swap(const TodoList &todoList, size_t index_a,
                          size_t index_b) {
  if (index_a == index_b)
    return todoList;
  if (index_a >= todoList.size() || index_b >= todoList.size())
    return todoList;
  TodoList result = todoList;
  std::swap(*(result.begin() + index_a), *(result.begin() + index_b));
  return result;
}

std::optional<RebaseFileEntry>
RebaseFileEntry::parse(const std::string &gitLine) {
  size_t firstSpace = gitLine.find(' ');
  if (firstSpace == std::string::npos)
    return {};
  size_t secondSpace = gitLine.find(' ', firstSpace + 1);
  if (secondSpace == std::string::npos)
    return {};
  if (gitLine.substr(0, firstSpace) != "pick")
    return {};

  return RebaseFileEntry{
      .sha1 = gitLine.substr(firstSpace + 1, secondSpace - firstSpace - 1),
      .message = gitLine.substr(secondSpace + 1)};
}

std::ostream &operator<<(std::ostream &os, const RebaseFileEntry &entry) {
  os << std::format("pick sha1={} message='{}'", entry.sha1, entry.message);
  return os;
}

std::string todoFileEntryFromTodo(const Todo &todo) {
  return std::format("{} {} {}", todo.kind, todo.sha1, todo.message);
}

TodoFile todoFile(const Todo::TodoList &fileEntry) {
  std::vector<std::string> result;
  for (const auto &item : fileEntry) {
    result.push_back(todoFileEntryFromTodo(item));
    if (item.renamed.has_value()) {
      result.push_back(
          std::format("exec git commit --amend -m '{}'", item.renamed.value()));
    }
  }
  return result;
}
} // namespace rebase
} // namespace nyub
