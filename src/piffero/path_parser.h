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

using namespace std;
using namespace rapidjson;
namespace jpath
{

	template <typename InputStream>
	class JSONParser {
	public:
		JSONParser() {
		}
		void parsePath(InputStream& is, vector<PathInfo>& jsonpath, ofstream& stream, Napi::Function& destroy, Napi::Env& env ) {


			vector<SingleStepHandler> handlerVector;

			for (int i = 0; i < jsonpath.size() - 1; i++) {
				SingleStepHandler handler(jsonpath[i], false, stream, destroy, env);
				handlerVector.push_back(handler);
			}


			SingleStepHandler parser(jsonpath[jsonpath.size() - 1], true, stream, destroy, env);
			handlerVector.push_back(parser);
			MasterHandler master(handlerVector);
			Reader reader;
			reader.Parse(is, master);

		}
	};

}
#endif

