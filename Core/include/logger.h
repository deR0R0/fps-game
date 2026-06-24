#pragma once

#include <mutex>
#include <string>

using namespace std;

namespace Core {
class Logger {
public:
    // singleton
    static Logger *getInstance();

    // other methods
    void info(string message);
    void warn(string message);
    void err(string message);

private:
    // constructor
    Logger();
    // instance vars
    mutex logger_mutex;
    static Logger *instance;
    // other methods
    void log(string type, string message);
    static char *getDateTime();
};
} // namespace Core
