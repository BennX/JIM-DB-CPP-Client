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
#include <map>

namespace jimdb
{
    namespace common
    {
        /**
        \brief Class to parse and obtain cmd arguments

        @author Benjamin Meyer
        @date 05.10.2015 12:58
        */
        class CmdArgs
        {
        public:

            static CmdArgs& getInstance();
            void init(const int& argc, char* argv[]);
            ~CmdArgs();

            std::string operator[](const std::string& v);

            /**
            @return true if args contain a value for example --h
            @author Benjamin Meyer
            @date 05.10.2015 12:56
            */
            bool contains(const std::string& v) const;

            friend std::ostream& operator<<(std::ostream& os, const CmdArgs& p);
        private:
            std::map<std::string, std::string> m_args;
            static CmdArgs m_instance;
        };
    }
}
