#pragma once

#include "data/vertex.h"
#include "glog/logging.h"

#include <algorithm>
#include <chrono>
#include <functional>
#include <random>
#include <thread>
#include <vector>

namespace NGAS {

template <typename VertexT>
auto MultiThreadScatter(const std::vector<VertexT> &vertexs) {
  using MsgT = typename VertexT::MsgT;

  std::vector<std::shared_ptr<std::thread>> threads;
  const size_t thread_num = 50;
  threads.resize(thread_num);
  size_t one_thread_vertex = vertexs.size() / thread_num;
  size_t num_plus_one = vertexs.size() % thread_num;

  std::vector<std::vector<std::pair<IdType, MsgT>>> msgs(thread_num);

  for (size_t i = 0; i < thread_num; i++) {
    threads[i] = std::make_shared<std::thread>([=, &msgs, &vertexs]() {
      size_t left, right;
      if (i < num_plus_one) {
        left = i * (one_thread_vertex + 1);
        right = left + one_thread_vertex + 1;
      } else {
        left = (i - num_plus_one) * one_thread_vertex +
               num_plus_one * (one_thread_vertex + 1);
        right = left + one_thread_vertex;
      }

      for (size_t j = left; j < right; j++) {
        for (const auto &edge : vertexs.at(j).GetEdge()) {
          auto msg = vertexs.at(j).Scatter(edge);
          msgs.at(i).insert(msgs.at(i).end(), msg.begin(), msg.end());
        }
      }
      left = right;
    });
    // send
  }

  for (auto &thread : threads) {
    thread->join();
  }
  return msgs;
}

} // namespace NGAS
