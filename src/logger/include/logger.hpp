/**
 * @file logger.hpp
 *
 * @brief Definition of class @ref VARAGAVAN::ARMTTS::Logger
 *
 *  Author: Albert Grigoryan
 *  E-mail: albert_grigoryan@yahoo.com
 *  109/26 Vardanants St.,
 *  Yerevan, 0050, Armenia
 *  Tel:  +374-93-635-380
 *
 *  Copyright © 2022 Albert Grigoryan. All rights reserved.
 */

#pragma once

#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

namespace VARAGAVAN {
namespace ARMTTS {

class Logger {
public:
    /**
     * Get logger.
     * @return logger
     */
    static log4cpp::Category& getLogger() noexcept;

    /**
     * @brief Destructor.
     */
    ~Logger();

private:

    /**
     * Check logger configuration file existence.
     * @return true if file exists, otherwise return false.
     */
    static bool checkConfigurationFile() noexcept;

    /**
     * @brief Private constructor
     */
    Logger();

    /**
     * @brief not copy-constructable;
     */
    Logger(const Logger&) = delete;

    /**
     * @brief not move-constructable;
     */
    Logger(Logger&&) = delete;

    /**
     * @brief not assignable
     */
    Logger& operator=(const Logger&) = delete;

    /**
     * @brief not move-assignable
     */
    Logger& operator=(Logger&&) = delete;

    static Logger* s_logger;
    static log4cpp::Category& s_lo4cplus_logger;

    static constexpr char CONFIGURATION_FILE[] = "cfg/logger.properties";
};

}
}
