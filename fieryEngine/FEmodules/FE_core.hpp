#pragma once

#ifndef FE_CORE_HPP
#define FE_CORE_HPP

#include "../GeoOp.hpp"
#include <string>
#include <unordered_map>

struct structureItem {
  double min[2];
  double max[2];
  std::string subId = "NULL";
  std::string owner = "NULL";
  float HP = 100;
};

enum warhead_type {
  HIGH_EXPLOSIVE,
  HEAT,
  THERMOBARIC,
  KINETIC,
  INCENDIARY,
  ELECTROMAGNETIC_PULSE,
  NUKE,
  FART
};

struct missileItem {
  missileItem(warhead_type wh, float payload, float speed,
              motus::GenericGeoObject missileObject)
      : wh(wh), payload(payload), speed(speed),
        missileObject(std::move(missileObject)) {}
  warhead_type wh;
  float payload;
  float speed;
  motus::GenericGeoObject missileObject;
};

inline std::unordered_map<std::string, structureItem *> mapStructures;
inline std::unordered_map<std::string, missileItem *> missileMap;

namespace FE {
namespace core {
float dmgVal(warhead_type wh, float distance, float payloadSize);
void addMsl(warhead_type warhead, float payloadSize, float mslSpeed,
            motus::GenericGeoObject mslObj);
void insertStructure(double a, double b, double c, double d, float hp,
                     std::string id, std::string own);
bool searchAndGo(structureItem *val);
bool searchAndStop(structureItem *val);
bool searchAndGetWithinDistance(structureItem *val, motus::Position pos,
                                float distance);
bool isWithinBB(motus::Position pos);
bool isWithinBB(motus::Position pos, float distance);
void damageEvent(
    motus::Position pos, float distance, missileItem& msl); // change this to use the missileItem in determining the dmg
} // namespace core
} // namespace FE

#endif
