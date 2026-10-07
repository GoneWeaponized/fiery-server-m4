#include "FE_fileOperations.hpp"

json FE::structures::structuresJson;
std::unordered_map<std::string, structure*> FE::structures::loadedStructures;
