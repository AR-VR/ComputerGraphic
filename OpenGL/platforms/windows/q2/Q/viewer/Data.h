#ifndef __DATA_H_
#define __DATA_H_
#pragma once
#include <stdint.h>

// OS Specific header files
#include <Winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <cstdarg>

#pragma warning(disable:4200)

#define STDCALL WINAPI // Calling convention

inline int snprintf(char* str, size_t size, const char* format, ...)
{
    size_t count;
    va_list ap;
    va_start(ap, format);
    count = _vscprintf(format, ap);
    _vsnprintf_s(str, size, _TRUNCATE, format, ap);
    va_end(ap);
    return (int)count;
}

#include <string.h>

#ifndef INC
	#ifdef _MSC_VER
		#ifdef _WIN64
			#define INC ::InterlockedIncrement64
		#else
			#define INC ::InterlockedIncrement
		#endif
	#else
		#define INC(a) __sync_add_and_fetch((a), 1)
	#endif
#endif

#ifndef DEC
	#ifdef _MSC_VER
		#ifdef _WIN64
			#define DEC ::InterlockedDecrement64
		#else
			#define DEC ::InterlockedDecrement
		#endif
	#else
		#define DEC(a) __sync_sub_and_fetch((a), 1)
	#endif
#endif

class CData
{
public:
    CData()
    {
        m_plData = NULL;
    }

    CData(size_t dwSize) 
    {
        if(dwSize > 0) {
#ifdef _MSC_VER
            m_plData = (PLONG_PTR) HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY, dwSize + sizeof(LONG_PTR) * 2);
#else
            m_plData = (PLONG_PTR) new char[dwSize + sizeof(LONG_PTR) * 2];
#endif
        } else {
            m_plData = NULL;
        }
        if(m_plData != NULL) {
#ifndef _MSC_VER
            memset(m_plData, 0, dwSize + sizeof(LONG) * 2);
#endif
            m_plData[1] = dwSize;
        }
        GetReference();
    }

    CData(const void* pData, size_t dwSize)
    {
        if(dwSize > 0 && pData != NULL) {
#ifdef _MSC_VER
            m_plData = (PLONG_PTR) HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY, dwSize + sizeof(LONG_PTR) * 2);
#else
            m_plData = (PLONG_PTR) new char[dwSize + sizeof(LONG_PTR) * 2];
#endif
        } else {
            m_plData = NULL;
        }
        if(m_plData != NULL) {
			m_plData[0] = 0;
            m_plData[1] = dwSize;
            memcpy(((PBYTE)&m_plData[2]), pData, dwSize);
        }
        GetReference();
    }

    CData(CData data1, CData data2)
    {
        if(data1.m_plData == NULL) {
            m_plData = (PLONG_PTR) data2.GetReference();
        } else if(data2.m_plData == NULL) {
            m_plData = (PLONG_PTR) data1.GetReference();
        } else {
            m_plData = (PLONG_PTR) CData(data1.GetData(), data1.GetSize(), data2.GetData(), data2.GetSize()).GetReference();
        }
    }

    CData(CData data, const void* pData, size_t dwSize)
    {
        if(data.m_plData == NULL) {
            m_plData = (PLONG_PTR) CData(pData, dwSize).GetReference();
        } else if((pData == NULL) || (dwSize == 0)) {
            m_plData = (PLONG_PTR) data.GetReference();
        } else {
            m_plData = (PLONG_PTR) CData(data.GetData(), data.GetSize(), pData, dwSize).GetReference();
        }
    }

    CData(const void* pData, size_t dwSize, CData data)
    {
        if((pData == NULL) || (dwSize == 0)) {
            m_plData = (PLONG_PTR) data.GetReference();
        } else if(data.m_plData == NULL) {
            m_plData = (PLONG_PTR) CData(pData, dwSize).GetReference();
        } else {
            m_plData = (PLONG_PTR) CData(pData, dwSize, data.GetData(), data.GetSize()).GetReference();
        }
    }

    CData(const void* pData1, size_t dwSize1, const void* pData2, size_t dwSize2)
    {
        if(dwSize1 == 0 || pData1 == NULL) {
            m_plData = (PLONG_PTR) CData(pData2, dwSize2).GetReference();
        } else if(dwSize2 == 0 || pData2 == NULL) {
            m_plData = (PLONG_PTR) CData(pData1, dwSize1).GetReference();
        } else {
#ifdef _MSC_VER
            m_plData = (PLONG_PTR) HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY, dwSize1 + dwSize2 + sizeof(LONG_PTR) * 2);
#else
            m_plData = (PLONG_PTR) new char[dwSize1 + dwSize2 + sizeof(LONG_PTR) * 2];
#endif
            if(m_plData != NULL) {
#ifndef _MSC_VER
                memset(m_plData, 0, dwSize1 + dwSize2 + sizeof(LONG_PTR) * 2);
#endif
                m_plData[1] = dwSize1 + dwSize2;
                memcpy(((LPBYTE)&m_plData[2]), pData1, dwSize1);
                memcpy(((LPBYTE)&m_plData[2])+ dwSize1, pData2, dwSize2);
            }
            GetReference();
        }
    }

    CData(const CData &copy)
    {
        m_plData = (PLONG_PTR)copy.GetReference();
    }

    CData& operator=(const CData &source)
    {
        ClearReference((LPVOID)m_plData);
        m_plData = (PLONG_PTR)source.GetReference();
        return *this;
    }

    inline bool operator== (const CData data) const
    {
        return (m_plData == data.m_plData);
    }

    inline bool operator!= (const CData data) const
    {
        return (m_plData != data.m_plData);
    }

    ~CData(void)
    {
        ClearReference(m_plData);
        m_plData = NULL;
    }

    const void* GetData() const
    {
        if(m_plData == NULL) {
            return NULL;
        }
        return (const void*) &m_plData[2];
    }

    size_t GetSize() const
    {
        if(m_plData == NULL) {
            return 0;
        }
        return (size_t) m_plData[1];
    }

    bool IsIdentical(const void* pData, size_t dwSize) const
    {
        if(GetSize() != dwSize) {
            return false;
        }
        return (memcmp(GetData(), pData, dwSize) == 0);
    }

    bool IsIdentical(const CData data) const
    {
		if(m_plData == data.m_plData) return true;
        return IsIdentical(data.GetData(), data.GetSize());
    }

private:
	LPVOID GetReference() const
    {
        if(m_plData != NULL) {
            INC(m_plData);
        }
        return (LPVOID) m_plData;
    }

	static void ClearReference(LPVOID pVoid)
    {
        if(pVoid != NULL) {
            if(DEC((PLONG_PTR)pVoid) <= 0) {
#ifdef _MSC_VER
                HeapFree(GetProcessHeap(), 0, pVoid);
#else
                delete[] (PLONG_PTR)pVoid;
#endif
            }
        }
    }

#ifdef _MSC_VER
#pragma pack (push, 8)
    PLONG_PTR m_plData;
#pragma pack (pop)
#else
    PLONG_PTR m_plData;
#endif
};

#endif // !defined(__DATA_H_)
