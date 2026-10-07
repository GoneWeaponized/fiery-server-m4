#pragma once

#ifndef COORD_OPS_HPP
#define COORD_OPS_HPP


#include <cmath>

const double R =  6378137.0;
constexpr double RAD_TO_DEG = 180.0 / M_PI;
namespace motus {
    struct Position {
        double lat;
        double lon;
    };
    class geoOp {
    public:
        struct Position moveBy(double lt1, double ln1, double bear,double angDist) {
            lt1 = lt1 * M_PI / 180;
            ln1 = ln1 *  M_PI / 180;
            bear = bear * M_PI / 180;
            angDist = angDist / R;
            double lt = asin(sin(lt1)*cos(angDist)+cos(lt1)*sin(angDist)*cos(bear));
            double ln = ln1 + atan2(sin(bear)*sin(angDist)*cos(lt1), cos(angDist)-sin(lt1)*sin(lt));
            struct Position newPos;
            newPos.lat = lt * RAD_TO_DEG; newPos.lon = ln * RAD_TO_DEG;
            newPos.lon = std::fmod(newPos.lon + 540.0, 360.0) - 180.0;
            return newPos;
        }
        double initialBearing(double lat1, double lon1, double lat2, double lon2) {
            double phi1 = lat1 * M_PI / 180.0;
            double phi2 = lat2 * M_PI / 180.0;
            double deltaLambda = (lon2 - lon1) * M_PI / 180.0;

            double y = std::sin(deltaLambda) * std::cos(phi2);
            double x = std::cos(phi1) * std::sin(phi2) -
            std::sin(phi1) * std::cos(phi2) * std::cos(deltaLambda);

            double theta = std::atan2(y, x);
            double bearingDeg = theta * (180.0 / M_PI);
            return std::fmod(bearingDeg + 360.0, 360.0);
        }

        // struct Position calcBearingInit() {
        //     // calculate initial bearing
        // }
        // struct Position calcBearingIntermediate() {
        //     // calculate intermeditae bearing
        // }
        //TODO make a NAPI program that runs forever and shares the memory state with node
        // we can make a NAPI program separate from the main CPP program that will be called by NAPI program
        // that NAPI program will give us loaded JSON from node server and apply operations on them such as move, remove, send back to server.
    };
} // namespace motus

#endif // COORD_OPS_HPP
