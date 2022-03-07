/**
 *@file IStrategy.h
 */
#ifndef STRATEGY_H
#define STRATEGY_H

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <EntityProject/facade/delivery_system.h>
#include "entity_subject.h"

namespace csci3081 {

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief The strategy interface for choosing routes for delivery vehicles.
 *
 * This class is contained within a delivery vehicle. A delivery vehicle uses an IStrategy to figure
 * out its routing pattern.
 */
class IStrategy {
    public:

        /**
        * Define a virtual destructor for proper memory management. Good practice to have.
        */
        virtual ~IStrategy() {}

        /*!
        * Virtual strategy that will be implemented by inheriting classes such as Smart, Beeline,
        * and Parabolic.
        * Takes in an IGraph* in cases that the strategy is smart, ignores the IGraph* if it's smart or parabolic.
        *
        * @param[in] graph An IGraph* so any strategy that uses a graph of the simulation can use it.
        * @param[in] src a std::vector<float> of size 3 that details the x, y, and z coordinates
        * of the starting location.
        * @param[in] dest a std::vector<float> of size 3 that detailst he x, y, and z coordinates of
        * the ending location.
        * @return A vector of of vectors which contain 3 floats. Each of these vectors detail the
        * 3D coordinates of the path.
        */
        virtual std::vector<std::vector<float>> GetPath(const IGraph* graph,
                            std::vector<float> src, std::vector<float> dest) = 0;

};

}  // namespace csci3081


#endif  //STRATEGY_H
