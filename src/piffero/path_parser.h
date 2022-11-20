#ifndef JSONPATHEE_H_
#define JSONPATHEE_H_
#include <napi.h>
#include "jsonpath.h"
#include "handler.h"
#include "../../deps/rapidjson/include/rapidjson/rapidjson.h"
#include "../../deps/rapidjson/include/rapidjson/schema.h"
#include "../../deps/rapidjson/include/rapidjson/filewritestream.h"
#include <vector>
#include <stdexcept>
#include <string>
#include <fstream>

using namespace std;
using namespace rapidjson;
namespace jpath
{

	template <typename InputStream>
	class JSONParser
	{
	public:
		JSONParser()
		{
		}
		void parsePath(InputStream &is, vector<PathInfo> &jsonpath, Napi::Function &stream, istream &origin, Napi::Env &env)
		{

			ofstream myfile;
  			myfile.open ("files/example.txt");
			vector<SingleStepHandler> handlerVector;

			for (int i = 0; i < jsonpath.size() - 1; i++)
			{
				SingleStepHandler handler(jsonpath[i], false, myfile, origin, env);
				handlerVector.push_back(handler);
			}

			SingleStepHandler parser(jsonpath[jsonpath.size() - 1], true, myfile, origin, env);
			handlerVector.push_back(parser);
			MasterHandler master(handlerVector);
			Reader reader;
			reader.Parse(is, master);
		}
	};

}
#endif
