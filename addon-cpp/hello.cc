#include <napi.h>

// The function to be called from JavaScript
Napi::Value Hello(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    return Napi::String::New(env, "Hello from C++!");
}

// Register the function to the exports object
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "hello"), Napi::Function::New(env, Hello));
    return exports;
}

// Macro to initialize the module
NODE_API_MODULE(my_addon, Init)
