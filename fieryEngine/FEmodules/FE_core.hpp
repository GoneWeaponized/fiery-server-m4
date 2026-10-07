#pragma once

#ifndef FE_CORE_HPP
#define FE_CORE_HPP

#include <string>
#include "../GeoOp.hpp"

struct structureItem {
    double min[2];
    double max[2];
    std::string subId = "NULL";
    std::string owner ="NULL";
    float HP = 100;
};
struct missileItem {
    int blastRad;
    int yeild;
};
namespace FE {
    namespace core {
        void insertStructure(double a, double b, double c, double d, float hp, std::string id, std::string own);
        bool searchAndGo(structureItem* val);
        bool searchAndStop(structureItem* val);
        bool searchAndGetWithinDistance(structureItem* val, motus::Position pos, float distance);
        bool isWithinBB(motus::Position pos);
        bool isWithinBB(motus::Position pos, float distance);
        void damageEvent(motus::Position pos, float distance, float yield);
    }
}

#endif
