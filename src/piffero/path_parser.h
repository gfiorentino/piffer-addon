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
		void parsePath(InputStream &is, vector<PathInfo> &jsonpath, istream &origin)
		{

			ofstream myfile;
  			myfile.open ("files/example.json");
			vector<SingleStepHandler> handlerVector;

			for (int i = 0; i < jsonpath.size() - 1; i++)
			{
				SingleStepHandler handler(jsonpath[i], false, myfile, origin);
				handlerVector.push_back(handler);
			}

			SingleStepHandler parser(jsonpath[jsonpath.size() - 1], true, myfile, origin);
			handlerVector.push_back(parser);
			MasterHandler master(handlerVector);
			Reader reader;
			reader.Parse(is, master);
		}
	};

}
#endif
