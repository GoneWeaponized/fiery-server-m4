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

std::atomic<bool> simulationRunning{false};
std::mutex geoObjMutex;
Geo geo;
std::chrono::seconds tickRate = std::chrono::seconds(1);
std::unordered_map<int, GeoObj> activeGeoObj;
int numOMissiles = 0;

void addGeoObject(Pos start, Pos dest, float speed) {
    GeoObj newGeoObj;
    numOMissiles++;
    newGeoObj.position = start;
    newGeoObj.startPosition = start;
    newGeoObj.targetPosition = dest;
    newGeoObj.speed = speed;
    newGeoObj.totalAngularDistance = geo.angularDistanceTo(start, dest);
    newGeoObj.bearing = geo.bearingTo(start, dest);
    newGeoObj.id = numOMissiles;

    {
        std::lock_guard<std::mutex> lock(geoObjMutex);
        activeGeoObj.insert({numOMissiles, newGeoObj});
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

        addGeoObject(start, destination, 3000.0f);
    }
}

