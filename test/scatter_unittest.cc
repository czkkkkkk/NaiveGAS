#include "data/vertex.h"
#include "engine/scatter.h"

#include "gtest/gtest.h"

namespace NGAS {
namespace {

class TestScatter : public testing::Test {
 public:
  TestScatter() {}
  ~TestScatter() {}

 protected:
  void SetUp() {}
  void TearDown() {}
};

class TVertex : public Vertex<double, Edge> {
 public:
  using MsgT = double;
  using EdgeT = Edge;

  TVertex(IdType id) : Vertex(id) {}

  void Apply(const double& msg) override {
  }
  std::vector<std::pair<IdType, double>> Scatter(const EdgeT& edge) const override {
    return {{edge.GetDst(), 1}};
  }
};

TEST_F(TestScatter, MultiThreadScatter) {
  std::vector<TVertex> vertexs;
  for(int i = 0; i < 10; ++i) {
    vertexs.push_back(TVertex(i));
  }
  for(int i = 0; i < 10; ++i) {
    for(int j = 0; j < 10; ++j) {
      vertexs[i].AddEdge(Edge(i, j));
    }
  }

  auto msgs = MultiThreadScatter(vertexs);
  EXPECT_EQ(50, msgs.size());

  for(int i = 0; i < 10; ++i) {
    EXPECT_EQ(10, msgs.at(i).size());
    for(int j = 0; j < 10; ++j) {
      EXPECT_EQ(j, msgs.at(i).at(j).first);
      EXPECT_EQ(1, msgs.at(i).at(j).second);
    }
  }
}


TEST_F(TestScatter, MultiThreadCombineScatter) {
  std::vector<TVertex> vertexs;
  for(int i = 0; i < 10; i++) {
    vertexs.push_back(TVertex(i));
  }
  for(int i = 9; i >= 0; i--) {
    for(int j = 9; j >= 0; j--) {
      vertexs.at(i).AddEdge(Edge(i, j));
    }
  }
  
  auto msg_combine = MultiThreadCombineScatter(vertexs);
  EXPECT_EQ(100, msg_combine.size());
  
  for(int i = 0; i < 10; i++) {
    for(int j = 0; j < 10; j++) {
      EXPECT_EQ(i, msg_combine.at(i * 10 + j).first);
      EXPECT_EQ(1, msg_combine.at(i * 10 + j).second);
    }
  }
}

} // namespace
} // namespace NGAS
