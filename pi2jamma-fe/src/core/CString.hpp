#pragma once

#include "core/Arg.hpp"
#include "core/Compare.hpp"
#include "core/debug.hpp"

#include <memory.h>
#include <string>

extern const char* OM_EMPTY_C_STRING;

class CStr
{
public:

    CStr(const std::string& str)
        : mpStr(str.c_str())
    {
    }

    CStr(const char* pStr = "")
        : mpStr(pStr) {
    }

    size_t size() const {
        return strlen(mpStr);
    }

    bool empty() const {
        return mpStr[0] == '\0';
    }

    bool operator==(const CStr& rhs) const {
        return strcmp(mpStr, rhs.mpStr) == 0;
    }

    bool operator<(const CStr& rhs) const {
        return strcmp(c_str(), rhs.c_str()) < 0;
    }

    bool operator>(const CStr& rhs) const {
        return strcmp(c_str(), rhs.c_str()) > 0;
    }

    const char* c_str() const { return mpStr; }

private:

    const char* mpStr;
};


template<>
struct Arg<CStr>
{
    using Type = CStr;
};


template<>
struct Comparer<CStr>
{
    static int Compare(CStr s1, CStr s2) {
        return strcmp(s1.c_str(),s2.c_str());
    }

};

using CStrArg = Arg<CStr>::Type;

inline bool EndsWith( const char* pS, const char* pE )
{
    const char* pRes = strstr( pS, pE );
    
    return pRes == ( pS + strlen( pS ) - strlen( pE ) );
}

void OmCStringDumpHex( const char* pSIn );
