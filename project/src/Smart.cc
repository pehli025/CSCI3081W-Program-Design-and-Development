#include "Smart.h"
#include <vector>

namespace csci3081 {

// No constructor for strategies

std::vector<std::vector<float>> Smart::GetPath(const IGraph* graph, std::vector<float> src,
                                               std::vector<float> dest) {
    return graph->GetPath(src, dest);
}

}
