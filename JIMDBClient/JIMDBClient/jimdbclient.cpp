#pragma once
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
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

namespace jimdb
{
    const char* MessageTypeMap::EnumString[] =
    {
        "insert",
        "delete",
        "find"
    };

    static_assert(sizeof(MessageTypeMap::EnumString) / sizeof(char*) == ENUM_SIZE, "size dont match!");

    const char* MessageTypeMap::get(const MessageTypes& e)
    {
        return EnumString[e];
    }

    JIMDBClient::JIMDBClient(const std::string& nameorAdd, const std::string& port) : m_host(nameorAdd), m_port(port) { }


    JIMDBClient::~JIMDBClient() {}

    std::shared_ptr<std::string> JIMDBClient::operator<<(std::shared_ptr<std::string> json)
    {
        CLink link(m_service, m_host, m_port);

        if(handShake(link))
        {
            link << generateInsert(json);
            auto recv = std::make_shared<std::string>("");
            link >> recv;

            if (*recv == "")
                throw std::runtime_error("io error orruced");

            //check if there is a issue
            rapidjson::Document doc;
            doc.Parse(recv->c_str());
            if (!doc.HasParseError() && doc["type"].GetString() == "error")
                throw std::runtime_error(doc["data"]["what"].GetString());

            return recv;
        }
        return nullptr;
    }

    std::shared_ptr<std::string> JIMDBClient::find(uint64_t oid)
    {
        CLink link(m_service, m_host, m_port);

        if (handShake(link))
        {
            link << generateFind(oid);
            auto recv = std::make_shared<std::string>("");

            link >> recv;
            if (*recv == "")
                throw std::runtime_error("io error orruced");
			return recv;
            //check if there is a issue
            rapidjson::Document doc;
            doc.Parse(recv->c_str());
            if (doc.HasParseError())
				throw std::runtime_error("parse");
            if(std::string("error") == doc["type"].GetString())
                throw std::runtime_error(doc["data"]["what"].GetString());

            return recv;
        }
    }

    bool JIMDBClient::handShake(CLink& link) const
    {
        auto recv = std::make_shared<std::string>("");
        link >> recv;
        if (*recv == "")
            return false;
        //echo back
        link << recv;
        return true;
    }

    std::shared_ptr<std::string> JIMDBClient::generateFind(const uint64_t& oid) const
    {
        rapidjson::Document doc;
        doc.SetObject();
        doc.AddMember("oid__", oid, doc.GetAllocator());
        return generate(FIND, doc);
    }

    std::shared_ptr<std::string> JIMDBClient::generateInsert(std::shared_ptr<std::string> json) const
    {
        rapidjson::Document doc;
        doc.Parse(json->c_str());
        if (doc.HasParseError())
            return nullptr;
        return generate(INSERT, doc);
    }

    std::shared_ptr<std::string> JIMDBClient::generate(const MessageTypes& t, rapidjson::Value& data) const
    {
        rapidjson::Document doc;
        doc.SetObject();
        doc.AddMember("type", rapidjson::Value(MessageTypeMap::get(t), doc.GetAllocator()), doc.GetAllocator());
        doc.AddMember("data", data, doc.GetAllocator());
        return toString(doc);
    }


    std::shared_ptr<std::string> JIMDBClient::toString(rapidjson::Value& data) const
    {
        // Convert JSON document to string
        rapidjson::StringBuffer strbuf;
        rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
        data.Accept(writer);
        return std::make_shared<std::string>(strbuf.GetString());
    }
}