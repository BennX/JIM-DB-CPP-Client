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

#include "clink.h"
#include "rapidjson/document.h"

namespace jimdb
{

    enum MessageTypes
    {
        INSERT,
        DEL,
        FIND,
        ENUM_SIZE,
    };

    struct MessageTypeMap
    {
        //need the strings for the size
        static const char* EnumString[];

        /**
        @return the configValue as const char*
        */
        static const char* get(const MessageTypes& e);
    };

    class JIMDBClient
    {
    public:
        JIMDBClient(const std::string& nameorAdd, const std::string& port);

        /**
        \brief send a JSON Object and get the result

        @throws std::runtime_error if something goes wrong or an error is returned
        @return the result which the server send back
        @author Benjamin Meyer
        @date 14.11.2015 12:36
        */
        std::shared_ptr<std::string> operator<<(std::shared_ptr<std::string> json);

        std::shared_ptr<std::string> find(uint64_t oid);

        //does a blank send no wrapping or something else!!
        std::shared_ptr<std::string> send(std::shared_ptr<std::string> json);

        /**
        \brief generates a string from document

        @param[in] rapidjsonValue the value to parse to a string
        @return sptr to the string
        @author Benjamin Meyer
        @date 01.12.2015 17:53
        */
        static std::shared_ptr<std::string> toString(rapidjson::Value& data);

    private:
        std::string m_host;
        std::string m_port;
        asio::io_service m_service;
		CLink m_link;

        /**
        \brief do the handshake

        @author Benjamin Meyer
        @date 14.11.2015 12:34
        */
        bool handShake(CLink& link) const;


        std::shared_ptr<std::string> generateFind(const uint64_t& oid) const;
        /**
        \brief generate an insert message

        @param[in] sptr to string the json object to send
        @return the generated insert message as string
        @author Benjamin Meyer
        @date 01.12.2015 17:55
        */
        std::shared_ptr<std::string> generateInsert(std::shared_ptr<std::string> json) const;

        /**
        \brief generate a message to send to the server

        @param[in] MessageType the type of the message for example inser/delete
        @param[in] rapidjson::Document the document which get into the data field
        @return the json of the generated message
        @author Benjamin Meyer
        @date 01.12.2015 17:54
        */
        std::shared_ptr<std::string> generate(const MessageTypes& t,  rapidjson::Value& data) const;
    };
}