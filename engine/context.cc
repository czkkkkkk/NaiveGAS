#include "engine/context.h"

namespace NGAS {

std::unique_ptr<ContextData> Context::context_data_;

void Context::Init(int worker_id, MailboxEventQueuePtr queue, const std::string& bind_addr, const std::string& connect_addr, zmq::context_t* zmq_context){
  context_data_.reset(new ContextData());
  context_data_->worker_id = worker_id;
  context_data_->mailbox.reset(new Mailbox(queue));
  context_data_->event_loop.reset(new MailboxEventLoop(queue));
  context_data_->recver.reset(new MailboxRecver(bind_addr, connect_addr, zmq_context, queue));
  context_data_->addr_book.reset(new MailboxAddressBook());
  context_data_->sender.reset(new MailboxSender(*context_data_->addr_book.get(), zmq_context));
}

} // namespace NGAS
