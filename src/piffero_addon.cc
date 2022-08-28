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

    // nodejs callback
    Napi::Function cb = info[0].As<Napi::Function>();

    Napi::String pathString = info[1].As<Napi::String>();


    char *file_name = "large-file.json";
    int i = 0;
    const int BLOCK_SIZE = 1024;
    char buffer[BLOCK_SIZE];
    //std::ifstream ifs;

    // std::ifstream ifs;


    std::ifstream ifs("large-file.json");
    IStreamWrapper isw(ifs);
    //ifs.open(file_name, std::ifstream::binary | std::ifstream::in);

    vector<PathInfo> vectorPathInfo = JsonPath::parseToVector(pathString);
    jpath::JSONParser<IStreamWrapper> parser;
    Napi::Function emit = info[0].As<Napi::Function>();
    parser.parsePath(isw, vectorPathInfo, emit, ifs, env);
    emit.Call({Napi::String::New(env, "start")});
}



// Init
Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "callEmit"),
              Napi::Function::New(env, CallEmit));
  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init);
