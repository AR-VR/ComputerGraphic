#pragma once

#include <string>
#include "Data.h"

#define DATA_DEBUG 1

class CMTracerFile {
public:
    CMTracerFile();     // Default constructor for default dummy swing
    bool OpenFile(std::string fileName);
    void CloseFile();
	
	// return all grip positions in floats (x,y,z)
	int getGripPosition(CData& outData);

	// return all head positions in floats (x,y,z)
	int getHeadPosition(CData& outData);

	// return all quaternions in floats (x,y,z)
	int getQuaternion(CData& outData);

    bool isValidFile();

private:
	CData m_grip;
	CData m_head;
	CData m_quaternion;
    bool m_valid;

    enum FILE_STATE{
        DATA_TYPE = 0,
        ENTRY_COUNT,
        DATA
    };
};