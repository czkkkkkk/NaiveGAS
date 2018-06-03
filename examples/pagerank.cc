#include <iostream>

#include "data/edge.h"
#include "data/vertex.h"
#include "engine/execute.h"

#include "gflags/gflags.h"
#include "glog/logging.h"

using namespace NGAS;

DEFINE_int32(process_id, 0, "process_id");
DEFINE_string(app_scratch_dir, "/tmp/zekucai", "scratch_dir");
DEFINE_string(workers_info_path, "/tmp/zekucai", "worker_info_path");

class PREdge : public Edge {
public:
  PREdge(IdType src, IdType dst) : Edge(src, dst) {}
};

class PRVertex : public Vertex<double, PREdge> {
public:
  using MsgT = double;
  using EdgeT = PREdge;

  PRVertex(IdType id) : Vertex(id) {}

  void Combine(double &lhs, const double &rhs) override { lhs += rhs; }
  void Apply(const double &msg) override { pr_value_ = msg; }
  std::vector<std::pair<IdType, double>>
  Scatter(const EdgeT &edge) const override {
    return {};
  }

private:
  double pr_value_;
};

int main(int argc, char *argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);

  LOG(INFO) << "Running Job PageRank";
  RunJob<PRVertex>();

  google::FlushLogFiles(google::INFO);

  gflags::ShutDownCommandLineFlags();
  return 0;
}
