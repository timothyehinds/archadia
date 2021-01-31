#include "core/StringUtil.hpp"

const std::string gEmptyString("");

OmStringEscapeItem OM_STRING_C_ESCAPE_ITEM_VALUES[] = {
    { "\\", "\\\\" },
    { "\n", "\\n" },
    { "\t", "\\t" },
    { "\"", "\\\"" } };

OmStringEscapeItems OM_STRING_C_ESCAPE_ITEMS = {
        OM_STRING_C_ESCAPE_ITEM_VALUES,
        sizeof ( OM_STRING_C_ESCAPE_ITEM_VALUES ) / sizeof( OmStringEscapeItem ) };

void OmStringEscape(
    std::string& stringOut,
    StringSpan s,
    Arg< OmStringEscapeItems >::Type escapeItems )
{
    stringOut.clear();

    auto len = s.size();

    for(size_t i = 0 ;i < len; i ++)
    {
        size_t left = len - i;

        bool found = false;

        for(size_t j = 0; j < escapeItems.mNumItems; j ++ )
        {
            const char* pPattern = escapeItems.mpItems[j].mpSequenceToEscape;
            size_t patternLength = strlen(pPattern);

            if( patternLength <= left)
            {
                bool patternFound = true;
                for(size_t p = 0; p < patternLength; p ++ )
                {
                    if(pPattern[p] != s[i+p])
                    {
                        patternFound = false;
                        break;
                    }
                }

                if(patternFound) {
                    const char* pReplacementPattern = escapeItems.mpItems[j].mpEscapeSequence;

                    stringOut.insert(i, pReplacementPattern);                    
                    found = true;
                    break;
                }
            }
        }

        if(! found) {
            stringOut.push_back(s[i]);
        }
    }
}

#include <stdarg.h>  // For va_start, etc.
#include <memory>    // For std::unique_ptr

std::string formatString(const char* fmt, ...) {
    size_t final_n;
    size_t n = strlen(fmt) * 2; /* Reserve two times as much as the length of the fmt_str */
    std::unique_ptr<char[]> formatted;
    va_list ap;
    while(1) {
        formatted.reset(new char[n]); /* Wrap the plain char array into the unique_ptr */
        strcpy(&formatted[0], fmt);
        va_start(ap, fmt);
        final_n = vsnprintf(&formatted[0], n, fmt, ap);
        va_end(ap);
        if (final_n < 0 || final_n >= n)
            n += abs(final_n - n + 1);
        else
            break;
    }
    return std::string(formatted.get());
}