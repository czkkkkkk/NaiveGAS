#pragma once

#include <functional>

#include "network/bin_stream.h"
#include "network/threadsafe_priority_queue.h"

namespace NGAS {

// Mailbox event types
enum MailboxEventType : int32_t { RecvComm = 1, SetRecvHandler = 2, SendComm = 3, Shutdown = 4 };

// Typedefs for send/recv handler
typedef std::function<void(int, BinStream*)> MailboxSendHandlerType;
typedef std::function<void(std::shared_ptr<BinStream>)> MailboxHandlerType;

class MailboxEventBase {
  public:
    MailboxEventBase(MailboxEventType type, int priority) : type_(type), priority_(priority) {}

    MailboxEventType GetType() { return type_; }
    int GetPriority() { return priority_; }

  private:
    MailboxEventType type_;
    int priority_;
};

class MailboxEventRecvComm : public MailboxEventBase {
  public:
    MailboxEventRecvComm(std::shared_ptr<BinStream> bin_stream)
      : MailboxEventBase(MailboxEventType::RecvComm, -1), bin_stream_(bin_stream) {}

    std::shared_ptr<BinStream> GetBinStream() { return bin_stream_; }

  private:
    std::shared_ptr<BinStream> bin_stream_;
};

class MailboxEventSetRecvHandler : public MailboxEventBase {
  public:
    MailboxEventSetRecvHandler(MailboxHandlerType* handler)
      : MailboxEventBase(MailboxEventType::SetRecvHandler, -1), handler_(handler) {}

    MailboxHandlerType* GetHandler() { return handler_; }

  private:
    MailboxHandlerType* handler_;
};

class MailboxEventSendComm : public MailboxEventBase {
  public:
    MailboxEventSendComm(int worker_id, std::shared_ptr<BinStream> payload, int priority)
      : MailboxEventBase(MailboxEventType::SendComm, priority), worker_id_(worker_id), payload_(payload) {}

    int GetWorkerId() { return worker_id_; }
    std::shared_ptr<BinStream> GetPayload() { return payload_; }

  private:
    int worker_id_;
    std::shared_ptr<BinStream> payload_;
};

class MailboxEventShutdown : public MailboxEventBase {
  public:
    MailboxEventShutdown() : MailboxEventBase(MailboxEventType::Shutdown, -1) {}
};

typedef ThreadsafePriorityQueue<std::shared_ptr<MailboxEventBase>> MailboxEventQueue;
typedef std::shared_ptr<MailboxEventQueue> MailboxEventQueuePtr;

} // namespace NGAS
