#pragma once 

#include <sstream> 
#include <fstream> 
#include <string> 
#include <mutex> 
#include <ostream> 

class Logger {
public:
    explicit Logger(const std::string& filename) {
        std::string full_path = "./logs/" + filename + ".log";
        file_.open(full_path.c_str(), std::ios::trunc);
        if (!file_.is_open()) {
            throw std::runtime_error("Unable to open log file: " + filename);
        }
    }

    // ”дал€ем конструктор копировани€ и оператор присваивани€
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    // –азрешаем перемещение
    Logger(Logger&&) = default;
    Logger& operator=(Logger&&) = default;

    ~Logger() {
        if (file_.is_open()) {
            flushToFile();
            file_.close();
        }
    }

    // Ўаблонный оператор << дл€ всех типов, кроме манипул€торов 
    template<typename T>
    Logger& operator<<(const T& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        buffer_ << value;
        return *this;
    }

    // ѕерегрузка дл€ манипул€торов 
    Logger& operator<<(std::ostream& (*manip)(std::ostream&)) {
        std::lock_guard<std::mutex> lock(mutex_);
        buffer_ << manip;

        std::string buffer_content = buffer_.str();
        if (buffer_content.find('\n') != std::string::npos) {
            flushToFile();
        }
        return *this;
    }

private:
    std::ofstream file_;
    std::stringstream buffer_;
    std::mutex mutex_;

    void flushToFile() {
        if (file_.is_open()) {
            file_ << buffer_.str();
            file_.flush();
            buffer_.str("");
            buffer_.clear();
        }
    }
};