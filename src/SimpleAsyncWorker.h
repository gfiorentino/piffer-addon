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
  SimpleAsyncWorker(Function& callback, std::stringstream& s);
  virtual ~SimpleAsyncWorker(){};

  void Execute();
  void OnOK();

  private: 
  std::stringstream& stream;
};