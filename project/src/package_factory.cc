#include "package_factory.h"
#include "json_helper.h"
#include "package.h"
#include <vector>

namespace csci3081 {

PackageFactory::PackageFactory() {}

IEntity* PackageFactory::CreateEntity(const picojson::object& detail) {
    if (JsonHelper::GetString(detail,"type").compare("package") == 0) {
        return new Package(detail);
    }
    return nullptr;
}

}
