#ifndef LOGGER_H
#define LOGGER_H
#include <iostream>

using namespace std;

namespace logger {  
  static void log_error(const char* message) {
    cerr << message << endl;
  }

  static void log_message(const char* message) {
    clog << message << endl;
  }
}

#endif
