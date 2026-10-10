#pragma once

#ifndef GEO_OP_HPP
#define GEO_OP_HPP
#define _USE_MATH_DEFINES

#include <algorithm>
#include <cmath>
#include <string>

constexpr double R = 6378137.0;
constexpr double RAD_TO_DEG = 180.0 / M_PI;
namespace motus {
struct Position {
  double lat;
  double lon;
};
struct GenericGeoObject {
  Position startPosition;
  Position targetPosition;
  Position position;
  double angularDistanceCovered = 0;
  double totalAngularDistance = 0;
  double bearing = 0;
  float speed = 0;
  std::string id;
};

class GeoOp {
public:
  static double distance(double lt1, double ln1, double lt2, double ln2) {

    double dLat = (lt1 - lt2) * M_PI / 180;
    double dLong = (ln1 - ln2) * M_PI / 180;

    lt1 = (lt1)*M_PI / 180;
    lt2 = (lt2)*M_PI / 180;
    double result =
        pow(sin(dLat / 2), 2) + pow(sin(dLong / 2), 2) * cos(lt1) * cos(lt2);
    result = std::min(1.0, result);
    double result2 = 2 * asin(sqrt(result));
    result = R * result2;
    return result;
  }

  Position destination(const Position &start, double bearing,
                       double angularDistance) {
    double lat1 = start.lat * M_PI / 180.0;
    double lon1 = start.lon * M_PI / 180.0;
    double brng = bearing * M_PI / 180.0;

    double lat2 =
        std::asin(std::sin(lat1) * std::cos(angularDistance) +
                  std::cos(lat1) * std::sin(angularDistance) * std::cos(brng));

    double lon2 =
        lon1 +
        std::atan2(std::sin(brng) * std::sin(angularDistance) * std::cos(lat1),

                   std::cos(angularDistance) - std::sin(lat1) * std::sin(lat2));

    lon2 = std::fmod(lon2 + 3.0 * M_PI, 2.0 * M_PI) - M_PI;

    return {lat2 * RAD_TO_DEG, lon2 * RAD_TO_DEG};
  }

  double bearingTo(const Position &from, const Position &to) {
    const double lat1 = from.lat * M_PI / 180.0;

    const double lat2 = to.lat * M_PI / 180.0;

    const double dLon = (to.lon - from.lon) * M_PI / 180.0;

    const double y = std::sin(dLon) * std::cos(lat2);

    const double x = std::cos(lat1) * std::sin(lat2) -
                     std::sin(lat1) * std::cos(lat2) * std::cos(dLon);

    double bearing = std::atan2(y, x) * RAD_TO_DEG;

    return std::fmod(bearing + 360.0, 360.0);
  }

  double angularDistanceTo(const Position &from, const Position &to) {
    const double lat1 = from.lat * M_PI / 180.0;

    const double lat2 = to.lat * M_PI / 180.0;

    const double dLat = (to.lat - from.lat) * M_PI / 180.0;

    const double dLon = (to.lon - from.lon) * M_PI / 180.0;

    const double sinLat = std::sin(dLat / 2.0);

    const double sinLon = std::sin(dLon / 2.0);

    const double a =
        sinLat * sinLat + std::cos(lat1) * std::cos(lat2) * sinLon * sinLon;

    return 2.0 * std::asin(std::sqrt(std::fmin(1.0, a)));
  }

  void objectMove(GenericGeoObject &object) {
    double angularStep = object.speed / R;

    object.angularDistanceCovered += angularStep;

    if (object.angularDistanceCovered >= object.totalAngularDistance) {
      object.angularDistanceCovered = object.totalAngularDistance;
    }
  }

  Position getPosition(const GenericGeoObject &object) {
    return destination(object.startPosition, object.bearing,
                       object.angularDistanceCovered);
  }
};
} // namespace motus

#endif // COORD_OPS_HPP
