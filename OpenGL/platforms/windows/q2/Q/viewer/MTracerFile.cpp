#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <string>

#include "MTracerFile.h"
#include "DummySwing.h"

using namespace std;

CMTracerFile::CMTracerFile()
{
    /**
    // initialize variable
    m_grip = CData();
    m_head = CData();
    m_quaternion = CData();
    m_valid = false;

    /*/
    m_grip = CData(g_grippos, DUMMY_GRIP_POS_LEN * sizeof(float) * 3);
    m_head = CData(g_headpos, DUMMY_HEAD_POS_LEN * sizeof(float) * 3);
    m_quaternion = CData(g_quaternion, DUMMY_QUATERNION_LEN * sizeof(float) * 4);
    m_valid = true;
    /**/
}

bool CMTracerFile::OpenFile(string fileName) {
    return true;
}

/*
bool CMTracerFile::OpenFile(string fileName)
{
    ifstream file;
    file.open(fileName);

    // read it here
    // <Header>:<Int><float><space/line as delimiter>
    // 3 states, header, int, float
    CMTracerFile::FILE_STATE state = CMTracerFile::FILE_STATE::DATA_TYPE;
    string type = "";
    int numValidSection = 0, numEntries = 0, numEntriesRead = 0;
    map<string, CData*> dataSectionMap;
    map<string, CData*>::iterator itor;

    CData* temp;
    dataSectionMap["HeadPosition"] = &m_head;
    dataSectionMap["GripPosition"] = &m_grip;
    dataSectionMap["Quaternion"] = &m_quaternion;

    m_valid = false;
    numValidSection = 0;
    while (!file.eof() && !file.fail())
    {
        if (state == DATA_TYPE) {
            std::getline(file, type, ':');
            type.erase(std::remove(type.begin(), type.end(), '\n'), type.end());

            if (file.fail())
            {
                break;
            }
            itor = dataSectionMap.find(type);
            if (itor == dataSectionMap.end())
            {
                break;
            }
            temp = itor->second;
            state = ENTRY_COUNT;
        }
        else if (state == ENTRY_COUNT)
        {
            file >> numEntries;
            state = DATA;
            numEntriesRead = 0;
            numEntries *= (type.compare("Quaternion") == 0) ? 4 : 3;
            (*temp) = CData(sizeof(float) * numEntries);
        }
        else if (state == DATA)
        {
            float tempFloat;
            file >> tempFloat;
            ((float*)temp->GetData())[numEntriesRead++] = tempFloat;
            if (numEntries == numEntriesRead)
            {
                // Done wiht this type of data, move on!
                state = DATA_TYPE;
                numValidSection++;
            }
        }
    }
    file.close();

    // What is the good condition?
    // 1. All three arrays are populated with the correct number of entries!
    if (numValidSection == dataSectionMap.size()) {
        m_valid = true;
    }

    return m_valid;
}
*/

// return all grip positions in floats (x,y,z)
int CMTracerFile::getGripPosition(CData& data)
{
	data = m_grip;
	return 0;
}

// return all head positions in floats (x,y,z)
int CMTracerFile::getHeadPosition(CData& data)
{
	data = m_head;
	return 0;
}

// return all quaternions in floats (x,y,z)
int CMTracerFile::getQuaternion(CData& data)
{
	data = m_quaternion;
	return 0;
}

bool CMTracerFile::isValidFile()
{
    return m_valid;
}

void CMTracerFile::CloseFile()
{
    m_valid = false;
}
