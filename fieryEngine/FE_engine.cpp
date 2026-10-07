#include "FE_engine.hpp"
#include "./FEmodules/FE_core.hpp"
#include "GeoOp.hpp"
#include <unordered_map>
#include <atomic>
#include <mutex>
#include <thread>
#include <chrono>
#include <iostream>

std::chrono::seconds tickRate = std::chrono::seconds(1);
using GObj = motus::GenericGeoObject;
using Geo = motus::GeoOp;
using Position = motus::Position;
std::unordered_map<std::string, GObj*> movingMissile;
std::atomic<bool> simulationRunning{true};
std::mutex geoObjMutex;
Geo geo;


void FE::engine::addToMap(std::string subId, double pos[], double dest[], float speed) {
    GObj* newObj = new GObj;
    newObj->startPosition.lat = pos[0];
    newObj->startPosition.lon = pos[1];
    newObj->position = newObj->startPosition;
    newObj->targetPosition.lat = dest[0];
    newObj->targetPosition.lon = dest[1];
    newObj->totalAngularDistance = geo.angularDistanceTo(newObj->position, newObj->targetPosition);
    newObj->bearing = geo.bearingTo(newObj->startPosition, newObj->targetPosition);
    newObj->angularDistanceCovered = 0.0;
    newObj->speed = speed;
    {
        std::lock_guard<std::mutex> lock(geoObjMutex);
        movingMissile.insert({subId, newObj});
    }
}

void FE::engine::removeFromMap(std::string key) {
    {
        std::lock_guard<std::mutex> lock(geoObjMutex);
        delete movingMissile.at(key);
        movingMissile.erase(key);
    }
}

void FE::engine::startEngine() {

    while(simulationRunning) {
        {
            std::lock_guard<std::mutex> lock(geoObjMutex);
            for (auto it = movingMissile.begin(); it != movingMissile.end();)
            {
                GObj* object = it->second;
                geo.objectMove(*object);
                //std::cout<<""<<geo.getPosition(*object).lat<<", "<<geo.getPosition(*object).lon<<std::endl;
                if ((*object).angularDistanceCovered >=(*object).totalAngularDistance)
                {
                    std::cout
                    << "\n"
                    << it->first
                    << " reached its destination\n";
                    FE::core::damageEvent(it->second->position, 100.0, 30);
                    delete movingMissile.at(it->first);
                    it = movingMissile.erase(it);
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
