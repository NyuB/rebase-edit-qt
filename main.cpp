#include "lib.hpp"
#include "panel.hpp"
#include <QApplication>
#include <iostream>

int main(int argc, char **argv) {
  std::cout << "Starting Application ..." << std::endl;
  QApplication app(argc, argv);
  PanelWidget panel(nullptr);
  panel.show();
  return app.exec();
}
