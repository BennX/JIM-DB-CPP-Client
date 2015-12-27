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
#include "cmdargs.h"
#include <random>
#define FIND
//test main
int main(int argc, char* argv[])
{
    jimdb::common::CmdArgs::getInstance().init(argc, argv);
    auto& args = jimdb::common::CmdArgs::getInstance();
#ifdef INSERT
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
#endif
    jimdb::JIMDBClient l_client("192.168.2.101", "6060");

    std::stringstream ss;
    ss << args["-count"];
    int count;
    ss >> count;
    LOG_DEBUG << "COUNT " << count;
#ifdef INSERT

    for (auto i = 0; i < count; ++i)
    {
        auto temp = l_client << l_persons[(i % l_persons.size())];
        if (i % 50000 == 0)
            LOG_DEBUG << *temp;
    }
#endif
#ifdef FIND

    ss.clear();
    ss << args["-findMax"];
    int findMax;
    ss >> findMax;
    LOG_DEBUG << "MAX " << findMax;
    std::mt19937 twister(std::chrono::system_clock::now().time_since_epoch().count());
    for (auto i = 0; i < 100; i++)
        twister();

    for (auto j = 0; j < count; ++j)
    {
        auto res = l_client.find(twister() % findMax);
        if (j % 50000 == 0)
            LOG_DEBUG << *res;
    }
#endif
#ifdef TEST
    //last but not least force a dump out of the results of the benchmark
    //l_client.send(std::make_shared<std::string>("{\"type\":\"bench\",\"data\":{}}"));



    std::stringstream ssss;
    std::string l_string;
    while (true)
    {
        std::getline(std::cin, l_string);
        int l_oid;
        ssss << l_string;
        ssss >> l_oid;
        ssss.clear();
        l_string.clear();

        LOG_DEBUG << *l_client.find(l_oid);
    }
#endif
}