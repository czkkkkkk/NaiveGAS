#pragma once

#include <functional>
#include <thread>
#include <unordered_map>

#include "zmq.hpp"

#include "network/bin_stream.h"
#include "network/mailbox_adress_book.h"
#include "network/mailbox_types.h"


namespace NGAS {

class MailboxEventLoop {
  public:
    explicit MailboxEventLoop(MailboxEventQueuePtr queue);
    ~MailboxEventLoop();

    void OnSend(MailboxSendHandlerType handler);
    void SetHandler(MailboxEventType event_type, MailboxHandlerType handler);
  
  protected:
    std::unique_ptr<std::thread> thread_;
    MailboxEventQueuePtr queue_;
    std::unordered_map<int, MailboxHandlerType> handlers_;
    std::unordered_map<int, std::vector<std::shared_ptr<BinStream>>> cached_comm_;
    MailboxSendHandlerType send_handler_;
};

} // namespace NGAS
