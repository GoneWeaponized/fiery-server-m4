#include "haversine.h"
#include <cmath>
#include <iostream>

double haversine(double lt1, double ln1, double lt2, double ln2) {
    double dRad = 6371;

    double dLat = (lt1 - lt2) * M_PI/180;
    double dLong = (ln1 - ln2) * M_PI/180;

    lt1 = (lt1) * M_PI / 180;
    lt2 = (lt2) * M_PI / 180;
    double result = pow(sin(dLat/2), 2) + pow(sin(dLong/2),2) * cos(lt1) * cos(lt2);
    result = std::min(1.0, result);
    double result2 = 2 * asin(sqrt(result));
    result = dRad * result2;
    return result;
}
