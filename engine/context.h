#pragma once

#include <memory>

#include "zmq.hpp"

#include "network/mailbox.h"
#include "network/mailbox_adress_book.h"
#include "network/mailbox_eventloop.h"
#include "network/mailbox_recver.h"
#include "network/mailbox_sender.h"
#include "network/mailbox_types.h"
#include "network/threadsafe_priority_queue.h"

namespace NGAS {

struct ContextData {
  int worker_id;
  std::unique_ptr<Mailbox> mailbox;
  std::unique_ptr<MailboxAddressBook> addr_book;
  std::unique_ptr<MailboxEventLoop> event_loop;
  std::unique_ptr<MailboxSender> sender;
  std::unique_ptr<MailboxRecver> recver;
};

class Context {
public:
  static void Init(int worker_id, MailboxEventQueuePtr queue, const std::string& bind_addr, const std::string& connect_addr, zmq::context_t* zmq_context);
  static int GetWorkerId() { return context_data_->worker_id; }
  static std::unique_ptr<Mailbox> GetMailbox() { return std::move(context_data_->mailbox); }
  static std::unique_ptr<MailboxEventLoop> GetMailboxEventLoop() {
    return std::move(context_data_->event_loop);
  }
  static std::unique_ptr<MailboxSender> GetMailboxSender() { return std::move(context_data_->sender); }
  static std::unique_ptr<MailboxRecver> GetMailboxRecver() { return std::move(context_data_->recver); }
  static std::unique_ptr<MailboxAddressBook> GetMailboxAddressBook() {
    return std::move(context_data_->addr_book);
  }

protected:
  static std::unique_ptr<ContextData> context_data_;
};

} // namespace NGAS
