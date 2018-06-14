#include "cluster_info.h"

#include <fstream>
#include <istream>
#include <string>

#include "gflags/gflags.h"
#include "glog/logging.h"

DEFINE_string(workers_info_path, "", "Indicate the path of worker_list.txt");

namespace NGAS {

void ClusterInfo::ParseWorkerInfo() {
  workers_info_path_ = FLAGS_workers_info_path;
  CHECK(not workers_info_path_.empty()) << "Workers info path is empty";

  std::ifstream workers_info_file(workers_info_path_, std::ifstream::in);
  CHECK(workers_info_file.is_open()) << "Cannot Open Worker Info File"
                                     << workers_info_path_;

  CHECK(ParseWorkersInfoWithStream(workers_info_file)) << "Parse Failed";
}

bool ClusterInfo::ParseWorkersInfoWithStream(std::istream &stream) {
  std::string line;
  int wid = 0;
  while (std::getline(stream, line)) {
    if (line[0] == '#' || line.size() == 0)
      continue;

    size_t colon_pos = line.find(':');
    if (colon_pos == std::string::npos)
      return false;

    std::string worker_host = line.substr(0, colon_pos);
    if (worker_host.empty())
      return false;

    int worker_port =
        std::stoi(line.substr(colon_pos + 1, line.size() - colon_pos - 1));

    LOG(INFO) << "Parse Worker : " << wid << ' ' << worker_host << ' '
              << worker_port;

    workers_info_.push_back(WorkerInfo{wid, worker_host, worker_port});
    wid++;
  }

  return !workers_info_.empty();
}

} // namesapce NGAS
