#ifndef NYUB_WREN_SCRIPTED_HPP
#define NYUB_WREN_SCRIPTED_HPP

#include "wren.h"

#include <QFile>
#include <QFileSystemWatcher>
#include <QObject>
#include <QPushButton>
#include <QWidget>

namespace nyub {
namespace rebase {

//! RAII-friendly wrapper around a wren static method handle
class WrenMethodBundle {
public:
  WrenMethodBundle(WrenVM *vm_MustOutliveThis, std::string const &module,
                   std::string const &className, std::string const &signature);

  //! Call the method assuming it is taking 0 argument
  void call0();

  ~WrenMethodBundle();

protected:
  WrenVM *vm{nullptr};
  WrenHandle *classHandle{nullptr};
  WrenHandle *methodHandle{nullptr};
};

//! RAII-friendly wrapper around a wren VM
class WrenBundle {
public:
  WrenBundle();

  ~WrenBundle();

  WrenVM *vm{nullptr};

private:
  static void writeFn(WrenVM *, const char *text) {
    printf("[WREN LOG] %s", text);
  }

  static void errorFn(WrenVM *, WrenErrorType errorType, const char *module,
                      const int line, const char *msg) {
    switch (errorType) {
    case WREN_ERROR_COMPILE: {
      printf("[WREN COMPILE ERROR] [%s line %d] [Error] %s\n", module, line,
             msg);
    } break;
    case WREN_ERROR_STACK_TRACE: {
      printf("[%s line %d] in %s\n", module, line, msg);
    } break;
    case WREN_ERROR_RUNTIME: {
      printf("[WREN RUNTIME ERROR] %s\n", msg);
    } break;
    }
  }
};

class WithWrenScriptWidget : public QWidget {
  Q_OBJECT
public:
  explicit WithWrenScriptWidget(QWidget *parent, QString const &p_script);
public slots:
  void setText(QString const &text) { button->setText(text); }

private:
  void loadScript(QString const &path);
  void runScript();

  //! Create a fresh module each time we load a script to avoid
  //! 'already defined variable' errors
  std::string newModuleName();

  WrenBundle wren{};
  QPushButton *button;
  QString script;
  QFileSystemWatcher watcher{};
  std::unique_ptr<WrenMethodBundle> doMethod{nullptr};
  size_t loadCount{0};
};

} // namespace rebase
} // namespace nyub
#endif
