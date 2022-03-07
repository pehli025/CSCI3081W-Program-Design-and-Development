#include "Beeline.h"
#include <vector>

namespace csci3081 {

// No constructor for strategies

std::vector<std::vector<float>> Beeline::GetPath(const IGraph* graph, std::vector<float> src,
                                                 std::vector<float> dest) {
    std::vector<std::vector<float>> ret;  // return variable
    // Calculate location directly above source and destination location
    std::vector<float> aboveSrc{src.at(0), BEELINE_MAX_HEIGHT, src.at(2)};
    std::vector<float> aboveDest{dest.at(0), BEELINE_MAX_HEIGHT, dest.at(2)};

    // Add initial positions, above locations, and destination to return route
    ret.push_back(src);
    ret.push_back(aboveSrc);
    ret.push_back(aboveDest);
    ret.push_back(dest);
    return ret;
}

}
