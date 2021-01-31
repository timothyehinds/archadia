#pragma once
#if 0

template< typename STREAM_TYPE >
class OmUtf8Decoder
{
    public:
    
        typedef STREAM_TYPE StreamType;
        typedef OmUnicode16Char CharType;
        static const int UTF8_ERROR = -2;
        static const int UTF8_END = -1;
   
        OmUtf8Decoder( void );
        OmUtf8Decoder( const OmUtf8Decoder& rhs );
        OmUtf8Decoder( STREAM_TYPE stream );
        OmUtf8Decoder& operator=( const OmUtf8Decoder& rhs );
        
        inline OmDecoderResult NextChar( CharType* pChar );

    private:
    
        inline int readNextByteFromStream( void );
        inline int readNextPayloadByteFromStream( void );
        const int decodeNext( void );
        const int decodeMultibyte( int c );
    
    
    STREAM_TYPE mStream;
};

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------

template< typename STREAM_TYPE >
inline OmUtf8Decoder< STREAM_TYPE >::OmUtf8Decoder( void )
{
}


template< typename STREAM_TYPE >
inline OmUtf8Decoder< STREAM_TYPE >::OmUtf8Decoder( const OmUtf8Decoder& rhs )
    :
    mStream( rhs.mStream )
{
}
    
template< typename STREAM_TYPE >
inline OmUtf8Decoder< STREAM_TYPE >::OmUtf8Decoder( STREAM_TYPE stream )
    :
    mStream( stream )
{
}

template< typename STREAM_TYPE >
inline OmUtf8Decoder< STREAM_TYPE >& OmUtf8Decoder< STREAM_TYPE >::operator=( const OmUtf8Decoder& rhs )
{
    mStream = rhs.mStream;
    
    return *this;
}

template< typename STREAM_TYPE >
inline OmDecoderResult OmUtf8Decoder< STREAM_TYPE >::NextChar( CharType* pChar )
{
    int n = decodeNext();
    
    if ( n < 0 )
    {
        if ( n == UTF8_ERROR )
        {
            return OmDecoderResult_Error;
        }
        else
        {
            ASSERT( n == UTF8_END );
            return OmDecoderResult_Eof;
        }
    }
    
    *pChar = (CharType) n;
    
    return OmDecoderResult_Ok;
}

template< typename STREAM_TYPE >
inline int OmUtf8Decoder< STREAM_TYPE >::readNextByteFromStream( void )
{
    typename STREAM_TYPE::CharType c = 0;
    
    OmDecoderResult r = mStream.NextChar( & c );
    
    if ( OmDecoderResult_Eof == r )
    {
        return UTF8_END;
    }
    else if ( OmDecoderResult_Error == r )
    {
        return UTF8_ERROR;
    }

    return c;
}

template< typename STREAM_TYPE >
inline int OmUtf8Decoder< STREAM_TYPE >::readNextPayloadByteFromStream( void )
{
    int c = readNextByteFromStream();
    
    if ((c & 0xC0) == 0x80)
    {
        return (c & 0x3F);
    }
    
    return UTF8_ERROR;
}

template< typename STREAM_TYPE >
const int OmUtf8Decoder< STREAM_TYPE >::decodeNext( )
{
    int c = readNextByteFromStream();

    // Zero continuation (0 to 127)

    if ((c & 0x80) == 0)
    {
        return c;
    }
    
    return decodeMultibyte( c );
}

template< typename STREAM_TYPE >
const int OmUtf8Decoder< STREAM_TYPE >::decodeMultibyte( int c )
{
    
    if ((c & 0xE0) == 0xC0)
    {
        // One continuation (128 to 2047)

        int c1 = readNextPayloadByteFromStream();
        
        if (c1 >= 0)
        {
            int r = ((c & 0x1F) << 6) | c1;
            
            return r >= 128 ? r : UTF8_ERROR;
        }
    }
    else if ((c & 0xF0) == 0xE0)
    {
        // Two continuation (2048 to 55295 and 57344 to 65535)

        int c1 = readNextPayloadByteFromStream();
        int c2 = readNextPayloadByteFromStream();
        
        if ((c1 | c2) >= 0)
        {
            int r = ((c & 0x0F) << 12) | (c1 << 6) | c2;
            return r >= 2048 && (r < 55296 || r > 57343) ? r : UTF8_ERROR;
        }
    }
    else if ((c & 0xF8) == 0xF0)
    {
        // Three continuation (65536 to 1114111)

        int c1 = readNextPayloadByteFromStream();
        int c2 = readNextPayloadByteFromStream();
        int c3 = readNextPayloadByteFromStream();
        
        if ((c1 | c2 | c3) >= 0)
        {
            return (((c & 0x0F) << 18) | (c1 << 12) | (c2 << 6) | c3) + 65536;
        }
    }
    return UTF8_ERROR;
}

#endif
