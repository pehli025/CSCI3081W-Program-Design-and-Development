#include "drone_factory.h"
#include "json_helper.h"
#include "drone.h"
#include "dvec_color_dec.h"
#include <vector>
#include <iostream>

namespace csci3081 {

DroneFactory::DroneFactory() {}

IEntity* DroneFactory::CreateEntity(const picojson::object& detail) {
    if (JsonHelper::GetString(detail,"type").compare("drone") == 0) {
        return new VehicleColorDecorator(new Drone(detail));
    }
    return nullptr;
}

}