#pragma once

#include <string>
#include <fstream>

class OEFile
{

public:
	OEFile();
	OEFile(std::string filename);

	OEFile& operator=(std::string filename);

	bool Open(std::string filename);
	void Close();
	void Delete();

	// helper functions
	bool isOpen() const { return m_isOpen; }

	friend class OEJsonData;

private:

	std::fstream m_file;
	std::string m_filename;
	bool m_isOpen;
};

