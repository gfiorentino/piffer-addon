#include "SimpleAsyncWorker.h"

SimpleAsyncWorker::SimpleAsyncWorker(Function& callback, std::stringstream& s)
    : AsyncWorker(callback),  stream(s){};


void SimpleAsyncWorker::Execute() {
  
   std::this_thread::sleep_for(std::chrono::milliseconds(1));
 
 // if (runTime == 4) SetError("Oops! Failed after 'working' 4 seconds.");
};

void SimpleAsyncWorker::OnOK() {
  // std::this_thread::sleep_for(std::chrono::seconds(1));

  Napi::String pathstring = Env().Global().Get('path').As<Napi::String>();
  
      vector<jpath::PathInfo> vectorPathInfo = JsonPath::parseToVector(pathstring);
      IStreamWrapper isw(stream);
      jpath::JSONParser<IStreamWrapper> parser;
      Napi::Env env = Env();
      Napi::Function emit = Env().Global().Get('emit').As<Napi::Function>();
      Napi::Function destroy = Env().Global().Get('destroy').As<Napi::Function>();
      parser.parsePath(isw, vectorPathInfo, emit, destroy, env);
   
  // std::cout<<pathstring.ToString().Utf8Value().c_str();
};