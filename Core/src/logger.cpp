#include "logger.h"

#include <cstring>
#include <ctime>
#include <iostream>
#include <mutex>

using namespace Core;
using namespace std;

Logger *Logger::instance = nullptr;

Logger::Logger() { Logger::instance = this; }

Logger *Logger::getInstance() {
    if (!Logger::instance) {
        new Logger();
    }

    return Logger::instance;
}

char *Logger::getDateTime() {
    time_t timestamp;
    time(&timestamp);
    char *timestampCString = ctime(&timestamp);

    if (timestampCString[strlen(timestampCString) - 1] == '\n')
        timestampCString[strlen(timestampCString) - 1] = '\0';

    return timestampCString;
}

void Logger::log(string type, string message) {
    std::lock_guard<std::mutex> lock(logger_mutex); // lock thread
    cout << "[" << type << "] " << getDateTime() << "\t\t" << message << endl;
}

void Logger::info(string message) { Logger::log("INFO", message); }
void Logger::warn(string message) { Logger::log("WARN", message); }
void Logger::err(string message) { Logger::log("ERR", message); }
