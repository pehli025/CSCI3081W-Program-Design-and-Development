#include "composite_factory.h"
#include "drone_factory.h"
#include "customer_factory.h"
#include "package_factory.h"
#include "robot_factory.h"
#include "json_helper.h"
#include <vector>
#include <iostream>

namespace csci3081 {

CompositeFactory::CompositeFactory() {}

void CompositeFactory::AddFactory(IEntityFactory* newFact) { factories.push_back(newFact); }

IEntity* CompositeFactory::CreateEntity(const picojson::object& detail) {
    // Make sure entity being created is valid
    if (!JsonHelper::ContainsKey(detail,"type")) {
        std::cout << "Composite factory cannot find type" << std::endl;
        return nullptr;
    }
    IEntity* newEnt = nullptr;
    // Iterate through all factories in composite
    for (IEntityFactory* fact : factories) {
        newEnt = fact->CreateEntity(detail);
        // If we find a factory to create a valid entity, return it
        if (newEnt) { return newEnt; }
    }
    return nullptr;
}

CompositeFactory::~CompositeFactory() {
    for (IEntityFactory* fact : factories) {
        delete fact;
    }
}

}
