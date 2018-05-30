#include "data/vertex.h"

#include "gtest/gtest.h"

namespace NGAS {
namespace {

class TestVertex : public testing::Test {
 public:
  TestVertex() {}
  ~TestVertex() {}

 protected:
  void SetUp() {}
  void TearDown() {}
};

class TVertex : public Vertex<double, Edge> {
 public:
  using EdgeT = Edge;

  TVertex(IdType id) : Vertex(id) {}

  void Apply(const double& msg) override {
  }
  std::vector<std::pair<IdType, double>> Scatter(const EdgeT& edge) const override {
    return {};
  }
};

TEST_F(TestVertex, InitAndDelete) {
  TVertex* ptr = new TVertex(10);
  delete ptr;
}

} // namespace
} // namespace NGAS
