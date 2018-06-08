#include "network/mailbox_adress_book.h"

#include <string>

namespace NGAS {

void MailboxAddressBook::AddWorker(int worker_id, const std::string& address) { address_map_[worker_id] = address; }

void MailboxAddressBook::RemoveWorker(int worker_id) { address_map_.erase(worker_id); }

std::string MailboxAddressBook::GerAddress(int worker_id) { return address_map_.at(worker_id); }

void MailboxAddressBook::ForEach(std::function<void(int worker_id, const std::string& addr)> handler) {
  for(auto& pair : address_map_) {
    handler(pair.first, pair.second);
  }
}

} // namespace NGAS
