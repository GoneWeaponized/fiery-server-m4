#include "../lib/RTree.h"

#include "FE_core.hpp"
#include <iostream>
#include <cmath>

constexpr int MAX_NODES = 16;

RTree<structureItem*, double, 2, double, MAX_NODES> structuresTree;
int count = 0;
        void FE::core::insertStructure(double a, double b, double c, double d, float hp, std::string id, std::string own) {
            structureItem* newStr = new structureItem;
            newStr->min[0] = a;
            newStr->max[0] = b;
            newStr->min[1] = c;
            newStr->max[1] = d;
            newStr->HP = hp;
            newStr->owner = own;
            newStr->subId = id;
            structuresTree.Insert(newStr->min, newStr->max, newStr);
            std::cout<<"Entry made. SubId:"<<newStr->subId<<std::endl;
        }
        bool FE::core::searchAndGo(structureItem* val) {
            return true;
        }
        bool FE::core::searchAndStop(structureItem* val) {
            return false;
        }
        bool FE::core::searchAndGetWithinDistance(structureItem* val, motus::Position pos, float distance) {
            if (
                motus::GeoOp::distance(pos.lat, pos.lon, val->min[0], val->min[1])<distance
            ) {
                return true;
            }
            return false;
        }
        bool FE::core::isWithinBB(motus::Position pos) {
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
        bool FE::core::isWithinBB(motus::Position pos, float distance) {
            structureItem* temp = new structureItem;
            temp->min[0] = pos.lat;
            temp->max[0] = pos.lat;
            temp->min[1] = pos.lon;
            temp->max[1] = pos.lon;
            // the lambda as parameter is added to check if there's an object within a given distance [] catches distance and pos () provides requested structureItem* val and the body returns a boolean.
            structuresTree.Search(temp->min, temp->max, [&temp, distance, pos](structureItem* val) {
                if (
                    motus::GeoOp::distance(pos.lat, pos.lon, val->min[0], val->min[1])<distance
                ) {
                    return false;
                }
                return false;
            });
            delete temp;
            return false;
        }
        void FE::core::damageEvent(
            motus::Position pos,
            float distance,
            float yield
        ) {
            double latRadius = distance / 111320.0;

            double lonRadius =
            distance /
            (111320.0 * std::cos(pos.lat * M_PI / 180.0));

            double min[2] = {
                pos.lat - latRadius,
                pos.lon - lonRadius
            };

            double max[2] = {
                pos.lat + latRadius,
                pos.lon + lonRadius
            };

            structuresTree.Search(
                min,
                max,
                [pos, distance, yield](structureItem* val) {

                    double structureLat =
                    (val->min[0] + val->max[0]) / 2.0;

                    double structureLon =
                    (val->min[1] + val->max[1]) / 2.0;

                    double d = motus::GeoOp::distance(
                        pos.lat,
                        pos.lon,
                        structureLat,
                        structureLon
                    );

                    if (d <= distance) {
                        count++;
                        val->HP = val->HP - yield*distance;
                        std::cout
                        << "Structure hit at "
                        << structureLat << ", "
                        << structureLon <<" " << '\n';
                    }

                    return false;
                }
            );
        }
// int main() {
//     // main code for testing
//     FE::core::insertRect(0.0, 0.01, 0.0, 0.01);
//     FE::core::insertRect(0.02, 0.03, 0.02, 0.03);
//     FE::core::insertRect(0.05, 0.06, 0.05, 0.06);
//     FE::core::insertRect(22.7196, 22.7200, 75.8500, 75.8505); // S1
//     FE::core::insertRect(22.7250, 22.7255, 75.8500, 75.8505); // S2
//     FE::core::insertRect(22.7150, 22.7155, 75.8500, 75.8505); // S3
//
//     FE::core::insertRect(22.7200, 22.7205, 75.8600, 75.8605); // S4
//     FE::core::insertRect(22.7200, 22.7205, 75.8400, 75.8405); // S5
//
//     FE::core::insertRect(22.7300, 22.7305, 75.8700, 75.8705); // S6
//     FE::core::insertRect(22.7000, 22.7005, 75.8300, 75.8305); // S7
//
//     FE::core::insertRect(22.7500, 22.7505, 75.9000, 75.9005); // S8
//     FE::core::damageEvent(
//         {0.005, 0.005},
//         2000.0f,
//         100.0f
//     );
//     FE::core::damageEvent(
//         {22.7198, 75.8502},
//         100.0f,
//         100.0f
//     );
//     std::cout << "Test 2: " << count << '\n';
//     FE::core::damageEvent(
//         {22.7198, 75.8502},
//         1000.0f,
//         100.0f
//     );
//
//     std::cout << "North/South test: " << count << '\n';
//
//     std::cout << "Structures hit: " << count << '\n';
//     return 0;
// }
