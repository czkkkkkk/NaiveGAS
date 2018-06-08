#pragma once

#include <functional>
#include <map>
#include <string>

namespace NGAS {

class MailboxAddressBook {
  public:
    void AddWorker(int worker_id, const std::string& address);
    void RemoveWorker(int worker_id);
    std::string GerAddress(int worker_id);
    void ForEach(std::function<void(int worker, const std::string& address)> handler);
  protected:
    std::map<int, std::string> address_map_;
};

} // namespace NGAS
