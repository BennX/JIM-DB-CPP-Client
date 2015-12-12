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

#include "CmdArgs.h"
#include <iostream>
#include "log/logger.h"

namespace jimdb
{
    namespace common
    {
        CmdArgs CmdArgs::m_instance;

        void CmdArgs::init(const int& argc, char* argv[])
        {
            //pushback all values
            std::string temp;
            for (auto i = 1; i < argc; ++i)
            {
                temp.clear();
                temp = argv[i];
                if(temp.at(0) == '-')
                {
                    //check if thers a next and if its not a argument
                    //todo add validation else -h can have a argument?!
                    if (i + 1 < argc && std::string(argv[i + 1]).at(0) != '-')
                    {
                        m_args[temp] = argv[i + 1];
                        ++i;//skip one
                    }
                    else
                    {
                        //no next or no argument
                        m_args[temp] = "";
                    }
                }
                else
                {
                    //else we got 2 times a non argument which we skip
                    LOG_WARN << "Invalid argument: " << temp;
                }
            }
            //todo validate ?!
        }

        CmdArgs& CmdArgs::getInstance()
        {
            return m_instance;
        }

        CmdArgs::~CmdArgs() {}

        std::string CmdArgs::operator[](const std::string& v)
        {
            if(m_args[v] != "")
            {
                return m_args[v]; //return the value
            }
            auto error = v;
            error += " needs a second parameter for example like the config: -config FILENAME";
            throw std::runtime_error(error);
        }

        bool CmdArgs::contains(const std::string& v) const
        {
            if (m_args.count(v))
            {
                return true;
            }
            return false;
        }

        std::ostream& operator<<(std::ostream& os, const CmdArgs& p)
        {
            os << "Arguments:";
            for (auto it = p.m_args.begin(); it != p.m_args.end(); ++it)
            {
                os << std::endl;
                os << it->first << ":" << it->second;
            }
            return os;
        }
    }
}
