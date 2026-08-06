#include <iostream>
#include <iomanip>
#include <cmath>
#include "coordOps.hpp"

int main() {
    constexpr double RAD_TO_DEG = 180.0 / M_PI;
    double dlt = 51.0 * M_PI / 180;
    double dln = 0.0 * M_PI / 180;
    double R =  6378137.0;
    double dangDist = 100.0/R;
    double b = 90 * M_PI / 180;

    motus::geoOp GeoOp;

    std::cout << std::fixed << std::setprecision(10);
    motus::Position vec = GeoOp.moveBy(dlt,dln,b,dangDist);
    std::cout<<vec.lat * RAD_TO_DEG<<" "<<vec.lon * RAD_TO_DEG;
}
