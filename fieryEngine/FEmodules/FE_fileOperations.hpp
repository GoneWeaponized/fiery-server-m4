#pragma once

#ifndef FE_FILEOP_HPP
#define FE_FILEOP_HPP

#include "../lib/json.hpp"
#include "../GeoOp.hpp"
#include "FE_core.hpp"
#include <unordered_map>
#include <fstream>
#include <string>
#include <stdexcept>

using json = nlohmann::json;
struct structure {
    motus::Position sPos;
    float HP;
    std::string SubId;
    std::string Owner;
    int Type;
    structure(motus::Position pos, float hp, std::string subId, std::string owner, int type) {
        sPos = pos;
        HP = hp;
        SubId = subId;
        Owner = owner;
        Type = type;
    }
};
namespace FE {
    class structures {
        public:
            static json structuresJson;
            static std::unordered_map<motus::Position*, std::string> string_pos_map;
            static std::unordered_map<std::string, structure*> loadedStructures;

            static void loadStructures(std::string filePath) {
                std::ifstream structuresFile (filePath);
                if (!structuresFile) {
                    throw std::runtime_error("Could not open \"" + filePath + "\" - For Structures.");
                }
                structuresFile >> FE::structures::structuresJson;
            }
            static void insertStructure(motus::Position pos, float hp, std::string subId, std::string owner, int type) {\
                if (FE::structures::loadedStructures.find(subId) != FE::structures::loadedStructures.end()) {
                    structure* newStr = new structure(pos, hp, subId, owner, type);
                    FE::structures::loadedStructures.at(subId) = newStr;
                    return;
                }
                else {
                     structure* newStr = new structure(pos, hp, subId, owner, type);
                    FE::structures::loadedStructures.insert({subId, newStr});
                    motus::Position* newPos = new motus::Position;
                    newPos->lat = pos.lat;
                    newPos->lon = pos.lon;
                    FE::structures::string_pos_map.insert({newPos, subId});
                }
            }
            static void addAllStructures(json data) {
                for (const auto& item :data) {
                    std::string subId = item["data"]["subId"];
                    std::string owner = item["owner"];
                    int type = item["type"];
                    float hp = item["data"]["hp"];
                    motus::Position sPos;
                    sPos.lat = item["position"]["lat"].get<double>();
                    sPos.lon = item["position"]["long"].get<double>();

                    FE::structures::insertStructure(sPos, hp, subId, owner, type);
                    FE::core::insertRect(sPos.lat, sPos.lat, sPos.lon, sPos.lon);
                }
            }
            static void saveAll(std::string filePath) {
                json updatedArray = json::array();
                for(const auto& pair : loadedStructures) {
                    json str = {
                        {"owner", pair.second->Owner},
                        {"type", pair.second->Type},
                        {"name", "noName"},
                        {"position", {
                            {"lat", pair.second->sPos.lat},
                            {"long", pair.second->sPos.lon}
                        }},
                        {"data", {
                            {"hp", pair.second->HP},
                            {"subId", pair.second->SubId},
                            {"hasInventory", false},
                            {"isOnline", false},
                            {"bootTime", 1800000},
                            {"bootStarted", 0}
                        }}
                    };

                    updatedArray.push_back(str);
                }
                structuresJson = updatedArray;
                std::ofstream outFile(filePath);
                if (!outFile) {
                    throw std::runtime_error("Could not open \"" + filePath + "\" for writing.");
                }
                outFile << structuresJson.dump(4);
            }
    };
}

// load files and stuff like that

#endif
