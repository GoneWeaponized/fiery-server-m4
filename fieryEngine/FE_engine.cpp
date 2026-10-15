#include "FE_engine.hpp"
#include "FEmodules/FE_core.hpp"
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
std::atomic<bool> simulationRunning { true };
std::mutex geoObjMutex;
Geo geo;

std::string generate_random_string(std::size_t length)
{
    const std::string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(0, characters.size() - 1);
    std::string random_string;
    random_string.reserve(length);

    for (std::size_t i = 0; i < length; ++i) {
        random_string += characters[distribution(generator)];
    }

    return random_string;
}

void FE::engine::addMissile(std::string subId, double pos[], double dest[],
    float speed, short payload, short warhead)
{
    GObj* newObj = new GObj;
    newObj->startPosition.lat = pos[0];
    newObj->startPosition.lon = pos[1];
    newObj->position = newObj->startPosition;
    newObj->targetPosition.lat = dest[0];
    newObj->targetPosition.lon = dest[1];
    newObj->id = subId;
    newObj->totalAngularDistance = geo.angularDistanceTo(newObj->position, newObj->targetPosition);
    newObj->bearing = geo.bearingTo(newObj->startPosition, newObj->targetPosition);
    newObj->angularDistanceCovered = 0.0;
    newObj->speed = speed;
    {
        std::lock_guard<std::mutex> lock(geoObjMutex);

        FE::core::addMsl(
            static_cast<warhead_type>(warhead), 500, newObj->speed, *newObj);
    }
}

void FE::engine::removeFromMap(std::string key)
{
    {
        std::lock_guard<std::mutex> lock(geoObjMutex);
        // delete movingMissile.at(key); I used to store them missiles here but
        // chagned ti missileMap
        delete missileMap.at(key); // good boys clean the mess they made. But heck i
                                   // make more mess when doing so
        missileMap.erase(key);
    }
}

void FE::engine::stopEngine() { simulationRunning = false; }

void FE::engine::startEngine()
{

    while (simulationRunning) {
        {
            std::lock_guard<std::mutex> lock(geoObjMutex);
            for (auto it = missileMap.begin(); it != missileMap.end();) {
                GObj& object = it->second->missileObject;
                geo.objectMove(object);
                std::cout << "" << geo.getPosition(object).lat << ", "
                          << geo.getPosition(object).lon << std::endl;
                if ((object).angularDistanceCovered >= (object).totalAngularDistance) {
                    std::cout << "\n"
                              << it->first << " reached its destination\n";
                    FE::core::damageEvent(it->second->missileObject.targetPosition, 100.0, *it->second);
                    delete missileMap.at(it->first);
                    it = missileMap.erase(it);
                } else {
                    ++it;
                }
            }
        }
        std::this_thread::sleep_for(tickRate);
    }
    std::cout << "\n ==== ENGINE STOPPING ==== \n";
}
