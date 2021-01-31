#pragma once

#if 0
#include "core/json/Json.hpp"
#include "core/json/Parse.hpp"
#include "core/Type.hpp"
#include "core/Result.hpp"

//-----------------------------------------------------------------------------
// class class JsonParser
//-----------------------------------------------------------------------------

Result JsonLoadFromFile( Json& json, const char* filename );

template< typename PARSER_T >
class JsonParser
{
    public:
    
        typedef typename PARSER_T::CharType CharType;

        JsonParser( PARSER_T* pParser );

        const bool Parse( Json* pJson );

        const std::string GetError( void );

    private:
    
        const bool ParseValue( JsonBase::Ref* pJson );
    
        const bool ParseObject( JsonBase::Ref* pJson );
        const bool ParseArray( JsonBase::Ref* pJson );
        const bool ParseString( JsonBase::Ref* pJson );
        const bool ParseNumber( JsonBase::Ref* pJson );
    
        const bool ParseUnicodeSequence( CharType* pRes );
    
        const bool EofError( void );
        const bool MakeError( Arg< std::string >::Type errorMsg );

        std::string mError;
        PARSER_T& mParser;
        std::vector< Utf8Char > mWorkArea;
};

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------

template< typename PARSER_T >
inline JsonParser< PARSER_T>::JsonParser( PARSER_T* pParser )
    :
    mParser( *pParser )
{
}

template< typename PARSER_T >
inline const std::string JsonParser< PARSER_T>::GetError( void )
{
    return mError;
}

template< typename PARSER_T >
inline const bool JsonParser< PARSER_T >::Parse( Json* pJson )
{
    JsonBase::Ref refJson;
    
    if ( ParseValue( & refJson ) )
    {
        *pJson = Json( refJson );

        OmParseEatWhite(&mParser);

        if ( !mParser.Eof() ) {
            mError = "Extra cruft after item.";
            return OmFalse;
        }

        return OmTrue;
    }

    return OmFalse;
}

template< typename PARSER_T >
const bool JsonParser< PARSER_T >::ParseValue( JsonBase::Ref* pJson )
{
    OmParseEatWhite( & mParser );
    
    if ( OmParseMatch( & mParser, '{' ) )
    {
        return ParseObject( pJson );
    }
    else if ( OmParseMatch( & mParser, '[' ) )
    {
        return ParseArray( pJson );
    }
    else if ( OmParseMatch( & mParser, '\"' ) )
    {
        return ParseString( pJson );
    }
    else if ( OmParseMatch( & mParser, "null" ) )
    {
        *pJson = new JsonNull();
        return OmTrue;
    }
    else if ( OmParseMatch( & mParser, "true" ) )
    {
        *pJson = new JsonBoolean( OmTrue );
        return OmTrue;
    }
    else if ( OmParseMatch( & mParser, "false" ) )
    {
        *pJson = new JsonBoolean( OmFalse );
        return OmTrue;
    }
    else
    {
        return ParseNumber( pJson );
    }
    
    mError = OmSl( "Expected '{' or '[' or '\"' or number" );
    
    return OmFalse;
}

template< typename PARSER_T >
const bool JsonParser< PARSER_T >::ParseArray( JsonBase::Ref* pJson )
{
    OmRef< JsonArray > refArray( new JsonArray() );
    
    while( OmTrue )
    {
        JsonBase::Ref refJson;
        
        if ( ! ParseValue( & refJson ) )
        {
            mError = OmSl( "Failed to parse array item, " ) + mError;
            return OmFalse;
        }
        
        refArray->push_back( refJson );
        
        OmParseEatWhite( & mParser );
        
        if ( ! OmParseMatch( & mParser, ',' ) )
        {
            if ( ! OmParseMatch( & mParser, ']' ) )
            {
                mError = OmSl( "Expected ']'" );
                
                return OmFalse;
            }
            else
            {
                break;
            }
        }
    }
    *pJson = refArray;
    
    return OmTrue;
        
}

template< typename PARSER_T >
const bool JsonParser< PARSER_T >::ParseObject( JsonBase::Ref* pJson )
{
    OmRef< JsonObject > refObject( new JsonObject() );
    
    while( OmTrue )
    {
        OmParseEatWhite( & mParser );
        
        CharType c;
        
        if ( ! mParser.Peek( & c ) )
        {
            return EofError();
        }
        
        if ( c == '}' )
        {
            mParser.Next();
            break;
        }
        
        if ( c != '\"' )
        {
            return MakeError( OmSl( "Expecting string for object key" ) );
        }
        
        mParser.Next();
        
        JsonBase::Ref refKey;
        
        if ( ! ParseString( & refKey ) )
        {
            return MakeError( OmSl( "Expecting string for object key" ) );
        }

        OmParseEatWhite( & mParser );
        
        if ( ! OmParseMatch( & mParser, ':' ) )
        {
            mError = OmSl( "Expected ':'" );
            
            return OmFalse;
        }
        
        OmParseEatWhite( & mParser );
        
        JsonBase::Ref refValue;
        
        if ( ! ParseValue( & refValue ) )
        {
            mError = OmSl( "Expecting object value, " ) + mError;
            
            return OmFalse;
        }
        
        
        refObject->SetValueForKey( refKey->GetString(), refValue );
        
        OmParseEatWhite( & mParser );
        
        if ( ! OmParseMatch( & mParser, ',' ) )
        {
            if ( ! OmParseMatch( & mParser, '}' ) )
            {
                return MakeError( OmSl( "Expecting ',' or '}' to close object." ) );
            }
            else
            {
                break;
            }
        }
    }
    
    (*pJson) = refObject;
    
    return OmTrue;
}

template< typename PARSER_T >
const bool JsonParser< PARSER_T >::ParseUnicodeSequence( CharType* pRes )
{
    CharType value = 0;
    
    OmLoopI( 4 )
    {
        CharType c;
        
        if ( ! mParser.Peek( & c ) )
        {
            return MakeError( OmSl( "EOF parsing string \\u unicode sequence" ) );
        }
        
        CharType hexDigit;
    
        if ( ( c >= '0' ) && ( c <= '9' ) )
        {
            hexDigit = ( c  - '0' );
        }
        else if ( c >= 'a' && ( c <= 'f' ) )
        {
            hexDigit = 0xA + ( c - 'a' );
        }
        else if ( c >= 'A' && ( c <= 'F' ) )
        {
            hexDigit = 0xA + ( c - 'A' );
        }
        else
        {
            return MakeError( OmSl( "Not a hex digit in \\u unicode sequence" ) );
        }

        value |= ( hexDigit << ( 4 * ( 3 - i ) ) );
    
        mParser.Next();
    }
    
    *pRes = value;
    
    return OmTrue;
}

template< typename PARSER_T >
const bool JsonParser< PARSER_T >::ParseString( JsonBase::Ref* pJson )
{
    bool escape = OmFalse;
    
    typename PARSER_T::WorkAreaType* pWorkArea = mParser.ClearWorkArea();
    
    while( 1 )
    {
        CharType c;
        
        if ( ! mParser.Peek( & c ) )
        {
            mError = OmSl( "Unexpected end of file" );
            
            return OmFalse;
        }
        
        if ( escape )
        {
            switch( c )
            {
                case '\"':
                case '\\':
                case '/':
                    break;
                case 'b': c = '\b'; break;
                case 'f': c = '\f'; break;
                case 'n': c = '\n'; break;
                case 'r': c = '\r'; break;
                case 't': c = '\t'; break;
                    pWorkArea->push_back( c );
                    break;
                case 'u':
                    mParser.Next();
                
                    if ( ! ParseUnicodeSequence( & c ) )
                    {
                        return OmFalse;
                    }
                    pWorkArea->push_back( c );
                    escape = OmFalse;
                    continue;
                    break;
                    
                default:
                    mError = OmFs( "Unrecognized escape charater: %C", c );
                    return OmFalse;
            }
            
            escape = OmFalse;
        }
        else if ( c == '\\' )
        {
            escape = true;
            mParser.Next();
            continue;
        }
        else if ( c == '\"' )
        {
            mParser.Next();
            break;
        }
        
        pWorkArea->push_back( c );
        mParser.Next();
    }
    
    *pJson = new JsonString( mParser.StringFromWorkArea() );
    
    return OmTrue;

}

template< typename PARSER_T >
const bool JsonParser< PARSER_T >::EofError( )
{
    mError = OmSl( "Unexpected EOF" );
    return OmFalse;
}

template< typename PARSER_T >
const bool JsonParser< PARSER_T >::MakeError( Arg< std::string >::Type errorMsg )
{
    mError =
      errorMsg + OmFs( " Line: %d Column: %d", mParser.GetLineCount(), mParser.GetColumnCount() );
    
    return OmFalse;
}

template< typename PARSER_T >
const bool JsonParser< PARSER_T >::ParseNumber( JsonBase::Ref* pRefJson )
{
    OmInt64 negativeMultiplier = 1;
    
    CharType c;

    if ( ! mParser.Peek( & c ) )
    {
        return OmFalse;
    }
    
    if ( '-' == c )
    {
        negativeMultiplier = -1;
        
        if ( ! mParser.Adv( & c ) )
        {
            return OmFalse;
        }
        
        if ( ! OmCharIsNumber( c ) )
        {
            return MakeError( OmSl( "No digits after '-'" ) );
        }
    }
    
    unsigned long long mantissa = 0;
    
    if ( '0' == c )
    {
        if ( ! mParser.Adv( & c ) )
        {
            return EofError();
        }
        
        if ( OmCharIsNumber( c ) )
        {
            return MakeError( OmSl( "Leading zero is invalid" ) );
        }
    }
    else
    {
        while ( OmCharIsNumber( c ) )
        {
            mantissa *= 10;
            mantissa += ( c - '0' );
            if ( ! mParser.Adv( & c ) )
            {
                return EofError();
            }
        }
    }
    
    OmInt64 exponent = 0;
    bool isFloat = OmFalse;
    
    if ( c == '.' )
    {
        isFloat = OmTrue;
        
        if ( ! mParser.Adv( & c ) )
        {
            return EofError();
        }
        
        if ( ! OmCharIsNumber( c ) )
        {
            return MakeError( OmSl( "No digits after decimal point" ) );
        }
        
        do
        {
            mantissa *= 10;
            mantissa += (c - '0' );
            exponent--;
            
            if ( ! mParser.Adv( & c ) )
            {
                return EofError();
            }
            
        } while( OmCharIsNumber( c ) );
    }
    
    //bool hasExponent = false;

    if ( c == 'e' || c == 'E' )
    {
        // hasExponent = true;

        if ( ! mParser.Adv( & c ) )
        {
            return EofError();
        }

        bool expIsNegative = OmFalse;
        
        if ( c == '-' )
        {
            expIsNegative = OmTrue;
            
            if ( ! mParser.Adv( & c ) )
            {
                return EofError();
            }
        }
        else if ( c == '+' )
        {
            if ( ! mParser.Adv( & c ) )
            {
                return EofError();
            }
        }

        short explicit_exponent = 0;
        
        if ( ! OmCharIsNumber( c ) )
        {
            return MakeError( OmSl( "No digits in exponent" ) );
        }
        
        do
        {
            explicit_exponent *= 10;
            explicit_exponent += ( c - '0' );

            if ( ! mParser.Adv( & c ) )
            {
                return EofError();
            }
            
        } while( OmCharIsNumber( c ) );


        if ( expIsNegative )
        {
            exponent -= explicit_exponent;
        }
        else
        {
            exponent += explicit_exponent;
        }
    }

    if ( ! isFloat )
    {
        OmInt64  v = negativeMultiplier * mantissa;
        
        if ( exponent > 0 )
        {
            OmLoopI( exponent )
            {
                v *= 10;
            }
        }
        else if ( exponent < 0 )
        {
            OmLoopI( -exponent )
            {
                v /= 10;
            }
        }
        
        *pRefJson = new JsonInteger( v );
    }
    else
    {
        OmFloat64 m = OmPow< OmFloat64 >( 10.0, exponent );
        
        OmFloat64 d = negativeMultiplier * ( mantissa * m );
        
        *pRefJson = new JsonReal( d );
    }

    //OM_LOG( "%s\n", (*pRefJson)->Dump( 0 ).GetUtf8() );
    
    return OmTrue;
}

#endif
