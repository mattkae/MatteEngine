#ifndef LOGGER_H
#define LOGGER_H
#include <iostream>

using namespace std;

namespace logger {  
  static void LogError(const char* message) {
    cerr << message << endl;
  }

  static void LogMessage(const char* message) {
    clog << message << endl;
  }
}

#endif
