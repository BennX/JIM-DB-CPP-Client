/**
############################################################################
# GPL License                                                              #
#                                                                          #
# This file is part of the JIM-DB Client.                                  #
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

#include "jimdbclient.h"
#include "log/logger.h"
//test main
int main(int argc, char* argv[])
{
    jimdb::JIMDBClient l_client("localhost", "6060");
    //dump a million!
    LOG_DEBUG << *l_client.find(156435);
    std::cin.get();
    /**
    for (int i = 0; i < 1000000; i++)
    {

        auto l_res = l_client << std::make_shared<std::string>("{\"Person\":{\"test\":\"striasdfasdngs\"}}");
        //std::this_thread::sleep_for(std::chrono::milliseconds(10));
    	if(i%1000 == 0)
    	{
    		LOG_INFO << *l_res;
    	}
    }
    std::cin.get();
    */
}