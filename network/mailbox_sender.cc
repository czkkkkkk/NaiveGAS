#include <string>

#include "network/mailbox_sender.h"
#include "network/mailbox_types.h"
#include "zmq_helpers.h"

namespace NGAS {
MailboxSender::MailboxSender(const MailboxAddressBook& addr_book, zmq::context_t* zmq_context)
    : zmq_context_(zmq_context), addr_book_(addr_book) {
  addr_book_.ForEach([=](int worker_id, const std::string& addr) {
    senders_.at(worker_id).reset(new zmq::socket_t(*zmq_context_, zmq::socket_type::push));
    senders_.at(worker_id)->connect(addr);    
  });
}

void MailboxSender::Send(int worker_id, BinStream* payload) {
  auto *sender = senders_.at(worker_id).get();
  zmq_sendmore_int32(sender, MailboxEventType::RecvComm);
  zmq_send_binstream(sender, *payload);
  delete payload;
}

} // namespace NGAS
