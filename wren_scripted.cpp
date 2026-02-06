#include "wren_scripted.hpp"
#include "wren.h"
#include <string>

namespace nyub::rebase {

WrenBundle::WrenBundle() {
  WrenConfiguration config;
  wrenInitConfiguration(&config);
  config.writeFn = writeFn;
  config.errorFn = errorFn;

  vm = wrenNewVM(&config); // Even though it takes an address, this copies the
                           // config, no need to keep it around
}

WrenBundle::~WrenBundle() {
  if (vm) {
    wrenFreeVM(vm);
    vm = nullptr;
  }
}

WrenMethodBundle::~WrenMethodBundle() {
  if (vm && methodHandle && classHandle) {
    wrenReleaseHandle(vm, methodHandle);
    wrenReleaseHandle(vm, classHandle);
    methodHandle = nullptr;
    classHandle = nullptr;
    vm = nullptr;
  }
}

WithWrenScriptWidget::WithWrenScriptWidget(QWidget *parent,
                                           QString const &p_script)
    : QWidget(parent), script(p_script) {
  button = new QPushButton("CLICK", this);
  loadScript(script);
  watcher.addPath(script);
  connect(&watcher, &QFileSystemWatcher::fileChanged, this,
          &WithWrenScriptWidget::loadScript);
  connect(button, &QPushButton::clicked, this,
          &WithWrenScriptWidget::runScript);
}

WrenMethodBundle::WrenMethodBundle(WrenVM *vm_MustOutliveThis,
                                   std::string const &module,
                                   std::string const &className,
                                   std::string const &signature)
    : vm(vm_MustOutliveThis) {

  // load method handle for any method with the given signature (regardless of
  // the receiver)
  methodHandle = wrenMakeCallHandle(vm, signature.c_str());

  // load an object, here the class we want to call the static method on
  wrenEnsureSlots(vm, 1);
  wrenGetVariable(vm, module.c_str(), className.c_str(), 0);
  classHandle = wrenGetSlotHandle(vm, 0);
}

void WrenMethodBundle::call0() {
  // Push the target class on the receiver slot
  wrenEnsureSlots(vm, 1);
  wrenSetSlotHandle(vm, 0, classHandle);

  wrenCall(vm, methodHandle);
}

void WithWrenScriptWidget::loadScript(QString const &path) {
  QFile file(path);
  if (!file.open(QFile::ReadOnly)) {
    return;
  }
  std::string source = file.readAll().toStdString();
  std::string moduleName = newModuleName();
  WrenInterpretResult res =
      wrenInterpret(wren.vm, moduleName.c_str(), source.c_str());
  if (res == WREN_RESULT_SUCCESS) {
    doMethod = std::make_unique<WrenMethodBundle>(wren.vm, moduleName, "Hello",
                                                  "do()");
  }
}

void WithWrenScriptWidget::runScript() {
  if (doMethod) {
    doMethod->call0();
  }
}
std::string WithWrenScriptWidget::newModuleName() {
  std::string moduleName = std::string{"widget"} + std::to_string(loadCount);
  loadCount++;
  return moduleName;
};
} // namespace nyub::rebase
