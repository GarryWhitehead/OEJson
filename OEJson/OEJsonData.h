#pragma once
#include "OEFile.h"
#include <sstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <vector>
#include <unordered_map>

using JsonObject = std::vector<std::string>;
using JsonArray = std::vector<std::vector<std::string> >;	// an array of objects

class OEJsonData
{
public:

	OEJsonData();
	OEJsonData(OEFile& file);
	
	OEJsonData& operator=(OEJsonData& other) = delete;
	OEJsonData& operator=(OEJsonData&& other);

	bool OEJsonData::LoadToObject(OEFile& file);
	bool HasObject(std::string object);
	size_t FindObject(std::string object);
	JsonObject InitArray(std::string object);
	JsonArray ConvertArrayToObjects(std::string objectName);

	int GetObjectInt(std::string object);
	int GetObjectInt(JsonObject& object, std::string objectName);
	std::string GetObjectString(std::string object);
	std::string GetObjectString(JsonObject& object, std::string objectName);

private:

	JsonObject m_jsonData;
	bool m_isFileOpen;
};


