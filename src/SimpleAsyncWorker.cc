#include "SimpleAsyncWorker.h"

SimpleAsyncWorker::SimpleAsyncWorker(Function &callback, std::stringstream &s, Napi::String &_path,
                                     Napi::Function &_outStream, Napi::Function &_destroy, Napi::Env &_env)
    : AsyncWorker(callback), callback(callback), stream(s), path(_path), outStream(_outStream), destroy(_destroy), env(_env){};

void SimpleAsyncWorker::Execute()
{
  std::this_thread::sleep_for(std::chrono::milliseconds(730));
 // Napi::Function emit = env.Global().Get('emit').As<Napi::Function>();
  vector<jpath::PathInfo> vectorPathInfo = JsonPath::parseToVector(path);
  IStreamWrapper isw(stream);
  jpath::JSONParser<IStreamWrapper> parser;
  ofstream myfile;
  myfile.open("files/example.json");
  parser.parsePath(isw, vectorPathInfo, myfile, destroy, env);
};

void SimpleAsyncWorker::OnOK()
{

  Napi::Function emit = Env().Global().Get('emit').As<Napi::Function>();
  emit.Call({Napi::String::New(env, "end")});
  /* Napi::String pathstring = Env().Global().Get('path').As<Napi::String>();
   cout << pathstring.ToString().Utf8Value().c_str();

   vector<jpath::PathInfo> vectorPathInfo = JsonPath::parseToVector(pathstring);
   IStreamWrapper isw(stream);
   jpath::JSONParser<IStreamWrapper> parser;
   Napi::Env env = Env();

   Napi::Function destroy = Env().Global().Get('destroy').As<Napi::Function>();
   parser.parsePath(isw, vectorPathInfo, emit, destroy, env);

    std::cout<<pathstring.ToString().Utf8Value().c_str();*/
};