#pragma once

#include <chrono>
#include <condition_variable>
#include <set>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>


namespace NGAS {

// Store the shared pointer of mailbox event
// Handle smaller priority event first
// Events with same priority should be FIFO
// By now, priority = -1 are system mailbox events, other priority >= 0 are job events
template <typename T>
class ThreadsafePriorityQueue {
 public:
  ThreadsafePriorityQueue() = default;
  ~ThreadsafePriorityQueue() = default;
  ThreadsafePriorityQueue(const ThreadsafePriorityQueue&) = delete;
  ThreadsafePriorityQueue& operator=(const ThreadsafePriorityQueue&) = delete;
  ThreadsafePriorityQueue(ThreadsafePriorityQueue&&) = delete;
  ThreadsafePriorityQueue& operator=(ThreadsafePriorityQueue&&) = delete;

  void Push(T elem_ptr) {
    mu_.lock();
    int priority = elem_ptr->GetPriority();
    queue_[priority].push(elem_ptr);
    if (queue_[priority].size() == 1) {
      global_queue_.push(priority);
    }
    mu_.unlock();
    cond_.notify_one();
  }

  T WaitAndPop() {
    std::unique_lock<std::mutex> lk(mu_);
    cond_.wait(lk, [this] { return !global_queue_.empty(); });
    int priority = global_queue_.top();
    global_queue_.pop();
    T elem = queue_.at(priority).front();
    queue_.at(priority).pop();
    if(not queue_.at(priority).empty()) {
      global_queue_.push(priority);
    }
    return elem;
  }

 private:
  
  std::mutex mu_;
  std::priority_queue<int, std::vector<int>, std::greater<int>> global_queue_;
  std::unordered_map<int, std::queue<T>> queue_;
  std::condition_variable cond_;
};

}  // namespace NGAS
