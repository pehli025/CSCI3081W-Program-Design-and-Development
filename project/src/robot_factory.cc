#include "robot_factory.h"
#include "json_helper.h"
#include "robot.h"
#include "dvec_color_dec.h"
#include <vector>
#include <iostream>

namespace csci3081 {

RobotFactory::RobotFactory() {}

IEntity* RobotFactory::CreateEntity(const picojson::object& detail) {
    if (JsonHelper::GetString(detail,"type").compare("robot") == 0) {
        return new VehicleColorDecorator(new Robot(detail));
    }
    return nullptr;
}

}
