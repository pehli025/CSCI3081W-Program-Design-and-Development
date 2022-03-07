#include "customer_factory.h"
#include "json_helper.h"
#include "customer.h"
#include <vector>

namespace csci3081 {

CustomerFactory::CustomerFactory() {}

IEntity* CustomerFactory::CreateEntity(const picojson::object& detail) {
    if (JsonHelper::GetString(detail,"type").compare("customer") == 0) {
        return new Customer(detail);
    }
    return nullptr;
}

}
