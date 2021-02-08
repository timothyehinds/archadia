#pragma once

#include "core/parse/Decoder.hpp"
#include "core/Type.hpp"


template< typename StreamType >
class OmParser
{
    public:
    
        using PositionType = size_t; 
        using CharType = typename StreamType::CharType;
        using WorkAreaType = std::vector< CharType >;

        OmParser(
            typename Arg< StreamType >::Type stream,
            std::vector< CharType >& wWorkArea,
            CStr fileName );

        bool Eof( void ) const;
    
        const CharType Peek( void );
        bool Peek( CharType* pChar );

        const bool Next( CharType* pCurrentChar );
        const bool Next( void );

        const bool Adv( CharType* pChar );
    
        std::vector< CharType >* ClearWorkArea( void );
        const std::string StringFromWorkArea( void );
    
        const std::string GetErrorHint( void );
    
        CStr GetFilename( void ) const;
        const size_t GetLineCount( void ) const;
        const size_t GetColumnCount( void ) const;

    private:

        void AdvanceLineColumnCount( void );
    
        StreamType mStream;
        CharType mCurrentChar;
        bool mEof;
        
        size_t mLineCount;
        size_t mColumnCount;

        CStr mFileName;
    
        std::vector< CharType >* mpWorkArea;
};

/* template< typename StreamType>
OmParser< StreamType>::OmParser( )
    :
    mEof( true ),
    mCurrentChar(0),
    mLineCount( 0 ),
    mColumnCount( 0 ),
    mpFilename(nullptr),
    mpWorkArea(nullptr)
{
} */

template< typename StreamType>
OmParser< StreamType>::OmParser(
    typename Arg< StreamType >::Type stream,
    std::vector< CharType >& workArea,
    CStr fileName )
    :
    mStream( stream ),
    mCurrentChar(0),
    mEof( OmDecoderResult_Ok != mStream.NextChar( & mCurrentChar ) ),
    mLineCount( 1 ),
    mColumnCount( 1 ),
    mFileName(fileName),   
    mpWorkArea(&workArea )    
{
}

template< typename StreamType>
const typename OmParser< StreamType >::CharType
OmParser< StreamType >::Peek( void )
{
    OM_ASSERT( ! Eof() );
    
    return mCurrentChar;
}

template< typename StreamType>
bool OmParser< StreamType>::Peek( CharType* pChar )
{
    if ( Eof() )
    {
        return false;
    }
    
    *pChar = mCurrentChar;
    
    return true;
}

template< typename StreamType>
const bool OmParser< StreamType>::Next( CharType* pCurrentChar )
{
    if ( Eof() )
    {
        return false;
    }
    (*pCurrentChar) = mCurrentChar;
    
    Next();
    
    return true;
}

template< typename StreamType>
const bool OmParser< StreamType>::Next( void )
{
    if ( OmDecoderResult_Ok != mStream.NextChar( & mCurrentChar ) )
    {
        // TODO: Handle Error
        mCurrentChar = 0;
        mEof = true;
        return false;
    }

    AdvanceLineColumnCount();
    
    return true;
}

template< typename StreamType>
inline const bool OmParser< StreamType>::Adv( CharType* pChar )
{
    Next();
    
    return Peek( pChar );
}

template< typename StreamType>
inline bool OmParser< StreamType>::Eof( void ) const
{
    return mEof;
}

template< typename StreamType>
inline typename OmParser< StreamType >::WorkAreaType*
OmParser< StreamType>::ClearWorkArea( void )
{
    mpWorkArea->clear();
    return mpWorkArea;
}

template< typename StreamType>
inline const std::string OmParser< StreamType>::StringFromWorkArea( void )
{
    return std::string( mpWorkArea->data(), mpWorkArea->size() );
}

template< typename StreamType>
const std::string OmParser< StreamType>::GetErrorHint( void )
{
    std::vector< CharType > err;
    err.reserve(80);
    
    CharType c;
    
    for(size_t i = 0; i < 80; i ++)
    {
        if ( ! Peek( &c ) )
        {
            break;
        }
        
        err.push_back( c );
        
        Next();
        
    }
    
    return OmSl( "Before:" ) + std::string( err.data(), err.size() );
}

template< typename StreamType>
CStr OmParser< StreamType>::GetFilename( void ) const
{
    return mFileName;
}

template< typename StreamType>
const size_t OmParser< StreamType>::GetLineCount( void ) const
{
    return mLineCount;
}

template< typename StreamType>
const size_t OmParser< StreamType>::GetColumnCount( void ) const
{
    return mColumnCount;
}

template< typename StreamType>
void OmParser< StreamType>::AdvanceLineColumnCount( void )
{
    if ( '\n' == mCurrentChar )
    {
        mLineCount++;
        mColumnCount = 1;
    }
    else
    {
        mColumnCount++;
    }
}
