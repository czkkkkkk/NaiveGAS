#pragma once

#include <vector>

namespace NGAS {

bool IsActive();

template <typename VertexT, typename MsgT>
void MultiThreadScatter(const std::vector<VertexT>& vertexs);

template <typename VertexT>
void RunJob(){
  std::vector<VertexT> objlist;
  // TODO load data
  // Sync

  while(IsActive()) {
    // Scatter
    for(auto &v: objlist) v.Scatter();
    // Gather

    // Wait for msg
    // Apply
    // Sync
    // Check whether break the loop
    break;
  }
  // Output
}

}  // namespace NGAS
