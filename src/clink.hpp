#pragma once
namespace jimdb
{
    template <typename AllowTime>
    void CLink::await_operation(AllowTime const& deadline_or_duration)
    {
        using namespace asio;
        auto& ioservice = m_socket.get_io_service();
        ioservice.reset();
        {
            asio::steady_timer tm(ioservice, deadline_or_duration);
            tm.async_wait([this](std::error_code ec)
            {
                //timer was not cancled so it run out
                if (ec != error::operation_aborted)
                {
                    m_socket.cancel();
                }
            });
            ioservice.poll_one();
        }
        ioservice.run();
    }
}