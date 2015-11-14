#pragma once
#include <string>

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

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#pragma comment(lib, "Ws2_32.lib")
#include <string>
#include <memory>

namespace jimdb
{
    class JIMDBClient
    {
    public:
        JIMDBClient(const std::string& nameorAdd, const std::string& port);
        ~JIMDBClient();
        /**
        \brief send a json and get the result

        @author Benjamin Meyer
        @date 14.11.2015 12:36
        */
        std::shared_ptr<std::string> operator<<(std::shared_ptr<std::string> json);

        /**
        \brief checks for data 1s

        @author Benjamin Meyer
        @date 14.11.2015 12:35
        */
        bool hasData();
    private:
        SOCKET m_sock;
        std::string m_host;
        std::string m_port;
        bool m_connected;

        /**
        \brief connect to the host and port

        @author Benjamin Meyer
        @date 14.11.2015 12:28
        */
        bool connect();

        /**
        \brief do the handshake

        @author Benjamin Meyer
        @date 14.11.2015 12:34
        */
        bool handShake();

        /**
        \brief close the socket if connected

        @author Benjamin Meyer
        @date 14.11.2015 12:28
        */
        void close();

        /**
        \brief method that get data if there are data

        BODY
        @author Benjamin Meyer
        @date 14.11.2015 12:39
        */
        std::shared_ptr<std::string> getData();

        /**
        \brief send raw data

        @author Benjamin Meyer
        @date 14.11.2015 12:36
        */
        bool sendData(std::shared_ptr<std::string> json);


        bool checkRetValRecv(const int& n);
    };
}