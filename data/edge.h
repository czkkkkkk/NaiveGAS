#pragma once

#include "data/macros.h"

namespace NGAS {

class Edge {
public:
  Edge(int src, int dst) : src_(src), dst_(dst) {}
  IdType GetSrc() const { return src_; }
  IdType GetDst() const { return dst_; }

private:
  IdType src_, dst_;
};

} // namespace NGAS
