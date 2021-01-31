#pragma once

#include "core/Arg.hpp"
#include "core/CString.hpp"

#include "debug.hpp"
#include <memory.h>
#include <typeinfo>
#include <string>

#include "core/StringSpan.hpp"

extern const std::string gEmptyString;

#define SL(x) x
#define TS(x) x
#define FS(fmt, ...) string_format(fmt, __VA_ARGS__)

inline std::string indent( size_t spaces )
{
    std::string s;
    
    for( size_t i = 0; i < spaces; i ++ )
    {
        s += SL("    ");
    }
    
    return s;
}

std::string formatString(const char* fmt, ...);

namespace std
{
	inline std::string to_string(CStrArg s) {
		return s.c_str();
	}

	inline std::string to_string(const std::type_info& ti) {
		return ti.name();
	}

	inline const std::string& to_string(const std::string& s) {
		return s;
	}
}

struct OmStringEscapeItem
{
    const char* mpSequenceToEscape;
    const char* mpEscapeSequence;
};

struct OmStringEscapeItems
{
    OmStringEscapeItem* mpItems;
    size_t mNumItems;
};

extern OmStringEscapeItems OM_STRING_C_ESCAPE_ITEMS;

void OmStringEscape(
    std::string& stringOut,
    StringSpan s,
    Arg< OmStringEscapeItems >::Type escapeItems = OM_STRING_C_ESCAPE_ITEMS );