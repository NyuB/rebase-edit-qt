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

class WrenBundle {
public:
  WrenBundle() {
    WrenConfiguration config;
    wrenInitConfiguration(&config);
    config.writeFn = writeFn;
    config.errorFn = errorFn;

    vm = wrenNewVM(&config); // Even though it takes an address, this copies the
                             // config, no need to keep it around
  }
  ~WrenBundle() {
    if (vm) {
      wrenFreeVM(vm);
      vm = nullptr;
    }
  }

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
  explicit WithWrenScriptWidget(QWidget *parent, QString const &p_script)
      : QWidget(parent), script(p_script) {
    button = new QPushButton("CLICK", this);
    runScript(script);
    watcher.addPath(script);
    connect(&watcher, &QFileSystemWatcher::fileChanged, this,
            &WithWrenScriptWidget::runScript);
    connect(button, &QPushButton::clicked, this,
            [this](bool) { runScript(script); });
  }
public slots:
  void setText(QString const &text) { button->setText(text); }

private:
  void wrenBind_setText();
  void runScript(QString const &path);
  QPushButton *button;
  QString script;
  WrenBundle wren;
  QFileSystemWatcher watcher;
};

} // namespace rebase
} // namespace nyub
#endif
