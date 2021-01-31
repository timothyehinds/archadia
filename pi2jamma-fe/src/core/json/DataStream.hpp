#pragma once
#if 0
#include "core/json/Decoder.hpp"
//#include "om/OmStream.h"

template< typename ElementType >
class OmDataStream
{
    public:
    
        typedef ElementType CharType;
    
        OmDataStream( void );
        OmDataStream( const OmDataStream& rhs );
        OmDataStream( const ElementType* pElements, OmUInt length );
    
        OmDataStream& operator=( const OmDataStream& );
    
        OmDecoderResult NextChar( ElementType* pElement );
        virtual OmBool GetIsEof( void );
    
    
    private:
    
        OmUInt mPosition;
        const ElementType* mpElements;
        OmUInt mNumElements;
};

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------

template< typename ElementType >
inline OmDataStream< ElementType >::OmDataStream( void )
    :
    mPosition( 0 ),
    mpElements( NULL ),
    mNumElements( 0 )
{
    
}

template< typename ElementType >
inline OmDataStream< ElementType >::OmDataStream( const OmDataStream& rhs )
    :
    mPosition( rhs.mPosition ),
    mpElements( rhs.mpElements ),
    mNumElements( rhs.mNumElements )
{
}

template< typename ElementType >
inline OmDataStream< ElementType >& OmDataStream< ElementType >::operator=( const OmDataStream& rhs )
{
    mPosition = rhs.mPosition;
    mpElements = rhs.mpElements;
    mNumElements = rhs.mNumElements;
    return *this;
}

template< typename ElementType >
inline OmDataStream< ElementType >::OmDataStream( const ElementType* pElements , OmUInt numElements )
    :
    mpElements( pElements ),
    mNumElements( numElements ),
    mPosition( 0 )
{
}

template< typename ElementType >
inline OmDecoderResult OmDataStream< ElementType >::NextChar( ElementType* pElementOut )
{
    if ( mPosition >= mNumElements )
    {
        return OmDecoderResult_Eof;
    }
    
    *pElementOut = mpElements[ mPosition ];
    
    mPosition++;
    
    return OmDecoderResult_Ok;
}


template< typename ElementType >
inline OmBool OmDataStream< ElementType >::GetIsEof( void )
{
    return ( mPosition >= mNumElements );
}

#endif