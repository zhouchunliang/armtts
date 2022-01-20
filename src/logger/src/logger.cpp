#include "logger.hpp"

#include <filesystem>

namespace VARAGAVAN {
namespace ARMTTS {

Logger* Logger::s_logger = NULL;
log4cpp::Category& Logger::s_lo4cplus_logger = log4cpp::Category::getRoot();

log4cpp::Category& Logger::getLogger() noexcept {
    if (! s_logger) {
        s_logger = new Logger;
    }
    return s_lo4cplus_logger;
}

bool Logger::checkConfigurationFile() noexcept {
    if (! std::__fs::filesystem::exists(CONFIGURATION_FILE)) {
        std::cerr << "Error: " << CONFIGURATION_FILE << " configuration file does not exist." << std::endl;
        return false;
    }
    return true;
}

Logger::Logger() {
    if (! checkConfigurationFile()) {
        exit(1);
    }
    log4cpp::PropertyConfigurator::configure(CONFIGURATION_FILE);
    log4cpp::Appender* console_appender =
            s_lo4cplus_logger.getAppender("ConsoleAppender");
    if(const char* env_p = std::getenv("PROGRAM_VERBOSITY_LEVEL")) {
        std::string v(env_p);
        if (v == "ALERT") {
            console_appender->setThreshold(log4cpp::Priority::ALERT);
        } else if (v == "ERROR") {
            console_appender->setThreshold(log4cpp::Priority::ERROR);
        } else if (v == "WARNING") {
            console_appender->setThreshold(log4cpp::Priority::WARN);
        } else if (v == "INFO") {
            console_appender->setThreshold(log4cpp::Priority::INFO);
        } else if (v == "DEBUG") {
            console_appender->setThreshold(log4cpp::Priority::DEBUG);
        }
    } else {
        console_appender->setThreshold(log4cpp::Priority::WARN);
    }
}

Logger::~Logger() {
    std::cout << "Destructing...\n";
    delete s_logger;
    log4cpp::Category::shutdown();
}

}
}