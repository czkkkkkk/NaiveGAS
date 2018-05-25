#include "data/vertex.h"

#include <thread>
#include <vector>

namespace NGAS {

template <typename VertexT, typename MsgT>
void MultiThreadScatter(const std::vector<VertexT> &);

} // namespace NGAS
