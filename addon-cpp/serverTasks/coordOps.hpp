#pragma once

#ifndef COORD_OPS_HPP
#define COORD_OPS_HPP

#include <iostream>
#include <iomanip>
#include <cmath>

namespace motus {
    struct Position {
        double lat;
        double lon;
    };
    class geoOp {
    public:
        struct Position moveBy(double lt1, double ln1, double bear,double angDist) {
            double lt = asin(sin(lt1)*cos(angDist)+cos(lt1)*sin(angDist)*cos(bear));
            double ln = ln1 + atan2(sin(bear)*sin(angDist)*cos(lt1), cos(angDist)-sin(lt1)*sin(lt));
            struct Position newPos;
            newPos.lat = lt; newPos.lon = ln;
            return newPos;
        }

        //TODO make a NAPI program that runs forever and shares the memory state with node
        // we can make a NAPI program separate from the main CPP program that will be called by NAPI program
        // that NAPI program will give us loaded JSON from node server and apply operations on them such as move, remove, send back to server.
    };
} // namespace motus

#endif // COORD_OPS_HPP
