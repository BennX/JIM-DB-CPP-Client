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
#include "logmessage.h"
#include <chrono>
#include <string>
namespace jimdb
{
    namespace common
    {
        /**
        \brief Extends the regular LogMessage by a µs timer

        It starts a timer and ends it when its distroyed. While it is Distroyed the
        time between start and end is taken and appended to the log data to know the execution time of the
        current scope.

        Use it like this
        \code
        {
        	LogTimer timer;
        	timer << "something";
        	//do something you want to mesure
        }//here it will be logged
        \endcode
        \author Benjamin Meyer
        */
        class LogTimer : public LogMessage
        {
        public:

            explicit inline LogTimer(const LoggerTypes& type, const std::string& file = "",
                              const int& i = 0);
            ~LogTimer();

            template <typename T>
            LogTimer& operator<<(const T& m);

            LogTimer(const LogTimer& other);
            LogTimer(LogTimer&& other);

            inline LogTimer& operator=(const LogTimer& other);
            inline LogTimer& operator=(LogTimer&& other);
        private:
            std::chrono::high_resolution_clock::time_point m_start;
        };
#include "logtimer.hpp"
    }
}
