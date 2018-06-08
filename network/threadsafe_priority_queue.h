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
    global_queue_.push(elem_ptr);
    mu_.unlock();
    cond_.notify_one();
  }

  T WaitAndPop() {
    std::unique_lock<std::mutex> lk(mu_);
    cond_.wait(lk, [this] { return !global_queue_.empty(); });
    
    T elem = global_queue_.front();
    global_queue_.pop();
    return elem;
  }

 private:
  
  std::mutex mu_;
  std::queue<T> global_queue_;
  std::condition_variable cond_;
};

}  // namespace NGAS
