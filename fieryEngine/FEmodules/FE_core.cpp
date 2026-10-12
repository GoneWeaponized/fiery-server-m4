#include "../lib/RTree.h"

#include "FE_core.hpp"
#include <cmath>
#include <iostream>
#include <unordered_map>

constexpr int MAX_NODES = 16;

RTree<structureItem*, double, 2, double, MAX_NODES> structuresTree;

int count = 0;

void FE::core::addMsl(warhead_type warhead, float payloadSize, float mslSpeed,
    motus::GenericGeoObject mslObj)
{
    missileItem* missile = new missileItem(warhead, payloadSize, mslSpeed, mslObj);
    missileMap.insert({ missile->missileObject.id, missile });
}

void FE::core::insertStructure(double a, double b, double c, double d, float hp,
    std::string id, std::string own)
{
    structureItem* newStr = new structureItem;
    newStr->min[0] = a;
    newStr->max[0] = b;
    newStr->min[1] = c;
    newStr->max[1] = d;
    newStr->HP = hp;
    newStr->owner = own;
    newStr->subId = id;
    structuresTree.Insert(newStr->min, newStr->max, newStr);
    mapStructures.insert({ id, newStr });
    std::cout << "Entry made. SubId:" << newStr->subId << std::endl;
}
bool FE::core::searchAndGo(structureItem* val) { return true; }
bool FE::core::searchAndStop(structureItem* val) { return false; }
bool FE::core::searchAndGetWithinDistance(structureItem* val,
    motus::Position pos, float distance)
{
    if (motus::GeoOp::distance(pos.lat, pos.lon, val->min[0], val->min[1]) < distance) {
        return true;
    }
    return false;
}
bool FE::core::isWithinBB(motus::Position pos)
{
    structureItem* temp = new structureItem;
    temp->min[0] = pos.lat;
    temp->max[0] = pos.lat;
    temp->min[1] = pos.lon;
    temp->max[1] = pos.lon;
    if (structuresTree.Search(temp->min, temp->max, FE::core::searchAndStop) > 0) {
        delete temp;
        return true;
    }
    return false;
}
bool FE::core::isWithinBB(motus::Position pos, float distance)
{
    structureItem* temp = new structureItem;
    temp->min[0] = pos.lat;
    temp->max[0] = pos.lat;
    temp->min[1] = pos.lon;
    temp->max[1] = pos.lon;
    bool result;
    // the lambda as parameter is added to check if there's an object within a
    // given distance [] catches distance and pos () provides requested
    // structureItem* val and the body returns a boolean.
    structuresTree.Search(
        temp->min, temp->max, [&result, &temp, distance, pos](structureItem* val) {
            if (motus::GeoOp::distance(pos.lat, pos.lon, val->min[0], val->min[1]) < distance) {
                result = false;
                return false;
            }
            return true;
        });
    delete temp;
    return result;
}

float FE::core::dmgVal(warhead_type wh, float distance, float payloadSize)
{
    switch ((int)wh) {
    case HIGH_EXPLOSIVE:
        return ((payloadSize * 2) / distance) * 25;
    case HEAT:
        return ((payloadSize * 0.8) / distance) * 15;
    default:
        return 1.0;
    }
}

void FE::core::damageEvent(
    motus::Position pos,
    float blastRadiusMeters,
    missileItem& msl)
{
    if (blastRadiusMeters <= 0.0f) {
        return;
    }

    constexpr double EARTH_RADIUS_METERS = 6378137.0;

    double latRadius = (blastRadiusMeters / EARTH_RADIUS_METERS)
        * (180.0 / M_PI);

    double cosLat = std::cos(pos.lat * M_PI / 180.0);
    if (std::abs(cosLat) < 1e-10) {
        std::cerr << "Blast longitude bounds undefined near pole\n";
        return;
    }

    double lonRadius = (blastRadiusMeters / (EARTH_RADIUS_METERS * cosLat))
        * (180.0 / M_PI);

    double min[2] = {
        pos.lat - latRadius,
        pos.lon - lonRadius
    };

    double max[2] = {
        pos.lat + latRadius,
        pos.lon + lonRadius
    };

    std::cout << "\nExplosion at: "
              << pos.lat << ", " << pos.lon
              << "\nBlast radius: " << blastRadiusMeters << " m\n";

    int candidates = structuresTree.Search(
        min,
        max,
        [&pos, blastRadiusMeters, &msl](structureItem* val) {
            double structureLat = (val->min[0] + val->max[0]) / 2.0;

            double structureLon = (val->min[1] + val->max[1]) / 2.0;
            double d = motus::GeoOp::distance(
                pos.lat,
                pos.lon,
                structureLat,
                structureLon);

            std::cout << "Checking " << val->subId
                      << ": distance = " << d << " m\n";
            if (d > blastRadiusMeters) {
                return true;
            }
            float damage = FE::core::dmgVal(
                msl.wh,
                static_cast<float>(d),
                msl.payload);

            val->HP = std::max(0.0f, val->HP - damage);

            ++count;

            std::cout << "STRUCTURE HIT: " << val->subId
                      << "\nDamage: " << damage
                      << "\nRemaining HP: " << val->HP
                      << '\n';

            return true;
        });

    std::cout << "R-tree candidates found: " << candidates << '\n';
}

// int main()
// {
//
//     FE::core::insertStructure(
//         26.2190, 26.2205,
//         78.2220, 78.2240,
//         3000.0,
//         "struct_001",
//         "player_owner"
//     );

//     motus::Position nearPos = { 26.2195, 78.2230 };
//     float searchRadiusMeters = 100.0; // 500 meters
//
//     bool isInsideA = FE::core::isWithinBB(nearPos, searchRadiusMeters);
//     std::cout << "Test A (Near position, 500m radius): "
//               << (isInsideA ? "PASS (Found)" : "FAIL (Not found)") << std::endl;
//
//     motus::Position farPos = { 28.6139, 77.2090 }; // New Delhi coordinates
//
//     bool isInsideB = FE::core::isWithinBB(farPos, searchRadiusMeters);
//     std::cout << "Test B (Far position, 500m radius): "
//               << (!isInsideB ? "PASS (Correctly outside)" : "FAIL (False positive)") << std::endl;
//
//     return 0;
// }
