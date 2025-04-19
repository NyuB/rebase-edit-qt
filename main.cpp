#include "lib.hpp"
#include <QApplication>
#include <iostream>

int main(int argc, char **argv) {
  std::cout << "Starting Application ..." << std::endl;
  QApplication app(argc, argv);
  return app.exec();
}
