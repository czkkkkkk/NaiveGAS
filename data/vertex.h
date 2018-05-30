#pragma once

#include <vector>

#include "data/edge.h"
#include "data/macros.h"

namespace NGAS {

template <typename MsgT, typename EdgeT> class Vertex {
public:
  Vertex(IdType id) : id_(id) {}

  virtual void AddEdge(const EdgeT &edge) { adj_.push_back(edge); }

  virtual void Combine(MsgT &lhs, const MsgT &rhs) { lhs += rhs; }
  virtual void Apply(const MsgT &msg) = 0;
  virtual std::vector<std::pair<IdType, MsgT>>
  Scatter(const EdgeT &edge) const = 0;

  const std::vector<EdgeT> &GetEdge() const { return adj_; }

private:
  IdType id_;
  int offset_;
  std::vector<EdgeT> adj_;
};

} // namespace NGAS
