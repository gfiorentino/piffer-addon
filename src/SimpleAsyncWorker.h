#pragma once
#include "../deps/rapidjson/include/rapidjson/document.h"
#include "../deps/rapidjson/include/rapidjson/writer.h"
#include "../deps/rapidjson/include/rapidjson/stringbuffer.h"
#include "../deps/rapidjson/include/rapidjson/istreamwrapper.h"
#include "piffero/path_parser.h"
#include "piffero/jsonpath.h"
#include <napi.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <vector>

using namespace Napi;
using namespace jpath;


class SimpleAsyncWorker : public AsyncWorker {
 public:
  SimpleAsyncWorker(Function& callback, std::stringstream& s, Napi::String& _path, 
  Napi::Function& _outStream, Napi::Function& _destroy, Napi::Env& _env );
  virtual ~SimpleAsyncWorker(){};

  void Execute();
  void OnOK();

  private: 
  Function& callback;
  std::stringstream& stream;
  std::string path;
  Napi::Function& outStream;
  Napi::Function& destroy;
  Napi::Env& env; 

};