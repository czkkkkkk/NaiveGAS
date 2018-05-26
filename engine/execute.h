#pragma once

#include <vector>

#include "engine/scatter.h"

namespace NGAS {

bool IsActive();

template <typename VertexT> void RunJob() {
  std::vector<VertexT> objlist;
  // TODO load data
  // Sync

  while (IsActive()) {
    // Scatter
    MultiThreadScatter(objlist);
    // Gather

    // Wait for msg
    // Apply
    // Sync
    // Check whether break the loop
    break;
  }
  // Output
}

} // namespace NGAS
