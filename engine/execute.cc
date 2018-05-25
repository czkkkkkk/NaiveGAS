#include "engine/execute.h"
#include "data/vertex.h"

#include <vector>
#include <thread>

namespace NGAS {

bool IsActive() {
  // TODO
  return true;
}


template <typename VertexT, typename MsgT>
void MultiThreadScatter(const std::vector<VertexT>& vertexs) {
  
  std::vector<std::thread*> t;

  size_t threadNum = vertexs.size() / 10 + (vertexs.size() % 10 != 0);
  t.resize(threadNum);

  for(int i = 0; i < threadNum; i++) {

    t[i] = std::make_shared<std::thread>([i,threadNum,&vertexs]() {
          size_t l = i * threadNum, r = (i + 1) * threadNum;
          if(i == threadNum - 1) r = vertexs.size();
          
          std::vector<std::pair<IdType, MsgT>> messageVector;
          for(int j = l; j < r; j++) {
            std::vector<std::pair<IdType, MsgT>> v = vertexs[j].Scatter();
            messageVector.insert(messageVector.end(), v.begin(), v.end());
          }


        });  
  }
  
  for(auto& i : t) {
    i -> join();
    delete i;
  }
}

}  // namespace NGAS
