#include <memory>
#include <functional>

#include "network/mailbox_eventloop.h"
#include "zmq_helpers.h"
#include "network/mailbox_types.h"

namespace NGAS {

  void MailboxEventLoop::SetHandler(MailboxEventType event_type, MailboxHandlerType handler) {
   handlers_[event_type] = handler; 
  }

  void MailboxEventLoop::OnSend(MailboxSendHandlerType handler) { send_handler_ = handler; }

  MailboxEventLoop::MailboxEventLoop(MailboxEventQueuePtr queue) : queue_(queue) {
    thread_.reset(new std::thread([=]() {
      bool shutdown = false;
      while(!shutdown) {
        auto event_base = queue_->WaitAndPop();
        switch (event_base->GetType()) {
          case MailboxEventType::RecvComm: {
            auto event = std::static_pointer_cast<MailboxEventRecvComm>(event_base);
            std::shared_ptr<BinStream> bin_stream = event->GetBinStream();
            if(handlers_.find(MailboxEventType::RecvComm) != handlers_.end()) {
              handlers_.at(MailboxEventType::RecvComm)(bin_stream);
            } else {
              cached_comm_.at(MailboxEventType::RecvComm).push_back(bin_stream);
            }
            break;
          }
          case MailboxEventType::SetRecvHandler: {
            auto event = std::static_pointer_cast<MailboxEventSetRecvHandler>(event_base);
            MailboxHandlerType* handler = event->GetHandler();
            handlers_[MailboxEventType::RecvComm] = *handler;
            
            if(cached_comm_.find(MailboxEventType::RecvComm) != cached_comm_.end()) {
              for(auto& payload : cached_comm_.at(MailboxEventType::RecvComm)) {
                handlers_.at(MailboxEventType::RecvComm)(payload);
              }
            }
            break;
          }                                  
          case MailboxEventType::SendComm: {
            auto event = std::static_pointer_cast<MailboxEventSendComm>(event_base);
            int worker_id = event->GetWorkerId();
            std::shared_ptr<BinStream> payload = event->GetPayload();
            send_handler_(worker_id, payload.get());
            break;
          }
          case MailboxEventType::Shutdown: {
            shutdown = true;
            break;
          }                              
        }
      }
      }));
  }

  MailboxEventLoop::~MailboxEventLoop() {
    auto event = std::make_shared<MailboxEventShutdown>();
    queue_->Push(event);
    thread_->join();
  }

} // namespace NGAS
