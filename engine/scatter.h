#pragma once

#include "data/vertex.h"

#include <thread>
#include <vector>

namespace NGAS {

template <typename VertexT>
void MultiThreadScatter(const std::vector<VertexT> &vertexs) {
  using MsgT = typename VertexT::MsgT;

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
        std::vector<std::pair<IdType, MsgT>> msgvec = vertexs[j].Scatter();
        msgVector.insert(msgVector.end(), msgvec.begin(), msgvec.end());
        // send
      }
      left = right;
    });
  }

  for (auto &thread : threads) {
    thread->join();
  }
}

template <typename VertexT>
void MultiThreadCombineScatter(const std::vector<VertexT> &vertexs) {
  using MsgT = typename VertexT::MsgT;

  std::vector<std::shared_ptr<std::thread>> threads;
  const size_t threadNum = 50;
  threads.resize(threadNum);
  size_t oneThreadVertex = vertexs.size() / threadNum;
  size_t numPlusone = vertexs.size() % threadNum;

  size_t left = 0, right = 0;
  std::vector<std::pair<IdType, MsgT>> msgCombineVector;
  std::vector<std::pair<IdType, MsgT>> msgVector;
  msgVector.resize(threadNum);

  for (size_t i = 0; i < threadNum; i++) {
    threads[i] = std::make_shared<std::thread>([&]() {
      if (i < numPlusone)
        right = left + oneThreadVertex + 1;
      else
        right = left + oneThreadVertex;

      for (size_t j = left; j < right; j++) {
        std::vector<std::pair<IdType, MsgT>> msgvec = vertexs[j].Scatter();
        msgVector[i].insert(msgVector.end(), msgvec.begin(), msgvec.end());
      }
      left = right;
    });
    msgCombineVector.insert(msgCombineVector.end(), msgVector[i].begin(),
                            msgVector[i].end());
  }

  // send

  for (auto &thread : threads) {
    thread->join();
  }
}

} // namespace NGAS
