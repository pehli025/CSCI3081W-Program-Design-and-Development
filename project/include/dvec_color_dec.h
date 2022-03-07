/**
 * @file dvec_color_dec.h
 */
#ifndef DVEC_COLOR_DEC_H
#define DVEC_COLOR_DEC_H
#define RGB_MAX 255

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <EntityProject/facade/delivery_system.h>
#include "delivery_vehicle.h"
#include "dvec_decorator.h"
#include <vector>
#include <string>

namespace csci3081 {

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief DECORATOR
 *
 * This class decorates delivery vehicles a certain color based on the battery life.
 *
 */
class VehicleColorDecorator : public DVecDecorator {
    public:
        /**
        * Constructor: Sets up delivery vehicle decorator. Decorates updating functionality of
        * vechicle.
        * numUpdates initially set at 0.  
        */
        VehicleColorDecorator(DeliveryVehicle* dvec);

        /**
        * Moves the delivery vehicle forward in time. Adds battery color indicator functionality.
        * The only virtual method in dvec_decorator so extending classes can decorate update.
        *
        * @param[in] dt The amount of time to move the delivery vehicle by.
        */
        void update(float dt) override;

        /**
        * Sends a message to all observers of the delivery vehicle to update the color of the drone.
        *
        */
        void updateColor();

        /**
        * Returns a string representation of the RGB hex value of the battery in the form 0xFFFF00.
        * The closer the battery is to dying, the more red it becomes (first 2 characters in the hex value)
        * The more alive the battery is, the more green it is (third and fourth characters in the hex value)
        */
        std::string getBatteryColor();
};

}  // namespace csci3081


#endif  // dvec_color_dec.h
