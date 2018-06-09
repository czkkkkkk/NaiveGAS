#pragma once

#include <memory>

namespace NGAS {

struct ContextData {
  int worker_id;
};

class Context {
public:
  static void Init(int worker_id);
  static int GetWorkerId() { return context_data_->worker_id; }

protected:
  static std::unique_ptr<ContextData> context_data_;
};

} // namespace NGAS
