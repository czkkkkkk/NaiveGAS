#include "sys_func.h"

#include "gflags/gflags.h"
#include "glog/logging.h"

#include "engine/context.h"

DEFINE_int32(worker_id, -1, "global worker id");

namespace NGAS {

void SystemInit(int argc, char *argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);

  DLOG(INFO) << "Start Init Context";
  google::FlushLogFiles(google::INFO);

  Context::Init(FLAGS_worker_id);
}

} // namespace NGAS
