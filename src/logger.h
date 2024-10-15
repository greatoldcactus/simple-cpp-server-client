#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <mutex>
#include <string>
#include <fstream>

class Logger {
    std::mutex lock;

    std::ofstream out;
    
    public:
    Logger(const std::string& filename);
    ~Logger();

    void Write(const std::string& s);
};


#endif