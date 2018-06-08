#pragma once

#include <functional>

#include "network/bin_stream.h"
#include "network/threadsafe_priority_queue.h"

namespace NGAS {

// Mailbox event types
enum MailboxEventType : int32_t { RecvComm = 1, SendComm = 2, Shutdown = 3 };

// Typedefs for send/recv handler
typedef std::function<void(int, BinStream*)> MailboxSendHandlerType;
typedef std::function<void(std::shared_ptr<BinStream>)> MailboxHandlerType;

class MailboxEventBase {
  public:
    MailboxEventBase(MailboxEventType type) : type_(type) {}

    MailboxEventType GetType() { return type_; }

  private:
    MailboxEventType type_;
};

class MailboxEventRecvComm : public MailboxEventBase {
  public:
    MailboxEventRecvComm(std::shared_ptr<BinStream> bin_stream)
      : MailboxEventBase(MailboxEventType::RecvComm), bin_stream_(bin_stream) {}

    std::shared_ptr<BinStream> GetBinStream() { return bin_stream_; }

  private:
    std::shared_ptr<BinStream> bin_stream_;
};

class MailboxEventSendComm : public MailboxEventBase {
  public:
    MailboxEventSendComm(int worker_id, std::shared_ptr<BinStream> payload)
      : MailboxEventBase(MailboxEventType::SendComm), worker_id_(worker_id), payload_(payload) {}

    int GetWorkerId() { return worker_id_; }
    std::shared_ptr<BinStream> GetPayload() { return payload_; }

  private:
    int worker_id_;
    std::shared_ptr<BinStream> payload_;
};

class MailboxEventShutdown : public MailboxEventBase {
  public:
    MailboxEventShutdown() : MailboxEventBase(MailboxEventType::Shutdown) {}
};

typedef ThreadsafePriorityQueue<std::shared_ptr<MailboxEventBase>> MailboxEventQueue;
typedef std::shared_ptr<MailboxEventQueue> MailboxEventQueuePtr;

} // namespace NGAS
