#include "robot.h"
#include "json_helper.h"
#include "battery.h"
#include "vector_operators.h"
#include "vector3d.h"
#include "vector2d.h"
#include <vector>
namespace csci3081 {

Robot::Robot(const picojson::object& val) : DeliveryVehicle(val) {
    type = "robot";
}

void Robot::moveTo(std::vector<float> floatDest, float dt) {
    Vector2D dest = Vector2D::convertToVector2D(floatDest);
    Vector2D currLoc = Vector2D::convertToVector2D(this->position);
    Vector2D travelDir = dest - currLoc;
    Vector3D finalLoc = Vector3D::convertToVector3D(this->position);
    // Our robot cannot make it to the destination
    if (this->speed * dt < travelDir.magnitude()) {
        travelDir.normalize();
        Vector2D realDisplacement = travelDir * dt * speed;
        finalLoc = finalLoc + realDisplacement;
    }
    else {
        finalLoc.setX(dest.getX());
        finalLoc.setY(this->position.at(1)); // Keep y coord constant
        finalLoc.setZ(dest.getZ());
        destIndex += 1;
    }
    travelDir.normalize();
    this->position = finalLoc.toStdVector();
    this->direction = travelDir.toStdVector();
}

void Robot::update(float dt) {
    // No package assigned to this robot, start the next enqueued delivery
    if (this->package == nullptr) {
        dynamic = false;
        return;
    }
    if (this->battery.getRemLife() <= 0) {  // Battery dies set up proper flags for next frame
        rescheduleFlag = true;
        return;
    }

    // Have package assigned
    this->moveTo(this->path.at(this->destIndex),dt);
    // If not holding package check if we're in range to pick it up.
    // For robots, packages may be too high, but we ignore y coordinate so we use Vector2D
    Vector2D robotLoc = Vector2D::convertToVector2D(this->GetPosition());
    if (!holdingPackage()) {
        Vector2D packageLoc = Vector2D::convertToVector2D(this->package->GetPosition());
        Vector2D difference = packageLoc - robotLoc;
        // If the distance between the package and robot is smaller than radius, pick it up
        if (difference.magnitude() < this->radius) {
            hasPackage = true;
            // Change package position to robot's position after picking up
            this->package->setPosition(this->position);
            this->package->setDynamic(true);
            this->package->notifyEnRoute();
            this->notifyToCustomer();
        }
    }
    // Robot is carrying package, update package position
    else {
        this->package->setPosition(this->position);
        this->package->setDirection(this->direction);
    }
    // We are in radius of customer
    Vector3D custLoc = this->package->getDestination();
    Vector2D custLoc2D(custLoc.getX(),0,custLoc.getZ());
    if ((custLoc2D - robotLoc).magnitude() < this->radius && holdingPackage()) {
        std::vector<float> teleport{0,-10000,0}; // Teleport out of scene
        this->package->setPosition(teleport);

        // Reset path for future deliveries
        this->package->notifyDelivered();
        this->package = nullptr;
        this->path.clear();
        destIndex = 0;
        this->isMoving = false;
        this->hasPackage = false;
        this->notifyIdle();
    }
    // Since we moved, we use battery based on time
    this->battery.useBattery(dt);
}

}
