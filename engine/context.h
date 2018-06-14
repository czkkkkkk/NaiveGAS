#pragma once

#include <memory>

#include "cluster/cluster_info.h"

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
  static ClusterInfo cluster_info_;
};

} // namespace NGAS
