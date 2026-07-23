#include "haversine.h"
#include <iostream>
#include <cmath>
#include <napi.h>

Napi::Value getDistance(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if(info.Length() < 1 || !info[0].IsArray()) {
        Napi::TypeError::New(env, "An array of 4 doubles was expected").ThrowAsJavaScriptException();
        return env.Null();
    }
    Napi::Array arr = info[0].As<Napi::Array>();
    if(arr.Length() != 4) {
        Napi::TypeError::New(env, "Not four doubles.").ThrowAsJavaScriptException();
        return env.Null();
    }
    double lt1 = arr.Get(uint32_t(0)).As<Napi::Number>().DoubleValue();
    double ln1 = arr.Get(uint32_t(1)).As<Napi::Number>().DoubleValue();
    double lt2 = arr.Get(uint32_t(2)).As<Napi::Number>().DoubleValue();
    double ln2 = arr.Get(uint32_t(3)).As<Napi::Number>().DoubleValue();
    double distance = haversine(lt1, ln1, lt2, ln2);

    return Napi::Number::New(env, distance);
}
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "getDistance"), Napi::Function::New(env, getDistance));
    return exports;
}

NODE_API_MODULE(addon, Init)
