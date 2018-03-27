#include "OEJsonData.h"
#include <iostream>

OEJsonData::OEJsonData()
{
}

OEJsonData::OEJsonData(OEFile& file)
{
	this->LoadToObject(file);
}

OEJsonData& OEJsonData::operator=(OEJsonData&& other)
{
	m_jsonData = other.m_jsonData;
	other.m_jsonData.clear();
	return *this;
}

bool OEJsonData::LoadToObject(OEFile& file)
{
	if (!file.m_file.is_open()) {
		return false;
	}
	m_isFileOpen = true;

	m_jsonData.clear();
	
	int openCount = 0;
	int closedCount = 0;

	std::string str;
	while (std::getline(file.m_file, str)) {

		size_t pos = str.find('{');
		if (pos != std::string::npos) {
			++openCount;
		}
		else {
			pos = str.find('}');
			if (pos != std::string::npos) {
				++closedCount;
			}
		}

		m_jsonData.push_back(str);
		std::cout << str << "\n";

		if (openCount == closedCount) {
			return true;
		}
	}
	return false;
}

bool OEJsonData::HasObject(std::string object)
{
	if (!m_isFileOpen || object.empty()) {
		return false;
	}

	for(auto& str : m_jsonData) {
		size_t pos = str.find(object);
		if (pos != std::string::npos) {
			return true;
		}
	}
	return false;
}

size_t OEJsonData::FindObject(std::string object)
{
	if (!m_isFileOpen || object.empty()) {
		return false;
	}

	for(auto& str : m_jsonData) {
		size_t pos = str.find(object);
		if (pos != std::string::npos) {
			return pos;
		}
	}
	return -1;
}

JsonObject OEJsonData::InitArray(std::string object)
{
	size_t pos = 0;
	int index = 0;
	JsonObject arrayObj;
	arrayObj.clear();

	// satrt by finding the array by name
	while (index < m_jsonData.size()) {
		std::string str = m_jsonData[index];

		size_t pos = str.find(object);
		if (pos != std::string::npos) {
			break;
		}
		++index;
	}

	// if index is the same as the end of the data, then this array isn't present
	if (index == m_jsonData.size()) {
		return arrayObj;
	}

	// ensure that we have an array at this position 
	pos = m_jsonData[index].find_last_of('[');
	if (pos == std::string::npos) {
		return arrayObj;
	}
		
	// now find the closing brace, copying all the data into the array as we go
	while (index < m_jsonData.size()) {
		std::string str = m_jsonData[index];
		
		pos = str.find("]");
		if (pos == std::string::npos) {
			arrayObj.push_back(str);
		}
		else {
			// found the closing brace so return the array
			return arrayObj;
		}
		++index;
	}
	return arrayObj;			//	if the function return here, then the json file is not properly formated and the closing brace for the array has not been found;
}

JsonArray OEJsonData::ConvertArrayToObjects(std::string objectName)
{
	JsonObject object = {};
	JsonArray objectArray = {};
	size_t pos = 0;
	int ID = 0, index = 0;

	if (!m_isFileOpen || objectName.empty()) {
		return objectArray;
	}

	// start by initialising array - find object and extract into seperate object
	JsonObject data = this->InitArray(objectName);
	if (data.empty()) {
		return objectArray;
	}

	std::string str;
	while (index < data.size()) {

		object.clear();
		std::string str = data[index];
		pos = str.find('{');
		if (pos != std::string::npos) {
	
			while (index < data.size()) {
				++index;
				str = data[index];
				pos = str.find('}');
				if (pos == std::string::npos) {

					object.push_back(str);
				}
				else {
					objectArray.push_back(object);
					break;
				}
			}
		}
		++index;
	}
	return objectArray;
}

int OEJsonData::GetObjectInt(std::string object)
{
	if (!m_isFileOpen || object.empty()) {
		return false;
	}

	for (auto& str : m_jsonData) {

		size_t pos = str.find(object);
		if (pos != std::string::npos) {
			
			pos = str.find_last_of(':');
			
			// remove all characers befor and including the ":" symbol as the data is specfied after this
			std::string data = str.substr(pos + 1, str.size());
		
			size_t lastPos = data.find_last_of('"');
			// if the string dosent contain " then assume its a number
			if (lastPos == std::string::npos) {

				// remove all non-alpha numeric characters
				data.erase(std::remove_if(data.begin(), data.end(), [](auto const& c) -> bool {return !std::isalnum(c); }), data.end());
				
				if (!data.empty()) {
					int value = std::stoi(data);
					return value;
				}
			}
		}
	}
}

int OEJsonData::GetObjectInt(JsonObject& object, std::string objectName)
{
	if (!m_isFileOpen || objectName.empty()) {
		return false;
	}

	for (auto& str : object) {

		size_t pos = str.find(objectName);
		if (pos != std::string::npos) {
	
			pos = str.find_last_of(':');

			// remove all characers befor and including the ":" symbol as the data is specfied after this
			std::string data = str.substr(pos + 1, str.size());
				
			// remove all non-alpha numeric characters
			data.erase(std::remove_if(data.begin(), data.end(), [](auto const& c) -> bool {return !std::isalnum(c); }), data.end());
			if (!data.empty()) {
				int value = std::stoi(data);
				return value;
			}
		}
	}
}

std::string OEJsonData::GetObjectString(std::string objectName)
{
	if (!m_isFileOpen || objectName.empty()) {
		return false;
	}

	for (auto& str : m_jsonData) {

		size_t pos = str.find(objectName);
		if (pos != std::string::npos) {
			
			pos = str.find_first_of(':');

			// remove all characers befor and including the ":" symbol as the data is specfied after this
			std::string data = str.substr(pos + 1, str.size());
			
			size_t lastPos = data.find_last_of('"');
			// if the string dosent contain " then assume its a number
			if (lastPos != std::string::npos) {

				size_t firstPos = data.find_first_of('"');
				// If the string contains "" assume that the data is a string
				if (firstPos != lastPos) {
					data = data.substr(firstPos + 1, lastPos - 2);
					
					if (!data.empty()) {
						return data;
					}
				}
			}
		}
	}
}

std::string OEJsonData::GetObjectString(JsonObject& object, std::string objectName)
{
	if (!m_isFileOpen || objectName.empty()) {
		return false;
	}

	for (auto& str : object) {

		size_t pos = str.find(objectName);
		if (pos != std::string::npos) {

			pos = str.find_last_of(':');

			// remove all characers befor and including the ":" symbol as the data is specfied after this
			std::string data = str.substr(pos + 1, str.size());

			size_t lastPos = data.find_last_of('"');

			// make sure that it is an integer and not a string
			if (lastPos != std::string::npos) {

				size_t firstPos = data.find_first_of('"');
				// If the string contains "" assume that the data is a string
				if (firstPos != lastPos) {
					data.erase(std::remove_if(data.begin(), data.end(), [](auto const& c) -> bool {return c == '"'; }), data.end());
					if (!data.empty()) {
						return data;
					}
				}
			}
		}
	}
}