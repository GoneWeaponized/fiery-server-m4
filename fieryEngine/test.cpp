#include "GeoOp.hpp"
#include <unordered_map>
#include <chrono>
#include <thread>
#include <iostream>
#include <random>
#include <atomic>
#include <mutex>

using Geo = motus::GeoOp;
using Pos = motus::Position;
using GeoObj = motus::GenericGeoObject;

std::atomic<bool> simulationRunning{true};
std::mutex geoObjMutex;
Geo geo;
std::chrono::seconds tickRate = std::chrono::seconds(1);
std::unordered_map<std::string, GeoObj> activeGeoObj;

std::string generate_random_string(std::size_t length) {
    // 1. Define the character pool you want to pick from
    const std::string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    // 2. Initialize the random number engine
    std::random_device rd;                             // Obtains a random seed from hardware
    std::mt19937 generator(rd());                      // Standard mersenne_twister_engine
    std::uniform_int_distribution<> distribution(0, characters.size() - 1);

    // 3. Build the string
    std::string random_string;
    random_string.reserve(length); // Optimize memory allocation

    for (std::size_t i = 0; i < length; ++i) {
        random_string += characters[distribution(generator)];
    }

    return random_string;
}

void addGeoObject(Pos start, Pos dest, float speed) {
    GeoObj newGeoObj;
    newGeoObj.position = start;
    newGeoObj.startPosition = start;
    newGeoObj.targetPosition = dest;
    newGeoObj.speed = speed;
    newGeoObj.totalAngularDistance = geo.angularDistanceTo(start, dest);
    newGeoObj.bearing = geo.bearingTo(start, dest);
    newGeoObj.id = generate_random_string(16);

    {
        std::lock_guard<std::mutex> lock(geoObjMutex);
        activeGeoObj.insert({newGeoObj.id, newGeoObj});
    }

}
void updateDistance(GeoObj& geoObj) {
    if (geoObj.angularDistanceCovered >= geoObj.totalAngularDistance) {
        activeGeoObj.erase(geoObj.id);
    }
    else {
        geoObj.angularDistanceCovered += (geoObj.speed/3.6) /*converting from kmph to mps*/;
    }
}
void iterativeUpdates() {
  while (simulationRunning) {

        {
            std::lock_guard<std::mutex> lock(geoObjMutex);
            for (auto it = activeGeoObj.begin(); it != activeGeoObj.end();)
            {
                 GeoObj& object = it->second;
                 geo.objectMove(object);
            std::cout<<""<<geo.getPosition(object).lat<<", "<<geo.getPosition(object).lon<<std::endl;
            if (object.angularDistanceCovered >=object.totalAngularDistance)
            {
                     std::cout
                     << "\n"
                     << it->first
                     << " reached its destination\n";

                     it = activeGeoObj.erase(it);
            }
            else
            {
                     ++it;
            }
          }

        }

             std::this_thread::sleep_for(tickRate);
    }

    std::cout << "\nall jobs done. No missiles in the map.\n";

}
void addRandomGeoObjects(int count)
{
    std::random_device rd;
    std::mt19937 generator(rd());

    std::uniform_real_distribution<double> latDist(-90.0, 90.0);
    std::uniform_real_distribution<double> lonDist(-180.0, 180.0);

    for (int i = 0; i < count; ++i)
    {
        Pos start{
            latDist(generator),
            lonDist(generator)
        };

        Pos destination{
            latDist(generator),
            lonDist(generator)
        };

        addGeoObject(start, destination, 300000.0f);
    }
}

int main() {
    addRandomGeoObjects(8);
    iterativeUpdates();
}
