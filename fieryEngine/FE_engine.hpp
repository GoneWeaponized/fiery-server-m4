#pragma once

#ifndef FE_ENGINE_HPP
#define FE_ENGINE_HPP

#include <string>

namespace FE {
    namespace engine {
        void addMissile(std::string subId, double pos[], double dest[], float speed);
        void removeFromMap(std::string key);
        void moveMissile(std::string key);
        void startEngine();
    }
}

#endif
