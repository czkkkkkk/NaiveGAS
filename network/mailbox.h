#pragma once

#include <condition_variable>
#include <functional>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

#include "zmq.hpp"

#include "network/mailbox_types.h"
#include "network/threadsafe_priority_queue.h"
#include "network/bin_stream.h"

namespace NGAS {

class Mailbox {
  public:
    explicit Mailbox(MailboxEventQueuePtr queue);
    virtual ~Mailbox();

    void Send(int worker_id, BinStream* payload);
  protected:
    MailboxEventQueuePtr queue_;
};

} // namespace NGAS

