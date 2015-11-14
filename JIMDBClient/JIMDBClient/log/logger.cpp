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

#include "logger.h"

namespace jimdb
{
    namespace common
    {
        std::ofstream* Logger::m_file = nullptr;
        Logger Logger::m_instance;
        tasking::SpinLock Logger::m_lock;

        void Logger::setLogLevel(const int& i)
        {
            m_logLevel = i;
        }

        int Logger::getLogLevel() const
        {
            return m_logLevel;
        }

        void Logger::setLogFile(const std::string& filename)
        {
            if (m_file != nullptr)
            {
                m_file->flush();
                delete m_file;
            }
            m_file = new std::ofstream(filename, std::ios::out | std::ios::app);
        }

        Logger::~Logger()
        {
            //clean up
            if(m_file != nullptr)
                m_file->flush();
            delete m_file;
        }
    }
}
