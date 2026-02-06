#include "wren_scripted.hpp"

void nyub::rebase::WithWrenScriptWidget::runScript(QString const &path) {
  QFile file(path);
  if (!file.open(QFile::ReadOnly)) {
    return;
  }
  std::string source = file.readAll().toStdString();
  wrenInterpret(wren.vm, "main", source.c_str());
}
