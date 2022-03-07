/**
 *@file parabolic.h
 */
#ifndef PARABOLIC_H
#define PARABOLIC_H
#define PARABOLIC_STEP 50
#define PARABOLIC_VERTEX 400

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <EntityProject/facade/delivery_system.h>
#include "entity_subject.h"
#include <strategy.h>

namespace csci3081 {

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief
 * The strategy interface for choosing routes for delivery vehicles.
 *
 * This class is contained within a delivery vehicle. A delivery vehicle uses an IStrategy to figure out its routing pattern.
 */
class Parabolic : public IStrategy {
    public:
        /*!
        * Gets the parabolic path from src to dest using integration. Ignores the graph parameter
        * passed in but other strategies use graph.
        *
        * @param[in] graph An IGraph* that is not used in generating the parabolic path but is used
        * in other strategies. Included so all strategies can be treated the same.
        * @param[in] src a std::vector<float> of size 3 that details the x, y, and z coordinates of
        * the starting location.
        * @param[in] dest a std::vector<float> of size 3 that detailst he x, y, and z coordinates of
        * the ending location.
        * @return A vector of of vectors which contain 3 floats. Each of these vectors detail the 3D
        * coordinates of the path for an entity to follow
        */
        std::vector<std::vector<float>> GetPath(const IGraph* graph, std::vector<float> src,
                                                std::vector<float> dest);

};

}  // namespace csci3081


#endif  // PARABOLIC_H
