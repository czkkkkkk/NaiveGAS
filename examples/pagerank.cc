#include "data/vertex.h"
#include "engine/execute.h"

using namespace NGAS;

class PRVertex : public Vertex<double> {
public:
  using MsgT = double;
  void Combine(double &lhs, const double &rhs) override { lhs += rhs; }
  void Apply(const double &msg) override { pr_value_ = msg; }
  std::vector<std::pair<IdType, double>> Scatter() override { return {}; }

private:
  double pr_value_;
};

int main() {
  RunJob<PRVertex>();
  return 0;
}
