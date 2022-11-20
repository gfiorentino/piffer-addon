#include <napi.h>
#include "../deps/rapidjson/include/rapidjson/document.h"
#include "../deps/rapidjson/include/rapidjson/writer.h"
#include "../deps/rapidjson/include/rapidjson/stringbuffer.h"
#include "../deps/rapidjson/include/rapidjson/istreamwrapper.h"
#include "piffero/path_parser.h"
#include "piffero/jsonpath.h"
#include <fstream>
#include <vector>
#include <chrono>
#include <thread>

using namespace rapidjson;
using namespace jpath;

void CallEmit(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);
    
    Napi::String pathString = info[1].As<Napi::String>();

    std::ifstream ifs("files/large-file.json");
    IStreamWrapper isw(ifs);

    vector<PathInfo> vectorPathInfo = JsonPath::parseToVector(pathString);
    jpath::JSONParser<IStreamWrapper> parser;
    //callback
    Napi::Function emit = info[0].As<Napi::Function>();
    parser.parsePath(isw, vectorPathInfo, emit, ifs, env);
    emit.Call({Napi::String::New(env, "start")});
    emit.Call({Napi::String::New(env, "end")});
}



// Init
Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "callEmit"),
              Napi::Function::New(env, CallEmit));
  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init);
