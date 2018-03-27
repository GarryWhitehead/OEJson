#include "stdafx.h"
#include "OEFile.h"


OEFile::OEFile()
{
}

OEFile::OEFile(std::string filename)
{
	m_filename = filename;
	this->Open(filename);
}

OEFile& OEFile::operator=(std::string filename)
{
	m_filename = filename;
	this->Open(filename);
}

bool OEFile::Open(std::string filename)
{
	m_isOpen = false;

	if (filename.empty()) {
		return false;
	}

	m_file.open(filename, m_file.in);
	if (!m_file.is_open()) {
		return false;
	}

	m_isOpen = true;
	return true;
}

void OEFile::Close()
{
	if (m_file.is_open()) {
		m_file.close();
	}
}

void OEFile::Delete()
{
	remove(m_filename.c_str());
}
