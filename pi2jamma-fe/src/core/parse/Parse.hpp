#pragma once

#include "core/Char.hpp"

template< typename CharType, typename StringType >
OmBool OmParseCharMatchesOneOf( const CharType c, const StringType* pChars )
{
    for( ; *pChars; pChars++ )
    {
        if ( (*pChars) == c )
        {
            return OmTrue;
        }
    }
    
    return OmFalse;
}

template< typename PARSER_T, typename STRING_TYPE >
OmBool OmParseMatchCaseInsensitive(
    PARSER_T* pParser,
    const STRING_TYPE* pString )
{
    typedef typename PARSER_T::CharType CharType;
    
    PARSER_T p( *pParser );
    
    while ( *pString )
    {
        CharType c1;
        
        if ( ! p.Next( & c1 ) )
        {
            return OmFalse;
        }
        
        c1 = OmCharToLowercase( static_cast< OmChar >( c1 ) );
        
        CharType c2 = OmCharToLowercase( *pString );
        
        if ( c1 != c2 )
        {
            return OmFalse;
        }
        
        pString++;
    }
    
    (*pParser) = p;
    
    return OmTrue;
}

template< typename PARSER_TYPE, typename STRING_TYPE >
OmBool OmParseMatchOneOf(
    PARSER_TYPE* pParser,
    const STRING_TYPE* array[] )
{
    const OmChar** ppKnown = array;
    
    while( *ppKnown )
    {
        if ( OmParseMatch( pParser, *ppKnown ) )
        {
            return OmTrue;
        }
        
        ppKnown++;
    }
    
    return OmFalse;
}


template< typename PARSER_T >
OmBool OmParseTokenWithCharDelimiters(
    PARSER_T* pParser,
    OmString* pString,
    const OmChar* invalidChars )
{
    typename PARSER_T::WorkAreaType* pWorkArea = pParser->ClearWorkArea();
    
    typename PARSER_T::CharType c;
    
    while( pParser->Peek( & c ) )
    {
        if ( OmParseCharMatchesOneOf( c, invalidChars ) )
        {
            break;
        }
        
        pWorkArea->PushBack( c );
        
        pParser->Next();
    }
    
    if ( pWorkArea->GetCount() > 0 )
    {
        (*pString) = pParser->StringFromWorkArea();
        return OmTrue;
    }

    return OmFalse;
}

template< typename PARSER_T, typename STRING_TYPE >
OmBool OmParseSkipMatchingChars(
    PARSER_T* pParser,
    const STRING_TYPE* pChars )
{
    typename PARSER_T::CharType c;

    OmBool skippedOne = OmFalse;
    
    OmLoopInf
    {
        if ( ! pParser->Peek( & c ) )
        {
            break;
        }
        
        if ( ! OmParseCharMatchesOneOf( c, pChars ) )
        {
            break;
        }
        
        
        pParser->Next( );
    }
    
    return skippedOne;
}

template< typename PARSER_T, typename RULE >
OmBool OmParseTokenWithCharRule(
    PARSER_T* pParser,
    RULE rule,
    OmString* pString )
{
    typedef typename PARSER_T::CharType CharType;
    
    typename PARSER_T::WorkAreaType* pWorkArea = pParser->ClearWorkArea();
    
    CharType c;
    
    if ( ! pParser->Peek( & c ) )
    {
        return OmFalse;
    }

    while ( rule( c ) )
    {
        pWorkArea->PushBack( c );
        
        if ( ! pParser->Adv( & c ) )
        {
            return OmFalse;
        }
    }
    
    if ( pWorkArea->GetCount() <= 0 )
    {
        return OmFalse;
    }

    *pString = pParser->StringFromWorkArea();
            
    return OmTrue;
}
template< typename PARSER_TYPE, typename RULE >
OmBool OmParseMatchDelmitedList(
    PARSER_TYPE* pParser,
    RULE rule,
    typename PARSER_TYPE::CharType seperator,
    OmVector< OmString >* pStrings,
    typename PARSER_TYPE::CharType escapeChar = '\0')
{
    PARSER_TYPE p( *pParser );

    typename PARSER_TYPE::WorkAreaType* pWorkArea = p.ClearWorkArea();

    OmBool escape = OmFalse;
    
    while( OmTrue )
    {
        typename PARSER_TYPE::CharType c;
        
        if ( ! p.Peek( & c ) )
        {
            pStrings->push_back( pWorkArea->StringFromWorkArea() );

            *pParser = p;
                    
            return OmTrue;
        }

        if ( escape )
        {
            escape = false;
        }
        else if ( c == escapeChar )
        {
            escape = OmTrue; 
            p.Next();
            continue; 
        }
        else if ( c == seperator )
        {
            pStrings->push_back( pWorkArea->StringFromWorkArea() );
            pWorkArea->Clear();
            p.Next();
            continue;
        }
        else if ( ! rule( c ) )
        {
            pStrings->push_back( pWorkArea->StringFromWorkArea() );

            *pParser = p;
                    
            return OmTrue;
        }
        
        pWorkArea->PushBack( c );
        
        p.Next();
    }
}

template< typename PARSER_T >
inline void OmParseSplit(
    PARSER_T* pParser,
    typename PARSER_T::CharType delimiter,
    OmVector< OmVector< typename PARSER_T::CharType > >* pVector )
{
    typename PARSER_T::WorkAreaType* pWorkArea = pParser->ClearWorkArea();
    
    OmLoopInf
    {
        typename PARSER_T::CharType c;
        
        OmBool eof = ! pParser->Peek( & c );
        
        if ( eof || ( delimiter == c ) )
        {
            pVector->PushBack( *pWorkArea );
            
            if ( eof )
            {
                return;
            }
            
            pWorkArea->Clear();
        }
        else
        {
            pWorkArea->PushBack( c );
        }
        
        pParser->Next();
    }
}

template< typename PARSER_T >
inline void OmParseToDelimiterOrEof(
    PARSER_T* pParser,
    typename PARSER_T::CharType delimiter,
    OmVector< typename PARSER_T::CharType >* pVector )
{
    OmLoopInf
    {
        typename PARSER_T::CharType c;
        
        OmBool eof = ! pParser->Peek( & c );
        
        if ( eof )
        {
            return;
        }
        
        if ( delimiter == c )
        {
            return;
        }
        
        pVector->PushBack( c );
        
        pParser->Next();
    }
}

template< typename PARSER_T >
inline OmBool OmParseTokenWithDelimeter(
    PARSER_T* pParser,
    typename PARSER_T::CharType delimiter,
    OmVector< typename PARSER_T::CharType >* pVector,
    typename PARSER_T::CharType escapeChar )
{
    PARSER_T parser( *pParser );
    
    OmBool escape = OmFalse;
    
    typename PARSER_T::CharType c;
    
    while( parser.Peek( & c ) )
    {
        if ( escape )
        {
            pVector->PushBack( c );
            escape = OmFalse;
        }
        else if ( escapeChar == c )
        {
            escape = OmTrue;
        }
        else if ( delimiter == c )
        {
            parser.Next();
            *pParser = parser;
            return OmTrue;
        }
        else
        {
            pVector->PushBack( c );
        }
        
        parser.Next();
    }
    
    return OmFalse;
}

    

template< typename PARSER_T >
inline OmBool OmParseMatchCVar( PARSER_T* pParser, OmString* pString )
{
    PARSER_T parser( *pParser );
    
    typename PARSER_T::WorkAreaType* pWorkArea = parser.ClearWorkArea();
    
    OmUnicode16Char c = parser.Peek();
    
    OmBool first = OmTrue;
    
    while(
        ( c == '_' ) ||
        ( ( c >= 'a' ) && ( c <= 'z' ) ) ||
        ( ( c >= 'A' ) && ( c <= 'Z' ) ) ||
        ( ( ! first ) && ( c >= '0' ) && ( c <= '9' ) ) )
    {
        pWorkArea->PushBack( c );
        
        parser.Next( );
        
        c = parser.Peek();
        
        first = OmFalse;
    }
    
    if ( pWorkArea->GetCount() <= 0 )
    {
        return OmFalse;
    }
    
    *pString = OmString( pWorkArea->GetRawPtr(), pWorkArea->GetCount() );

    *pParser = parser;
    
    return OmTrue;
}

template< typename PARSER_TYPE >
inline OmBool OmParseMatch( PARSER_TYPE* pParser, Arg< OmString >::Type token )
{
    OmUInt tokenLength = token.size();
    
    PARSER_TYPE parser( *pParser );
    
    OmLoopI( tokenLength )
    {
        typename PARSER_TYPE::CharType c;
        
        if ( ! parser.Peek( & c ) )
        {
            return OmFalse;
        }
        
        if ( c != token[ i ] )
        {
            return OmFalse;
        }
        
        parser.Next();
    }
    
    *pParser = parser;
    
    return OmTrue;
}

template< typename PARSER_T, typename STRING_CHAR_TYPE >
inline OmBool OmParseMatch(
    PARSER_T* pParser,
    const STRING_CHAR_TYPE* pString )
{
    PARSER_T parser( *pParser );
    
    while ( *pString != '\0' )
    {
        typename PARSER_T::CharType c;
        
        if ( ! parser.Peek( & c ) )
        {
            return OmFalse;
        }
        
        typename PARSER_T::CharType test = *pString;
        
        if ( c != test )
        {
            return OmFalse;
        }
        
        pString++;
        
        parser.Next();
    }
    
    *pParser = parser;
    
    return OmTrue;
}

template< typename PARSER_T >
inline OmBool OmParseMatch( PARSER_T* pParser, const OmChar m )
{
    typename PARSER_T::CharType c;
    
    if ( ! pParser->Peek( & c ) )
    {
        return OmFalse;
    }
    
    if ( c == m )
    {
        pParser->Next();
        return OmTrue;
    }
    
    return OmFalse;
}

template< typename PARSER_T >
inline OmBool OmParseMatchCStyleComment( PARSER_T* pParser, OmString* pComment )
{
    PARSER_T parser( *pParser );
    
    typename PARSER_T::WorkAreaType* pWorkArea = pParser->ClearWorkArea();
    
    if ( OmParseMatch( & parser, OmSl( "/*" ) ) )
    {
        while( ! OmParseMatch( & parser, OmSl( "*/" ) ) )
        {
            pWorkArea->PushBack( parser.Peek() );
            
            if ( parser.Eof() )
            {
                return OmFalse;
            }
            
            parser.Next();
        }
        
        *pComment = OmString( pWorkArea->GetRawPtr(), pWorkArea->GetCount() );
        
        (*pParser) = parser;
        return OmTrue;
    }
    
    return OmFalse;
}

template< typename PARSER_T >
inline OmBool OmParseMatchCppStyleComment( PARSER_T* pParser, OmString * pComment )
{
    PARSER_T parser( *pParser );

    typename PARSER_T::WorkAreaType* pWorkArea = parser.ClearWorkArea();
    
    if ( ! OmParseMatch( & parser, OmSl( "//" ) ) )
    {
        return OmFalse;
    }

    while( ! parser.Eof() )
    {
        OmUnicode16Char c = parser.Peek();
        
        if ( c == '\n' )
        {
            *pComment = parser.StringFromWorkArea();
            
            parser.Next();
            break;
        }
        
        pWorkArea->PushBack( c );
        
        parser.Next();
    }
    
    *pParser = parser;
    
    return OmTrue;
}

template< typename PARSER_T >
OmBool OmParseEatWhite( PARSER_T* pParser )
{
    while( ! pParser->Eof() )
    {
        typename PARSER_T::CharType c = pParser->Peek();
        
        if ( c > 32 )
        {
            break;
        }
        
        pParser->Next();
    }
    
    return OmTrue;
}


template< typename PARSER_T >
inline OmBool OmParseMatchQuotedString( PARSER_T* pParser, OmString* pString )
{
    PARSER_T parser( *pParser );

    typename PARSER_T::WorkAreaType* pWorkArea = parser.ClearWorkArea();

    typename PARSER_T::CharType c;
    
    if ( ! parser.Peek( & c ) )
    {
        return OmFalse;
    }
    
    if ( '"' == c )
    {
        OmBool escape = OmFalse;
        
        while( OmTrue )
        {
            if ( ! parser.Adv( & c ) )
            {
                return OmFalse;
            }
            
            if ( escape )
            {
                escape = OmFalse;

                if ( c == 'n' )
                {
                    c = '\n';
                }
            }
            else
            {
                if ( c == '\\' )
                {
                    escape = OmTrue;
                    continue;
                }

                if ( c == '"' )
                {
                    *pString = parser.StringFromWorkArea();
                    
                    parser.Next();
                    
                    (*pParser ) = parser;
                    return OmTrue;
                }
            }

            pWorkArea->PushBack( c );
        }
    }
    
    return OmFalse;
}
