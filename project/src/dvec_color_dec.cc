#include "dvec_decorator.h"
#include "dvec_color_dec.h"
#include "json_helper.h"
#include <iomanip>

namespace csci3081 {

VehicleColorDecorator::VehicleColorDecorator(DeliveryVehicle* dvec) : DVecDecorator(dvec) {}

void VehicleColorDecorator::update(float dt) {
    float lifeBefore = 100.0 * getBattery().getRemLife() / getBattery().getMaxLife();
    decoratedVehicle->update(dt);
    // Update for the first 1000 frames or so. This is so that each delivery vehicle's color
    // sphere is rendered
    if (numUpdates <= 360*3 + 1) {
        // Update color every 360 frames so we don't flood the terminal
        if (numUpdates % 360 == 0 && getBattery().getRemLife() > 0) {
          updateColor();
        }
    }
    else {
        float lifeAfter = 100.0 * getBattery().getRemLife() / getBattery().getMaxLife();
        // Life before and life after are percentage of battery left ranging 0->100
        int batterySectionBefore = lifeBefore / 10;
        int batterySectionAfter = lifeAfter / 10;
        if (batterySectionAfter < batterySectionBefore) { // We've hit a 10% mark
            updateColor();
        }

    }
    numUpdates++;
}

void VehicleColorDecorator::updateColor() {
    picojson::object message = JsonHelper::CreateJsonObject();
    JsonHelper::AddStringToJsonObject(message, "type", "notify");
    JsonHelper::AddStringToJsonObject(message, "value", "updateDetails");
    // Since GetDetails() returns a const object, we have to copy it into a temporary json object
    // and add color there
    picojson::object tempDetails = GetDetails();
    JsonHelper::AddStringToJsonObject(tempDetails, "color", getBatteryColor());
    JsonHelper::AddObjectToJsonObject (message, "details", tempDetails);
    IEntity* castVehicle = static_cast<IEntity*>(decoratedVehicle);
    notify(message,*castVehicle);
}

std::string VehicleColorDecorator::getBatteryColor() {
    int greenColor = RGB_MAX * (getBattery().getRemLife() / getBattery().getMaxLife());
    int redColor = RGB_MAX - greenColor;
    // RGB values are represented as 3 hex numbers like 0x(FF red) (FF green) and (FF blue). We
    // shift the red and green values into their proper place
    std::stringstream redMask;
    std::stringstream greenMask;
    std::stringstream stream;
    redMask << std::setfill('0') << std::setw(2) << std::hex << redColor;
    greenMask << std::setfill('0') << std::setw(2) << std::hex << greenColor;
    stream << "0x" << redMask.str() << greenMask.str() << "00";
    return stream.str();
}

}
