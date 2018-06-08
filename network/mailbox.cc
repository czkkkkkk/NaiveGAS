#include "network/mailbox.h"
#include "network/zmq_helpers.h"

namespace NGAS {

Mailbox::Mailbox(MailboxEventQueuePtr queue) : queue_(queue) {}

Mailbox::~Mailbox() {}

void Mailbox::Send(int worker_id, BinStream* payload, int priority) {
  auto event = std::make_shared<MailboxEventSendComm>(worker_id, std::shared_ptr<BinStream>(payload), priority);
  queue_->Push(event);
}

} // namespace NGAS

