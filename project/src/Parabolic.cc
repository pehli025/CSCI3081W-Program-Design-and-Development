#include "Parabolic.h"
#include <vector>
namespace csci3081 {

// No constructor for strategies

std::vector<std::vector<float>> Parabolic::GetPath(const IGraph* graph, std::vector<float> src, std::vector<float> dest) {
    std::vector<std::vector<float>> ret;
    float x0 = src.at(0);
    float y0 = src.at(1);
    float z0 = src.at(2);
    float x1 = dest.at(0);
    float y1 = dest.at(1);
    float z1 = dest.at(2);
    float j  = PARABOLIC_VERTEX;
    for (int t = 0; t <= PARABOLIC_STEP; t++) { // t ranges from 0 to 100, 0 = src, 100 = dest, 50 = midpoint
        float stepT = ((float) t) / PARABOLIC_STEP; // 0 <= stepT <= 1
        float lerpX = x0 + stepT * (x1 - x0); // Lerp = linearly interpolate x
        float lerpZ = z0 + stepT * (z1 - z0);
        float quadY = (-4*j + 2*y0 + 2*y1)*(stepT*stepT) + (4*j - 3*y0 - y1)*stepT + y0; // Quad = quadratically interpolate y with parametric eqn.
        // std::cout << "j, y0, y1, stepT: " << "(" << j << ", " << y0 << ", " << y1 << ", " << stepT << ")" << std::endl;
        // std::cout << "Y val: " << quadY << std::endl;
        std::vector<float> node{lerpX,quadY,lerpZ};
        ret.push_back(node);
    }
    return ret;
}

}