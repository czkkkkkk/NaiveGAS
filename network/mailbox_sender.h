#pragma once

#include <memory>
#include <unordered_map>

#include "zmq.hpp"

#include "network/bin_stream.h"
#include "network/mailbox_adress_book.h"

namespace NGAS {

class MailboxSender {
  public:
    MailboxSender(const MailboxAddressBook& addr_book, zmq::context_t* zmq_context);

    void Send(int worker_id, BinStream *payload);

  protected:
    MailboxAddressBook addr_book_;
    zmq::context_t* zmq_context_;
    std::unordered_map<int, std::shared_ptr<zmq::socket_t>> senders_;
};

} // namespace NGAS
