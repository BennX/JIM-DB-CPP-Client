/**
############################################################################
# GPL License                                                              #
#                                                                          #
# This file is part of the JIM-DB.                                         #
# Copyright (c) 2015, Benjamin Meyer, <benjamin.meyer@tu-clausthal.de>     #
# This program is free software: you can redistribute it and/or modify     #
# it under the terms of the GNU General Public License as                  #
# published by the Free Software Foundation, either version 3 of the       #
# License, or (at your option) any later version.                          #
#                                                                          #
# This program is distributed in the hope that it will be useful,          #
# but WITHOUT ANY WARRANTY; without even the implied warranty of           #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            #
# GNU General Public License for more details.                             #
#                                                                          #
# You should have received a copy of the GNU General Public License        #
# along with this program. If not, see <http://www.gnu.org/licenses/>.     #
############################################################################
**/

#pragma once
#include <fstream>
#include "logmessage.h"
#include "logtimer.h"
#include "../thread/spinlock.h"
#include <mutex>
#include <iostream>
//custom macro for filename shorten but only for windows
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
//LINUX
//#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define LOG_ERROR jimdb::common::Logger::Log(jimdb::common::LoggerTypes::ERROR_L,__FILENAME__,__LINE__)
#define LOG_INFO jimdb::common::Logger::Log(jimdb::common::LoggerTypes::INFO,__FILENAME__,__LINE__)
#define LOG_WARN jimdb::common::Logger::Log(jimdb::common::LoggerTypes::WARNING,__FILENAME__,__LINE__)
#define LOG_EXCAPT jimdb::common::Logger::Log(jimdb::common::LoggerTypes::EXCAPTION,__FILENAME__,__LINE__)
#define LOG_DEBUG jimdb::common::Logger::Log(jimdb::common::LoggerTypes::DEBUG,__FILENAME__,__LINE__)

/**
to prevent from copy elision
\see http://stackoverflow.com/questions/30372466/logger-logs-2-times-instead-of-one-time-because-of-copy/30372847#30372847
*/
#define LOG_SCOPE_TIME jimdb::common::LogTimer t___ = jimdb::common::Logger::Timer(__FILENAME__,__LINE__); t___

namespace jimdb
{
    namespace common
    {
        /**
        \brief threadsafe Logger

        It is a threadsafe logger which also can handle scope time logging
        several makros to log are given like LOG_DEBUG LOG_INFO and so on.
        To log a scope time use LOG_SCOPE_TIME and dont forget to scope!

        \author Benjamin Meyer
        */
        class Logger
        {
        public:

            /**
            \brief Get a LogMessage

            Returns a message which can be shifted ostream to with <<.
            The Message will be written to consol and log.
            \author Benjamin Meyer
            \date
            */
            static inline LogMessage Log(LoggerTypes type, const std::string& file,
                                         const int& i);


            /**
            \brief Get a Timer Message to mesure the time of a scope

            Returns a Timer message which can be shifted ostream to with <<.
            The Message will be written to consol and log.

            The message will automatically append the time since creation and leaving
            the local scope. So if use without the macro create a local variable for it.
            \code
            {//start a scope
            	LogTimer t = Logger::Timer(__FILE__,__LINE__);
            	//do some thing wou want to mesure the time of
            } //here it well be logged automatically!
            \endcode
            \author Benjamin Meyer
            \date
            */
            static inline LogTimer Timer(const std::string& file, const int& i);

            static inline Logger& getInstance();

            /**
            static set method for the file so it can be accessed out of the getInstance
            @param[in] filename the new logfile name. Closes the old
            @author Benjamin Meyer
            @date 07.10.2015 11:27
            */
            static void setLogFile(const std::string& filename);

            void setLogLevel(const int& i);
            int getLogLevel() const;

            void inline operator<<(const std::ostringstream& message) const;

        private:
            //default log everything
            Logger() : m_logLevel(DEBUG) { };

            ~Logger();
            //no copy no move no swap if not copy there is no move
            //if copy it generates "move"
            Logger(const Logger&) = delete;
            Logger& operator=(const Logger& other) = delete;

            static std::ofstream* m_file;
            static Logger m_instance;
            static tasking::SpinLock m_lock;
            int m_logLevel;
        };
#include "logger.hpp"
    }
}
