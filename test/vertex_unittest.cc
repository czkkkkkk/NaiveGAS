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

class TVertex : public Vertex<double> {
 public:
  void Apply(const double& msg) {
  }
  std::vector<std::pair<IdType, double>> Scatter() {
    return {};
  }
};

TEST_F(TestVertex, InitAndDelete) {
  TVertex* ptr = new TVertex;
  delete ptr;
}

} // namespace
} // namespace NGAS
