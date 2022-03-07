#include "drone.h"
#include "json_helper.h"
#include "battery.h"
#include "vector_operators.h"
#include "vector3d.h"
#include "vector2d.h"
#include <vector>

namespace csci3081 {

Drone::Drone(const picojson::object& val) : DeliveryVehicle(val) {
    type = "drone";
}

void Drone::moveTo(std::vector<float> floatDest, float dt) {
    Vector3D dest = Vector3D::convertToVector3D(floatDest);
    Vector3D currLoc = Vector3D::convertToVector3D(this->position);
    Vector3D oldPos = Vector3D::convertToVector3D(this->position);
    Vector3D travelDir = dest - currLoc;
    // If our drone cannot make it to the destination in one unit of time
    if (this->speed * dt < travelDir.magnitude()) {
        travelDir.normalize();
        Vector3D realDisplacement = travelDir * dt * speed;
        currLoc = currLoc + realDisplacement;
    } else {
        currLoc.setX(dest.getX());
        currLoc.setY(dest.getY());
        currLoc.setZ(dest.getZ());
        destIndex += 1;
    }
    travelDir.normalize();
    this->position = currLoc.toStdVector();
    this->direction = travelDir.toStdVector();
}

void Drone::update(float dt) {
    // Ensure that no package assigned to this drone, start the next enqueued delivery
    if (this->package == nullptr) {
        dynamic = false;
        return;
    }
    // If battery dies, set up proper flags for next frame
    if (this->battery.getRemLife() <= 0) {
        rescheduleFlag = true;
        return;
    }

    // Have package assigned
    this->moveTo(this->path.at(this->destIndex),dt);
    // If not holding package check if we're in range to pick it up
    Vector3D droneLoc = Vector3D::convertToVector3D(this->GetPosition());
    if (!holdingPackage()) {
        Vector3D packageLoc = Vector3D::convertToVector3D(this->package->GetPosition());
        Vector3D difference = packageLoc - droneLoc;
        // If the distance between the package and drone is smaller than radius, pick it up
        if (difference.magnitude() < this->radius) {
            hasPackage = true;
            // Change package position to drone's position after picking up
            this->package->setDynamic(true);
            this->package->setPosition(this->position);
            this->package->notifyEnRoute();
            this->notifyToCustomer();
        }
    }
    // Drone is carrying package, update package position
    else {
        this->package->setPosition(this->position);
        this->package->setDirection(this->direction);
    }
    // We are in radius of customer and holding package
    Vector3D custLoc = this->package->getDestination();
    if ((custLoc - droneLoc).magnitude() < this->radius && holdingPackage()) {
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
