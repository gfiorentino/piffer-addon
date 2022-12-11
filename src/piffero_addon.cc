#include <napi.h>
#include "SimpleAsyncWorker.h"
#include "../deps/rapidjson/include/rapidjson/document.h"
#include "../deps/rapidjson/include/rapidjson/writer.h"
#include "../deps/rapidjson/include/rapidjson/stringbuffer.h"
#include "../deps/rapidjson/include/rapidjson/istreamwrapper.h"
#include "piffero/path_parser.h"
#include "piffero/jsonpath.h"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

using namespace rapidjson;

struct A
{
  inline static std::stringstream s;
};

bool first = true;
int _count = 0;
SimpleAsyncWorker* asyncWorker ;

Napi::Value ondata(const Napi::CallbackInfo &info)
{
  if (!info[0].IsObject())
    printf("Something strange received");
  A::s.clear();
  A::s << info[0].ToString().Utf8Value().c_str();
  _count = _count + 1;
  if (first)
  {
   
    Napi::Function emit = info.Env().Global().Get('emit').As<Napi::Function>();
    Napi::Function destroy = info.Env().Global().Get('destroy').As<Napi::Function>();
    Napi::Env env = info.Env();
    
    first = false;
  }
  return info.Env().Undefined();
}

Napi::Value onend(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::Function emit = info.Env().Global().Get('emit').As<Napi::Function>();
  emit.Call({Napi::String::New(env, "end")});
  return info.Env().Undefined();
}

void CallEmit(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::Function emit = info[0].As<Napi::Function>();
  Napi::Object stream = info[2].ToObject();
  env.Global().Set('path', info[1]);
  env.Global().Set('emit', emit);

  Napi::Function on_data_ref = Napi::Function::New(info.Env(), ondata, "on_data");
  Napi::Function on_end_ref = Napi::Function::New(info.Env(), onend, "on_end");

  Napi::Value onValue = stream.Get("on");
  Napi::Function destroy = info[3].As<Napi::Function>();
  env.Global().Set('destroy', destroy);
  if (!onValue.IsFunction())
    throw Napi::Error::New(info.Env(), "This is not an event emitter");
  Napi::Function on = onValue.As<Napi::Function>();
   Napi::String pathstring = info.Env().Global().Get('path').As<Napi::String>();
  asyncWorker = new SimpleAsyncWorker(emit, A::s, pathstring, emit, destroy, env);
      asyncWorker->Queue();
  on.Call(stream, {Napi::String::New(info.Env(), "data"), on_data_ref});
  //on.Call(stream, {Napi::String::New(info.Env(), "end"), on_end_ref});
}

// Init
Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  exports.Set(Napi::String::New(env, "callEmit"),
              Napi::Function::New(env, CallEmit));
  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init);
