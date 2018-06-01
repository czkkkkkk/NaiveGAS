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
    });
  }

  for (auto &thread : threads) {
    thread->join();
  }
  return msgs;
}

template <typename VertexT>
auto MultiThreadCombineScatter(const std::vector<VertexT> &vertexs) {
  using MsgT = typename VertexT::MsgT;

  std::vector<std::shared_ptr<std::thread>> threads;
  const size_t thread_num = 50;
  threads.resize(thread_num);
  size_t one_thread_vertex = vertexs.size() / thread_num;
  size_t num_plus_one = vertexs.size() % thread_num;

  std::vector<std::pair<IdType, MsgT>> msgs_combine;
  std::vector<std::vector<std::pair<IdType, MsgT>>> msgs(thread_num);

  for (size_t i = 0; i < thread_num; i++) {
    threads.at(i) = std::make_shared<std::thread>([=, &msgs, &vertexs]() {
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
    });
  }
  for (auto &thread : threads) {
    thread->join();
  }

  for (size_t i = 0; i < thread_num; i++) {
    msgs_combine.insert(msgs_combine.end(), msgs.at(i).begin(),
                        msgs.at(i).end());
    msgs.at(i).clear();
  }

  std::vector<std::pair<IdType, MsgT>> msgs_sample(thread_num);
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  auto generator = std::bind(
      std::uniform_int_distribution<size_t>(0, msgs_combine.size() - 1),
      std::mt19937(seed));

  for (int i = 0; i < thread_num - 1; i++) {
    msgs_sample.at(i) = msgs_combine.at(generator());
  }

  std::sort(msgs_sample.begin(), msgs_sample.end(),
            [&](const auto &x, const auto &y) { return x.first < y.first; });
  for (auto &msg : msgs_combine) {
    size_t thread_id =
        std::lower_bound(
            msgs_sample.begin(), msgs_sample.end(), msg,
            [&](const auto &x, const auto &y) { return x.first < y.first; }) -
        msgs_sample.begin();
    msgs.at(thread_id).push_back(msg);
  }

  for (int i = 0; i < thread_num; i++) {
    threads.at(i) = std::make_shared<std::thread>([=, &msgs]() {
      std::sort(
          msgs.at(i).begin(), msgs.at(i).end(),
          [&](const auto &x, const auto &y) { return x.first < y.first; });
    });
  }

  for (auto &thread : threads) {
    thread->join();
  }

  msgs_combine.clear();
  for (int i = 0; i < thread_num; i++) {
    msgs_combine.insert(msgs_combine.end(), msgs.at(i).begin(),
                        msgs.at(i).end());
  }
  return msgs_combine;
}

} // namespace NGAS
