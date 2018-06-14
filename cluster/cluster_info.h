#pragma once

#include <istream>
#include <vector>

namespace NGAS {

struct WorkerInfo {
  int worker_id;
  std::string host_name;
  int port;
};

class ClusterInfo {
public:
  void ParseWorkerInfo();

protected:
  bool ParseWorkersInfoWithStream(std::istream &stream);

private:
  std::vector<WorkerInfo> workers_info_;
  std::string workers_info_path_;
};

} // namespace NGAS
