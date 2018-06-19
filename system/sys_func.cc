#include "sys_func.h"

#include "gflags/gflags.h"
#include "glog/logging.h"

#include "engine/context.h"

DEFINE_int32(worker_id, -1, "global worker id");

namespace NGAS {

void SystemInit(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);

  Context::Init(FLAGS_worker_id, FLAGS_queue, FLAGS_bind_addr, FLAGS_connect_addr, FLAGS_zmq_context);
}

} // namespace NGAS
