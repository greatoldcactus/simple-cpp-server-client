#include "logger.h"

Logger::Logger(const std::string& filename){
    out.open(filename);

    if (!out.is_open()) {
        throw std::runtime_error("Unable to open log file");
    }
}

Logger::~Logger() {
    if (out.is_open()) {
        out.close();
    }
}

void Logger::Write(const std::string& s) {
    std::lock_guard<std::mutex> guard(lock);
    out<<s<<std::endl;
}