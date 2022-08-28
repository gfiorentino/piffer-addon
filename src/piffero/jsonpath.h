#ifndef JSONPATH_H_
#define JSONPATH_H_
#include <string>
#include <vector>
#include <stdexcept>

using namespace std;

namespace jpath
{
	struct PathInfo {
		string path;
		vector<string> level;
		string primoLivello;
		string nextPath;
		int numLivelli;
		bool isLastLevel;
		int itemsId;
	};


	struct JsonPath {

		/**
		* Fa il parsing di un path e ne restituisce un PathInfo
		*/
		static PathInfo parse(string path) {

			if (path.empty() ) {
				throw invalid_argument("Invalid path");
			}

			vector<string> level = JsonPath::split(path, ".");
			string primoLivello = level[0];
			int numLivelli = level.size();
			bool isLastLevel = (numLivelli == 1);

			vector<string> nextLivelli = JsonPath::slicing(level, 1, numLivelli - 1);
			
			string nextPath = JsonPath::join(nextLivelli, ".");

			int itemsId = -1;
			if ( primoLivello.find('[') != string::npos) {
				vector<string> primoLivelloParams = JsonPath::split(primoLivello, "[");
				primoLivello = primoLivelloParams[0];
				string tmp = primoLivelloParams[1];
				string itemsIdStr = tmp.substr(0, tmp.size() - 1);
				if (itemsIdStr.empty() ) {
					throw invalid_argument("Invalid id " + itemsIdStr + " in " + path);
				}
				itemsId = atoi( itemsIdStr.c_str() );
			}

			return { path, level, primoLivello, nextPath, numLivelli, isLastLevel, itemsId };
		}

		static vector<PathInfo> parseToVector(string path) {
			
			vector<PathInfo> result;
			string currentPath = path;
			bool ultimoLivello;
			do {
				PathInfo info =  JsonPath::parse(currentPath);
				result.push_back(info);
				ultimoLivello = info.isLastLevel;
				currentPath = info.nextPath;
			}
			while (!ultimoLivello);
			return result;
		}

		static vector<string> split(string str, string token) {
			vector<string>result;
			while (str.size()) {
				int index = str.find(token);
				if (index != string::npos) {
					result.push_back(str.substr(0, index));
					str = str.substr(index + token.size());
					if (str.size() == 0)result.push_back(str);
				}
				else {
					result.push_back(str);
					str = "";
				}
			}
			return result;
		}

		static vector<string> slicing(vector<string>& arr, int X, int Y) {

			// Starting and Ending iterators 
			auto start = arr.begin() + X;
			auto end = arr.begin() + Y + 1;

			// To store the sliced vector 
			vector<string> result(Y - X + 1);

			// Copy vector using copy function() 
			copy(start, end, result.begin());

			// Return the final sliced vector 
			return result;
		}

		static string join(const std::vector<std::string> &lst, const std::string &delim)
		{
			std::string ret;
			for (const auto &s : lst) {
				if (!ret.empty())
					ret += delim;
				ret += s;
			}
			return ret;
		}
	};

}
#endif