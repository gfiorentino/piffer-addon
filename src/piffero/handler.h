#ifndef JSONPATHaa_H_
#define JSONPATHaa_H_
#include "jsonpath.h"
#include <napi.h>
#include <vector>
#include <stdexcept>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "../../deps/rapidjson/include/rapidjson/rapidjson.h"
#include "../../deps/rapidjson/include/rapidjson/schema.h"
#include "../../deps/rapidjson/include/rapidjson/writer.h"
#include "../../deps/rapidjson/include/rapidjson/filewritestream.h"

using namespace rapidjson;
using namespace std;
using namespace Napi;

#pragma once
namespace jpath
{

	enum Last
	{
		startobject = 0,
		endobject = 1,
		startarray = 2,
		endarray = 3,
		value = 4,
		key = 5
	};

	struct SingleStepHandler
	{
		string pathPrimoLivello;
		string lastKey;
		long pathItemId;
		bool recording;
		bool isArray;
		bool closed;
		int depthCounter;
		bool isMatching;
		int currentIndex;
		bool isLast_;
		Last last;
	    ofstream& stream; 
		istream& origin;

		SingleStepHandler(PathInfo &path, bool isLast,  ofstream &_stream, istream& _origin) : origin(_origin), stream(_stream)
		{
			pathItemId = path.itemsId;
			recording = false;
			isArray = path.itemsId >= 0;
			closed = false;
			depthCounter = 0;
			isMatching = false;
			currentIndex = -1;
			lastKey = "";
			isLast_ = isLast;
			pathPrimoLivello = path.primoLivello;
		}

		// used to add escape characters 
		std::string escape_json(const std::string &s)
		{
			std::ostringstream o;
			for (auto c = s.cbegin(); c != s.cend(); c++)
			{
				switch (*c)
				{
				case '"':
					o << "\\\"";
					break;
				case '\\':
					o << "\\\\";
					break;
				case '\b':
					o << "\\b";
					break;
				case '\f':
					o << "\\f";
					break;
				case '\n':
					o << "\\n";
					break;
				case '\r':
					o << "\\r";
					break;
				case '\t':
					o << "\\t";
					break;
				default:
					if ('\x00' <= *c && *c <= '\x1f')
					{
						o << "\\u"
						  << std::hex << std::setw(4) << std::setfill('0') << (int)*c;
					}
					else
					{
						o << *c;
					}
				}
			}
			return o.str();
		}
		
		void closeStream(){
			recording = false;
			closed = true;
			//origin.setstate(std::ios::eofbit);		
		}

		bool StartObject()
		{
			if (recording && isLast_)
				if (depthCounter == 0 || (isMatching && depthCounter == 1))
				{
					this->closeStream();
				}
				else
				{
					addCommaIfNeeded();
					stream << "{";
				}
			last = startobject;
			depthCounter++;
			return true;
		}

		bool EndObject(SizeType memberCount)
		{
			if (recording && isLast_)
			{
				if (depthCounter == 1)
				{
					this->closeStream();
				}
				else
				{
					stream << "}";
				}
			}
			last = endobject;
			depthCounter--;
			return true;
		}

		bool StartArray()
		{
			if (recording && isLast_)
			{
				addCommaIfNeeded();
				stream << "[";
			}
			last = startarray;
			depthCounter++;
			return true;
		}

		bool EndArray(SizeType elementCount)
		{
			if (recording && isLast_)
			{
				if (depthCounter == 1)
				{
					this->closeStream();
					return true;
				}
				else
				{		
					stream << "]";
				}
			}
			last = endarray;
			depthCounter--;
			return true;
		}

		bool Key(const char *str, SizeType length, bool copy)
		{
			// capire come popolare key dal buffer
			string keystring = str;
			if (depthCounter == 1 && recording)
			{

				this->closeStream();
				return true;
			}
			if (recording && isLast_)
			{
				addCommaIfNeeded();
				stream << "\"" << str << "\":";
			}
			else if (pathPrimoLivello.compare(keystring) == 0 && depthCounter <= 1)
			{
				if (depthCounter == 0)
				{
					depthCounter = 1;
				}
				if (!isArray)
				{
					recording = true;
				}
				else
				{
					isMatching = true;
				}
			}
			else if (isMatching && depthCounter == 3 && last == startobject)
			{
				currentIndex++;
				if (pathItemId == currentIndex)
				{
					if (isLast_)
					{
						addCommaIfNeeded();				
					    stream << "{\"" << str << "\":";
					}
					recording = true;
					depthCounter--;
				}
			}
			last = key;
			lastKey = keystring;
			return true;
		}

		bool Null()
		{
			if (recording && isLast_)
			{
				addCommaIfNeeded();
				stream << "null";

				if (depthCounter == 1)
				{
					this->closeStream();
				}
			}
			last = value;
			return true;
		}

		bool Bool(bool b)
		{
			if (recording && isLast_)
			{
				addCommaIfNeeded();
				if (b)
				{
					stream << "true";
				}
				else
				{
					stream << "false";
				}

				if (depthCounter == 1)
				{
					this->closeStream();
				}
			}
			last = value;
			return true;
		}

		bool Int(int i)
		{
			if (recording && isLast_)
			{
				addCommaIfNeeded();
				stream << to_string(i).c_str();
				;

				if (depthCounter == 1)
				{
					this->closeStream();
				}
			}
			last = value;
			return true;
		}

		bool Uint(unsigned u)
		{
			if (recording && isLast_)
			{
				addCommaIfNeeded();
				stream << to_string(u).c_str();
				;

				if (depthCounter == 1)
				{
					this->closeStream();
				}
			}
			last = value;
			return true;
		}

		bool Int64(int64_t i)
		{
			if (recording && isLast_)
			{
				addCommaIfNeeded();
				stream << to_string(i).c_str();

				if (depthCounter == 1)
				{
					this->closeStream();
				}
			}
			last = value;
			return true;
		}
		bool Uint64(uint64_t u)
		{
			if (recording && isLast_)
			{
				addCommaIfNeeded();
				stream << to_string(u).c_str();


				if (depthCounter == 1)
				{
					this->closeStream();
				}
			}
			last = value;
			return true;
		}

		bool Double(double d)
		{
			if (recording && isLast_)
			{
				addCommaIfNeeded();
				stream << to_string(d).c_str();

				if (depthCounter == 1)
				{
					this->closeStream();
				}
			}
			last = value;
			return true;
		}

		bool RawNumber(const char *str, SizeType length, bool copy)
		{
			if (recording && isLast_)
			{
				addCommaIfNeeded();
				stream << str;
				if (depthCounter == 1)
				{
					this->closeStream();
				}
			}
			last = value;
			return true;
		}

		bool String(const char *str, SizeType length, bool)
		{
			if (recording && isLast_)
			{
				addCommaIfNeeded();
				if ((str == NULL) || (str[0] == '\0'))
				{
					stream << "\"\"";
				}
				else
				{
				    stream << "\"" << escape_json(str).c_str() << "\"";
				}
				if (depthCounter == 1)
				{
					this->closeStream();
				}
			}
			last = value;
			return true;
		}

		void addCommaIfNeeded()
		{
			if (last == endarray || last == endobject || last == value)
			{
				stream << ",";
			}
		}
	};

	class MasterHandler
	{
	public:
		vector<SingleStepHandler> &handlerVector_;
		bool isLast = false;
		int parserIndex = 0;
		MasterHandler(vector<SingleStepHandler> &handlerVector) : handlerVector_(handlerVector)
		{
			isLast = handlerVector_.size() == 1;
		}
		void shiftParser()
		{
			if (handlerVector_[parserIndex].recording && !isLast)
			{
				parserIndex++;
				isLast = parserIndex == handlerVector_.size() - 1;
			}
		}

		bool Null()
		{
			if (handlerVector_[parserIndex].closed)
				return true;
			handlerVector_[parserIndex].Null();
			shiftParser();
			return true;
		}
		bool Bool(bool b)
		{
			if (handlerVector_[parserIndex].closed)
				return true;
			handlerVector_[parserIndex].Bool(b);
			shiftParser();
			return true;
		}
		bool Int(int i)
		{
			handlerVector_[parserIndex].Int(i);
			shiftParser();
			return true;
		}

		bool Uint(unsigned u)
		{
			if (handlerVector_[parserIndex].closed)
				return true;
			handlerVector_[parserIndex].Uint(u);
			shiftParser();
			return true;
		}
		bool Int64(int64_t i)
		{
			handlerVector_[parserIndex].Int64(i);
			shiftParser();
			return true;
		}
		bool Uint64(uint64_t u)
		{
			if (handlerVector_[parserIndex].closed)
				return true;
			handlerVector_[parserIndex].Uint64(u);
			shiftParser();
			return true;
		}
		bool Double(double d)
		{
			if (handlerVector_[parserIndex].closed)
				return true;
			handlerVector_[parserIndex].Double(d);
			shiftParser();
			return true;
		}
		bool RawNumber(const char *str, SizeType length, bool copy)
		{
			if (handlerVector_[parserIndex].closed)
				return true;
			handlerVector_[parserIndex].RawNumber(str, length, copy);
			shiftParser();
			return true;
		}

		bool String(const char *str, SizeType length, bool copy)
		{
			if (handlerVector_[parserIndex].closed)
				return true;
			handlerVector_[parserIndex].String(str, length, copy);
			shiftParser();
			return true;
		}
		bool StartObject()
		{
			if (handlerVector_[parserIndex].closed)
				return true;
			handlerVector_[parserIndex].StartObject();
			shiftParser();
			return true;
		}
		bool Key(const char *str, SizeType length, bool copy)
		{
			if (handlerVector_[parserIndex].closed)
				return true;
		 	handlerVector_[parserIndex].Key(str, length, copy);
			shiftParser();
			return true;
		}
		bool EndObject(SizeType memberCount)
		{
			if (handlerVector_[parserIndex].closed)
				return true;
			handlerVector_[parserIndex].EndObject(memberCount);
			shiftParser();
			return true;
		}
		bool StartArray()
		{
			if (handlerVector_[parserIndex].closed)
				return true;
		    handlerVector_[parserIndex].StartArray();
			shiftParser();
			return true;
		}
		bool EndArray(SizeType elementCount)
		{
			if (handlerVector_[parserIndex].closed)
				return true;
			handlerVector_[parserIndex].EndArray(elementCount);
			shiftParser();
			return true;
		}
	};

}// namespace jpath
#endif