#pragma once

#include <string>
#include <thread>

#include "zmq.hpp"

#include "network/bin_stream.h"
#include "network/mailbox_types.h"


namespace NGAS {

class MailboxRecver {
  public:
    MailboxRecver(const std::string& bind_addr, const std::string& connect_addr, zmq::context_t* zmq_context,
                  MailboxEventQueuePtr queue);
    ~MailboxRecver();
  
  protected:
    std::unique_ptr<std::thread> thread_;
    zmq::context_t* zmq_context_;
    MailboxEventQueuePtr queue_;
    std::string bind_addr_;
    std::string connect_addr_;
};

} // namespace NGAS
