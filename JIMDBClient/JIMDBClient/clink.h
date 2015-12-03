// /**
// ############################################################################
// # GPL License                                                              #
// #                                                                          #
// # This file is part of the JIM-DB.                                         #
// # Copyright (c) 2015, Benjamin Meyer, <benjamin.meyer@tu-clausthal.de>     #
// # This program is free software: you can redistribute it and/or modify     #
// # it under the terms of the GNU General Public License as                  #
// # published by the Free Software Foundation, either version 3 of the       #
// # License, or (at your option) any later version.                          #
// #                                                                          #
// # This program is distributed in the hope that it will be useful,          #
// # but WITHOUT ANY WARRANTY; without even the implied warranty of           #
// # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            #
// # GNU General Public License for more details.                             #
// #                                                                          #
// # You should have received a copy of the GNU General Public License        #
// # along with this program. If not, see <http://www.gnu.org/licenses/>.     #
// ############################################################################
// **/
#pragma once
#include <memory>
#include <string>
#define ASIO_STANDALONE
#define ASIO_HAS_STD_CHRONO
#define ASIO_HAS_STD_ARRAY
#define ASIO_HAS_STD_TYPE_TRAITS
#define ASIO_HAS_CSTDINT
#define ASIO_HAS_STD_SHARED_PTR
#define ASIO_HAS_STD_ADDRESSOF
#include <asio.hpp>
namespace jimdb
{
    /**
    \brief CLink Class to connect to the server

    It represents a connection to the server which can
    accept input and output of the server
    @author Benjamin Meyer
    @date 01.12.2015 17:48
    */
    class CLink
    {
    public:
        /**
        \brief ctor

        @param[in] service the asio ioservice
        @param[in] string host the host to connect to
        @param[in] string port the port of the host
        @@param[in] timeout optional timeout in millis
        @author Benjamin Meyer
        @date 01.12.2015 17:47
        */
        explicit CLink(asio::io_service& service, const std::string& host, const std::string& port, const int& i = 1000);
        ~CLink();
        /**
        \brief pass a sharedptr to a string result get appended

        @author Benjamin Meyer
        @date 01.12.2015 17:46
        */
        void operator>>(std::shared_ptr<std::string> get);
        /**
        \brief pass shared_ptr to send

        BODY
        @author Benjamin Meyer
        @date 01.12.2015 17:46
        */
        void operator<<(std::shared_ptr<std::string> tosend);
    private:
        /**
        \brief await operation

        @param[in] time the timeout for example chrono::millies(100)
        @author Benjamin Meyer
        @date 01.12.2015 17:49
        */
        template<typename AllowTime> void await_operation(AllowTime const& deadline_or_duration);
        /**
        \brief read into a buffer

        @param[in] size_t how much byte to read
        @return a ptr to the buffer. need to be deleted!
        @author Benjamin Meyer
        @date 01.12.2015 17:49
        */
        char* read(const size_t& count);

        asio::ip::tcp::socket m_socket;
        int m_timeout;
		bool m_connected;
    };
}
#include "clink.hpp"