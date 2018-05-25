#include "engine/scatter.h"

#include <thread>
#include <vector>

namespace NGAS {

template <typename VertexT, typename MsgT>
void MultiThreadScatter(const std::vector<VertexT> &vertexs) {

  std::vector<std::shared_ptr<std::thread>> threads;
  const size_t threadNum = 50;
  threads.resize(threadNum);
  size_t oneThreadVertex = vertexs.size() / threadNum;
  size_t numPlusone = vertexs.size() % threadNum;

  size_t left = 0, right = 0;

  for (size_t i = 0; i < threadNum; i++) {
    threads[i] = std::make_shared<std::thread>([&]() {
      if (i < numPlusone)
        right = left + oneThreadVertex + 1;
      else
        right = left + oneThreadVertex;
      std::vector<std::pair<IdType, MsgT>> msgVector;
      for (size_t j = left; j < right; j++) {
        std::vector<std::pair<IdType, MsgT>> v = vertexs[j].Scatter();
        msgVector.insert(msgVector.end(), v.begin(), v.end());
      }
      left = right;
    });
  }

  for (auto &thread : threads) {
    thread->join();
  }
}

} // namespace NGAS
