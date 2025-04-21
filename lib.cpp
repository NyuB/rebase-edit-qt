#include "lib.hpp"
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

std::optional<RebaseFileEntry>
RebaseFileEntry::parse(const std::string &gitLine) {
  std::vector<std::string> splitted;
  splitted.reserve(3);
  std::string currentWord = "";
  for (const auto &item : gitLine) {
    if (item == ' ') {
      splitted.push_back(currentWord);
      currentWord = "";
    } else {
      currentWord += item;
    }
  }
  splitted.push_back(currentWord);
  if (splitted.size() < 3 || splitted.at(0) != "pick")
    return {};
  return RebaseFileEntry{.sha1 = splitted.at(1), .message = splitted.at(2)};
}

std::ostream &operator<<(std::ostream &os, const RebaseFileEntry &entry) {
  os << std::format("pick {} {}", entry.sha1, entry.message);
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
