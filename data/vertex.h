#pragma once

#include <vector>

#include <data/macros.h>

namespace NGAS {

template <typename MsgT> class Vertex {
public:
  virtual void Combine(MsgT &lhs, const MsgT &rhs) = 0;
  virtual void Apply(const MsgT &msg) = 0;
  virtual std::vector<std::pair<IdType, MsgT>> Scatter() = 0;

private:
  IdType id_;
};

} // namespace NGAS
