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
#define MESSAGE_SIZE 8
#include "log/logger.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

namespace jimdb
{

    JIMDBClient::JIMDBClient(const std::string& nameorAdd, const std::string& port) : m_host(nameorAdd), m_port(port),
        m_connected(false)
    {
    }


    JIMDBClient::~JIMDBClient()
    {
        close();
        WSACleanup();
    }

    std::shared_ptr<std::string> JIMDBClient::operator<<(std::shared_ptr<std::string> json)
    {
        rapidjson::Document l_doc;
        l_doc.Parse(json->c_str());
        if (l_doc.HasParseError())
            return std::make_shared<std::string>("");

        rapidjson::Document l_toSend;
        l_toSend.SetObject();

        l_toSend.AddMember("type", "insert", l_toSend.GetAllocator());
        l_toSend.AddMember("data", l_doc, l_toSend.GetAllocator());

        //convert to string
        rapidjson::StringBuffer strbuf;
        rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
        l_toSend.Accept(writer);
        auto l_message = std::make_shared<std::string>(strbuf.GetString());

        auto l_result = std::make_shared<std::string>();
        if (!connect())
        {
            close();
            return std::make_shared<std::string>("");
        }

        sendData(l_message);

        //now get the result if it didnt fail
        //if (hasData())
        l_result = getData();

        //close after sending
        close();
        return l_result;
    }

    bool JIMDBClient::sendData(std::shared_ptr<std::string> json)
    {
        char l_length[MESSAGE_SIZE + 1];
        if (json == nullptr)
            return false;
        sprintf(l_length, "%8d", static_cast<int>(json->size()));

        auto l_message = std::string(l_length);
        l_message += *json; //add the message
        int iSendResult;

        iSendResult = ::send(m_sock, l_message.c_str(), l_message.size(), 0);
        if (iSendResult == SOCKET_ERROR)
        {
            //TODO THROW
            close();
            return false;
        }
        return true;
    }

    bool JIMDBClient::hasData()
    {
        fd_set temp;
        FD_ZERO(&temp);
        FD_SET(m_sock, &temp);

        //setup the timeout to 1000ms
        struct timeval tv;
        tv.tv_sec = 5;
        tv.tv_usec = 1000;

        if (select(m_sock + 1, &temp, nullptr, nullptr, &tv) == -1)
        {
            return false;
        }

        if (FD_ISSET(m_sock, &temp))
            return true;

        return false;
    }

    bool JIMDBClient::connect()
    {
        WSADATA wsaData;
        m_sock = INVALID_SOCKET;
        struct addrinfo* result = nullptr, *ptr = nullptr, hints;

        int iResult;

        /* Initialisiere TCP für Windows ("winsock"). */
        WORD wVersionRequested;

        // Initialize Winsock
        iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0)
        {
            m_connected = false;
            return false;
        }

        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        // Resolve the server address and port
        iResult = getaddrinfo(m_host.c_str(), m_port.c_str(), &hints, &result);
        if (iResult != 0)
        {
            m_connected = false;
            return false;
        }

        // Attempt to connect to an address until one succeeds
        for (ptr = result; ptr != nullptr; ptr = ptr->ai_next)
        {
            // if (ptr->ai_protocol == 6 && ptr->ai_family == 23 && ptr->ai_socktype == 1)
            //   continue;
            // Create a SOCKET for connecting to server
            m_sock = socket(ptr->ai_family, ptr->ai_socktype,
                            ptr->ai_protocol);

            if (m_sock == INVALID_SOCKET)
            {
                m_connected = false;
                return false;
            }

            // Connect to server.
            iResult = ::connect(m_sock, ptr->ai_addr, static_cast<int>(ptr->ai_addrlen));
            if (iResult == SOCKET_ERROR)
            {
                closesocket(m_sock);
                m_sock = INVALID_SOCKET;
                continue;
            }
            break;
        }

        freeaddrinfo(result);

        if (m_sock == INVALID_SOCKET)
        {
            WSACleanup();
            m_connected = false;
            return false;
        }

        if(!handShake())
        {
            LOG_WARN << "handshake failed";
            return false;
        }

        m_connected = true;
        return m_connected;
    }

    bool JIMDBClient::handShake()
    {
        if(hasData())
        {
            //simply echo back here
            auto l_result = getData();
            if (l_result == nullptr)
                return false;

            sendData(l_result);
            return true;
        }
        return false;
    }

    void JIMDBClient::close()
    {
        if (m_connected)
        {
            closesocket(m_sock);
            m_connected = false;
        }
    }

    std::shared_ptr<std::string> JIMDBClient::getData()
    {
        auto l_result = std::make_shared<std::string>();
        int n;

        char size[MESSAGE_SIZE + 1];
        n = recv(m_sock, size, sizeof(size) - 1, 0);
        size[MESSAGE_SIZE] = '\0';

        //check if retval passed
        if (!checkRetValRecv(n))
            return l_result; //should be empty

        //calc how much data
        auto msgLen = atoi(size);
        //create buffer for the data
        auto buffer = new char[msgLen + 1];
        buffer[msgLen] = '\0';
        //read buffer
        n = recv(m_sock, buffer, msgLen, 0);


        //check retval if it passes
        if (!checkRetValRecv(n))
            return l_result;//should be empty

        l_result->append(buffer, msgLen);

        delete[] buffer;
        return l_result;
    }

    bool JIMDBClient::checkRetValRecv(const int& n)
    {
        if (n == SOCKET_ERROR)
        {
            size_t err = WSAGetLastError();
            if (err == WSAECONNRESET)
            {
                LOG_ERROR << "Failed to recv! Disconnecting from client: " << "Connection reset by peer.";
            }
            else
            {
                LOG_ERROR << "Disconnecting from client. WSAError: " << err;
            }
            m_connected = false;
            return false;
        }

        if (n == 0)
        {
            LOG_INFO << "Client disconnected.";
            m_connected = false;
            return false;
        }
        return true;
    }
}