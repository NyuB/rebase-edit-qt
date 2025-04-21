#include "lib.hpp"
#include "panel.hpp"
#include <QApplication>
#include <fstream>
#include <iostream>

using namespace nyub::rebase;

RebaseFileEntry::RebaseFile read(char *file) {
  std::ifstream in(file);
  std::string line;
  RebaseFileEntry::RebaseFile result;
  while (std::getline(in, line)) {
    auto parsed = RebaseFileEntry::parse(line);
    if (parsed.has_value())
      result.push_back(parsed.value());
  }
  return result;
}

void write(char *file, const TodoFile &todoFile) {
  std::ofstream out(file);
  for (const auto &line : todoFile) {
    out << line << "\n";
  }
}

//! @brief ensure rebase commands are written back to git at application exit
class WriteRebaseFileAtAppExit : public TodoListCallback {
public:
  WriteRebaseFileAtAppExit(char *file, const Todo::TodoList &todoList)
      : m_file(file) {
    set(todoList);
  }
  void set(const Todo::TodoList &todoList) { m_todoList = todoList; }
  ~WriteRebaseFileAtAppExit() { write(m_file, todoFile(m_todoList)); }

private:
  Todo::TodoList m_todoList;
  char *m_file;
};

int main(int argc, char **argv) {
  RebaseFileEntry::RebaseFile input;
  if (argc > 1)
    input = read(argv[1]);
  auto callback =
      std::make_shared<WriteRebaseFileAtAppExit>(argv[1], Todo::from(input));
  QApplication app(argc, argv);
  PanelWidget panel(nullptr, Todo::from(input), callback);
  panel.show();
  return app.exec();
}
