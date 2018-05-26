#include "data/edge.h"
#include "data/vertex.h"
#include "engine/execute.h"

using namespace NGAS;

class PREdge : public Edge {};

class PRVertex : public Vertex<double, PREdge> {
public:
  using MsgT = double;
  using EdgeT = PREdge;

  void Combine(double &lhs, const double &rhs) override { lhs += rhs; }
  void Apply(const double &msg) override { pr_value_ = msg; }
  std::vector<std::pair<IdType, double>>
  Scatter(const EdgeT &edge) const override {
    return {};
  }

private:
  double pr_value_;
};

int main() {
  RunJob<PRVertex>();
  return 0;
}
