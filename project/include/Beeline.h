/**
 *@file beeline.h
 */
#ifndef BEELINE_H
#define BEELINE_H
#define BEELINE_MAX_HEIGHT 400

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
 * This class is contained within a delivery vehicle. A delivery vehicle uses an IStrategy to
 * figure out its routing pattern.
 */
class Beeline : public IStrategy {
    public:
        /*!
        * Gets the beeline path from src to dest. Does not use graph, but graph is used in other
        * strategies such as the Smart strategy. A beeline route moves vertically at first to avoid
        * any collisions with buildings, then moves in a straight line towards the destination
        * ignoring the verticality, then drops down to the destination.
        *
        * @param graph An IGraph* that is not used in generating the beeline path but it is used in
        * other path strategies. Included so all strategies can be treated the same.
        * @param src a std::vector<float> of size 3 that details the x, y, and z coordinates of the
        * starting location.
        * @param dest a std::vector<float> of size 3 that detailst he x, y, and z coordinates of the
        * ending location.
        * @return A vector of of vectors which contain 3 floats. Each of these vectors detail the
        * 3D coordinates of the path for an entity to follow
        */
        std::vector<std::vector<float>> GetPath(const IGraph* graph, std::vector<float> src,
                                                std::vector<float> dest);

};

}  // namespace csci3081


#endif  // BEELINE_H
