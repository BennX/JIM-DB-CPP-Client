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
#pragma once
#include "jimdbclient.h"
#include "log/logger.h"
//test main
int main(int argc, char* argv[])
{


    std::ifstream t("Persons.json");
    std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());

    rapidjson::Document doc;
    doc.Parse(str.c_str());
    if (doc.HasParseError())
        return EXIT_FAILURE;

    std::vector<std::shared_ptr<std::string>> l_persons;

    for (auto it = doc.Begin(); it != doc.End(); ++it)
    {
        l_persons.push_back(jimdb::JIMDBClient::toString(*it));
    }
    /**
    asio::io_service serv;
    jimdb::CLink link(serv, "127.0.0.1", "6060");
    auto recv = std::make_shared<std::string>("");
    link >> recv;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    LOG_DEBUG << *recv;
    link << recv;
    std::cin.get();
    */


    LOG_INFO << l_persons.size();
    //dump a million!
    //std::cin.get();

    jimdb::JIMDBClient l_client("localhost", "6060");

    //throw in a million
    for (int i = 0; i < 100; ++i)
    {
        for (int j = 0; j < 10000; ++j)
        {
            try
            {

                auto recv = l_client << l_persons[j];
                if(j % 10000 == 0)
                    LOG_ERROR << *recv;
            }
            catch(std::runtime_error& e)
            {
                LOG_ERROR << e.what();
            }
        }
    }
    //last but not least force a dump out of the results of the benchmark
    //l_client.send(std::make_shared<std::string>("{\"type\":\"bench\"}"));
    std::stringstream ss;
    std::string l_string;
    while (true)
    {
        std::getline(std::cin, l_string);
        int l_oid;
        ss << l_string;
        ss >> l_oid;
        ss.clear();
        l_string.clear();

        LOG_DEBUG << *l_client.find(l_oid);
    }
}