#pragma once

#include "core/debug.hpp"
#include "core/Util.hpp"

inline bool OmCharIsLetter( char c );
inline bool OmCharIsNumber( char c );
inline bool OmCharIsUppercaseLetter( char c );
inline bool OmCharIsLowercaseLetter( char c );
inline bool OmCharIsUnderscore( char c );
inline bool OmCharIsNull( char c );
inline bool OmCharIsWhite( char c );
inline bool OmCharIsParenthesis( char c );

inline bool OmCharIsNumber( OmUnicode16Char c );
inline uint8_t OmCharToUInt8( OmUnicode16Char c );

//-----------------------------------------------------------------------------
// Inline functions
//-----------------------------------------------------------------------------

inline bool OmCharIsLetter( char c )
{
    return OmLogOr(
        OmCharIsUppercaseLetter( c ),
        OmCharIsLowercaseLetter( c ) );
}

inline bool OmCharIsNumber( char c )
{
    bool isNumber = true;
    
    OmAssignAnd( & isNumber, ( c >= '0' ) );
    OmAssignAnd( & isNumber, ( c <= '9' ) );
    
    return isNumber;
}

inline bool OmCharIsNumber( OmUnicode16Char c )
{
    bool isNumber = true;
    
    OmAssignAnd( & isNumber, ( c >= '0' ) );
    OmAssignAnd( & isNumber, ( c <= '9' ) );
    
    return isNumber;
}

inline bool OmCharIsUppercaseLetter( char c )
{
    bool isUppercaseLetter = true;
    
    OmAssignAnd( & isUppercaseLetter, ( c >= 'A' ) );
    OmAssignAnd( & isUppercaseLetter, ( c <= 'Z' ) );

    return isUppercaseLetter;
}

inline bool OmCharIsLowercaseLetter( char c )
{
    bool isLowercaseLetter = true;
    
    OmAssignAnd( & isLowercaseLetter, ( c >= 'a' ) );
    OmAssignAnd( & isLowercaseLetter, ( c <= 'z' ) );
    
    return isLowercaseLetter;
}

inline bool OmCharIsUnderscore( char c )
{
    return OmEqual( c, '_' );
}

inline bool OmCharIsNull( char c )
{
    return OmEqual( c, '\0' );
}


inline bool OmCharIsWhite( char c )
{
    return c <= 32;
}

inline bool OmCharIsParenthesis( char c )
{
    if (
        OmLogOr(
            OmEqual( '(', c ),
            OmEqual( ')', c )  ) )
    {
        return true;
    }
    
    return false;
}

inline char OmCharToLowercase( char c )
{
    if ( OmCharIsUppercaseLetter( c ) )
    {
        c = 'a' + ( c - 'A' );
        
        return c;
    }
    
    return c;
}

inline uint8_t OmCharToUInt8( OmUnicode16Char c )
{
    ASSERT( OmCharIsNumber( c ) );
    
    return static_cast< uint8_t >( c - '0' );
}

inline uint8_t OmCharToUInt32( OmUnicode16Char c )
{
    ASSERT( OmCharIsNumber( c ) );
    
    return static_cast< uint8_t >( c - '0' );
}

template< typename CHAR_TYPE, typename VALUE_TYPE >
inline bool OmCharToHexValue(
    CHAR_TYPE c ,
    VALUE_TYPE* pValue )
{
    if ( ( c >= '0' ) && ( c <= '9' ) )
    {
        *pValue = ( c - '0' );
        return true;
    }
    
    if ( ( c >= 'A' ) && ( c <= 'F' ) )
    {
        *pValue = 0xA + (c - 'A');
        return true;
    }
    
    if ( c >= 'a' && ( c <= 'f') )
    {
        *pValue = 0xA + ( c - 'a' );
        return true;
    }
    
    return false;
}


