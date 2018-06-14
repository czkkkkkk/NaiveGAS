#include "engine/context.h"

namespace NGAS {

std::unique_ptr<ContextData> Context::context_data_;
ClusterInfo Context::cluster_info_;

void Context::Init(int worker_id) {
  context_data_.reset(new ContextData());
  context_data_->worker_id = worker_id;
  cluster_info_.ParseWorkerInfo();
}

} // namespace NGAS
