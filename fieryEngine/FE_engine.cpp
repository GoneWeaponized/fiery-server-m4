#include "FE_engine.hpp"
#include "./FEmodules/FE_core.hpp"
#include "GeoOp.hpp"
#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>
#include <unordered_map>

using GObj = motus::GenericGeoObject;
using Geo = motus::GeoOp;
using Position = motus::Position;

std::chrono::seconds tickRate = std::chrono::seconds(1);
// std::unordered_map<std::string, GObj*> movingMissile;
std::atomic<bool> simulationRunning{true};
std::mutex geoObjMutex;
Geo geo;

std::string generate_random_string(std::size_t length) {
  const std::string characters =
      "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  std::random_device rd;
  std::mt19937 generator(rd());
  std::uniform_int_distribution<> distribution(0, characters.size() - 1);
  std::string random_string;
  random_string.reserve(length); // Optimize memory allocation

  for (std::size_t i = 0; i < length; ++i) {
    random_string += characters[distribution(generator)];
  }

  return random_string;
}

void FE::engine::addMissile(std::string subId, double pos[], double dest[],
                            float speed) {
  GObj *newObj = new GObj;
  newObj->startPosition.lat = pos[0];
  newObj->startPosition.lon = pos[1];
  newObj->position = newObj->startPosition;
  newObj->targetPosition.lat = dest[0];
  newObj->targetPosition.lon = dest[1];
  newObj->totalAngularDistance =
      geo.angularDistanceTo(newObj->position, newObj->targetPosition);
  newObj->bearing =
      geo.bearingTo(newObj->startPosition, newObj->targetPosition);
  newObj->angularDistanceCovered = 0.0;
  newObj->speed = speed;
  {
    std::lock_guard<std::mutex> lock(geoObjMutex);
    std::cout << "LOCKED! FE::engine::addMissile\n";
    // movingMissile.insert({subId, newObj});
    FE::core::addMsl(HIGH_EXPLOSIVE, 500, newObj->speed, *newObj);
    // adds a missile in missileMap
    // the code might give a stroke to an AI model :skull:
  }
  std::cout << "ADDED MISSILE!\n";
}

void FE::engine::removeFromMap(std::string key) {
  {
    std::lock_guard<std::mutex> lock(geoObjMutex);
    // delete movingMissile.at(key); I used to store them missiles here but
    // chagned ti missileMap
    delete missileMap.at(key); // good boys clean the mess they made. But heck i
                               // make more mess when doing so
    missileMap.erase(key);
  }
}

void FE::engine::startEngine() {

  while (simulationRunning) {
    {
      std::lock_guard<std::mutex> lock(geoObjMutex);
      for (auto it = missileMap.begin(); it != missileMap.end();) {
        GObj& object = it->second->missileObject;
        geo.objectMove(object);
        std::cout << "" << geo.getPosition(object).lat << ", "
                  << geo.getPosition(object).lon << std::endl;
        if ((object).angularDistanceCovered >= (object).totalAngularDistance) {
          std::cout << "\n" << it->first << " reached its destination\n";
          FE::core::damageEvent(it->second->missileObject.position, 100.0, *it->second);
          delete missileMap.at(it->first);
          it = missileMap.erase(it);
        } else {
          ++it;
        }
      }
    }
    std::this_thread::sleep_for(tickRate);
  }
  std::cout << "\nall jobs done. No missiles in the map.\n";
}
int main() {
  float hp = 10000.0;
  // main code for testing
  FE::core::insertStructure(0.0, 0.01, 0.0, 0.01, hp,
                            generate_random_string(16),
                            generate_random_string(16));
  FE::core::insertStructure(0.02, 0.03, 0.02, 0.03, hp,
                            generate_random_string(16),
                            generate_random_string(16));
  FE::core::insertStructure(0.05, 0.06, 0.05, 0.06, hp,
                            generate_random_string(16),
                            generate_random_string(16));
  FE::core::insertStructure(22.7196, 22.7200, 75.8500, 75.8505, hp,
                            generate_random_string(16),
                            generate_random_string(16)); // S1
  FE::core::insertStructure(22.7250, 22.7255, 75.8500, 75.8505, hp,
                            generate_random_string(16),
                            generate_random_string(16)); // S2
  FE::core::insertStructure(22.7150, 22.7155, 75.8500, 75.8505, hp,
                            generate_random_string(16),
                            generate_random_string(16)); // S3

  FE::core::insertStructure(22.7200, 22.7205, 75.8600, 75.8605, hp,
                            generate_random_string(16),
                            generate_random_string(16)); // S4
  FE::core::insertStructure(22.7200, 22.7205, 75.8400, 75.8405, hp,
                            generate_random_string(16),
                            generate_random_string(16)); // S5

  FE::core::insertStructure(22.7300, 22.7305, 75.8700, 75.8705, hp,
                            generate_random_string(16),
                            generate_random_string(16)); // S6
  FE::core::insertStructure(22.7000, 22.7005, 75.8300, 75.8305, hp,
                            generate_random_string(16),
                            generate_random_string(16)); // S7

  FE::core::insertStructure(22.7500, 22.7505, 75.9000, 75.9005, hp,
                            generate_random_string(16),
                            generate_random_string(16)); // S8

  // FE::core::damageEvent({0.005, 0.005}, 2000.0f);
  // FE::core::damageEvent({22.7198, 75.8502}, 100.0f);
  // std::cout << "Test 2: " << '\n';
  // FE::core::damageEvent({22.7198, 75.8502}, 1000.0f);
  double p[2] = {22.900, 75.8700};
  double d[2] = {22.7505, 75.9005};
  FE::engine::addMissile(generate_random_string(16), p, d, 3000.0);
  FE::engine::startEngine();
  return 0;
}
