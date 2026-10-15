#include "FE_engine.hpp"
#include "FEmodules/FE_core.hpp"
#include "GeoOp.hpp"
#include <cstdint>
#include <iostream>
#include <napi.h>
#include <thread>

std::mutex mtx;
std::thread* engineThread = nullptr;
Napi::ThreadSafeFunction updateStructureCallback;

Napi::Value getStructures(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    uint32_t index = 0;
    Napi::Array structuresUpdatedArray = Napi::Array::New(env);
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& it : mapStructures) {
        Napi::Object structureItem = Napi::Object::New(env);

        structureItem.Set(Napi::String::New(env, "id"), Napi::String::New(env, it.second->subId));
        structureItem.Set(Napi::String::New(env, "hp"), Napi::Number::New(env, it.second->HP));
        // structureItem.Set(Napi::String::New(env, "latmin"), Napi::Number::New(env, it.second->min[0]));
        // structureItem.Set(Napi::String::New(env, "latmax"), Napi::Number::New(env, it.second->max[0]));
        // structureItem.Set(Napi::String::New(env, "longMin"), Napi::Number::New(env, it.second->min[1]));
        // structureItem.Set(Napi::String::New(env, "longMax"), Napi::Number::New(env, it.second->max[1]));
        structuresUpdatedArray.Set(index++, structureItem);
    }
    mtx.unlock();
    return structuresUpdatedArray;
}
Napi::Value getMissile(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    uint32_t index = 0;
    Napi::Array missileJSObjectArray = Napi::Array::New(env);
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& it : missileMap) {
        Napi::Object mslItem = Napi::Object::New(env);

        mslItem.Set(Napi::String::New(env, "id"), Napi::String::New(env, it.second->missileObject.id));
        mslItem.Set(Napi::String::New(env, "slat"), Napi::Number::New(env, it.second->missileObject.startPosition.lat));
        mslItem.Set(Napi::String::New(env, "slon"), Napi::Number::New(env, it.second->missileObject.startPosition.lon));
        mslItem.Set(Napi::String::New(env, "dlat"), Napi::Number::New(env, it.second->missileObject.targetPosition.lat));
        mslItem.Set(Napi::String::New(env, "dlon"), Napi::Number::New(env, it.second->missileObject.targetPosition.lon));
        mslItem.Set(Napi::String::New(env, "speed"), Napi::Number::New(env, it.second->missileObject.speed));
        missileJSObjectArray.Set(index++, mslItem);
    }
    mtx.unlock();
    return missileJSObjectArray;
}
Napi::Boolean addMissile(const Napi::CallbackInfo& inf) // bro, it takes String: ID, DoubleArray: [latMin, latMax], DoubleArray: [longMin, longMax], Float: speed
{
    Napi::Env env = inf.Env();
    if (inf.Length() < 4) {
        Napi::TypeError::New(env, "Wrong number of arguments. Expected 4.")
            .ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }
    if (!inf[0].IsString()) {
        Napi::TypeError::New(env, "Wrong argument at [0]: must be a string.")
            .ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }
    Napi::String id = inf[0].As<Napi::String>();
    Napi::Float64Array parr = inf[1].As<Napi::Float64Array>();
    Napi::Float64Array darr = inf[2].As<Napi::Float64Array>();
    Napi::Number spd = inf[3].As<Napi::Number>();
    double p[2] = { parr[0], parr[1] };
    double d[2] = { darr[0], darr[1] };
    Napi::Number warhead = inf[4].As<Napi::Number>();
    Napi::Number payload = inf[5].As<Napi::Number>();
    try {
        FE::engine::addMissile(id, p, d, spd, (int)payload, (int)warhead);
    } catch (std::exception& e) {
        std::cerr << e.what();
        return Napi::Boolean::New(env, false);
    }
    return Napi::Boolean::New(env, true);
}

Napi::Boolean startTicking(const Napi::CallbackInfo& inf)
{
    Napi::Env env = inf.Env();
    try {
        engineThread = new std::thread([]() { FE::engine::startEngine(); });
        engineThread->detach();
    } catch (const std::exception& e) {
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }
    return Napi::Boolean::New(env, true);
}
Napi::Boolean getDistanceFromStructure(const Napi::CallbackInfo& info) // takes Position p and searches in the RTree for a structure then checks for the distance between location and structure
{
    Napi::Env env = info.Env();
    motus::Position p = { info[0].As<Napi::Number>(), info[1].As<Napi::Number>() };
    return Napi::Boolean::New(env, FE::core::isWithinBB(p, info[2].As<Napi::Number>()));
}
Napi::Boolean addStuctureCpp(const Napi::CallbackInfo& inf)
{
    Napi::Env env = inf.Env();
    // letting arg 0 = subid, 1 = owner, 2 = hp, 3,4 = lat and 5,6 = long
    Napi::String id = inf[0].As<Napi::String>();
    Napi::String own = inf[1].As<Napi::String>();
    Napi::Number hp = inf[2].As<Napi::Number>();
    Napi::Number a = inf[3].As<Napi::Number>();
    Napi::Number b = inf[4].As<Napi::Number>();
    Napi::Number c = inf[5].As<Napi::Number>();
    Napi::Number d = inf[6].As<Napi::Number>();
    try {
        FE::core::insertStructure(a, b, c, d, hp, id, own);
    } catch (int e) {
        return Napi::Boolean::New(env, false);
    }
    return Napi::Boolean::New(env, true);
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "addMissile"),
        Napi::Function::New(env, addMissile));
    exports.Set(Napi::String::New(env, "startEngine"),
        Napi::Function::New(env, startTicking));
    exports.Set(Napi::String::New(env, "addStr"),
        Napi::Function::New(env, addStuctureCpp));
    exports.Set(Napi::String::New(env, "isInBB"), Napi::Function::New(env, getDistanceFromStructure));
    exports.Set(Napi::String::New(env, "getMissile"), Napi::Function::New(env, getMissile));
    exports.Set(Napi::String::New(env, "getStructures"), Napi::Function::New(env, getStructures));
    return exports;
}

NODE_API_MODULE(addon, Init)
