#pragma once
#include "core/Arg.hpp"
#include "core/Type.hpp"
#include <cmath>
#include <cstring>

struct NULLTYPE { };
typedef int OmNullType;

#define OmLoop( TIMES, COUNTER ) for( OmUInt COUNTER = 0; COUNTER < TIMES; COUNTER ++ )
#define OmLoopI( TIMES ) for ( OmUInt i = 0; i < TIMES; i ++ )
#define OmLoopInf while( OmTrue )
const size_t OmNullIndex = static_cast< size_t >( -1 );

template< typename T1, typename T2, typename T3 >
T1 OmClamp( const T1& v, const T2& min, const T3& max )
{
    if ( v < min )
    {
        return min;
    }
    else if ( v > max )
    {
        return max;
    }
    
    return v;
}

template< typename T1, typename T2 >
T1 OmMax( const T1& t1, const T2& t2 )
{
    if ( t1 > t2 )
    {
        return t1;
    }
    
    return t2;
}

template< typename T  >
T OmMin( const T& t1, const T& t2 )
{
    if ( t1 < t2 )
    {
        return t1;
    }
    
    return t2;
}

template< typename T >
T OmMin( const  T& x1, const  T& x2, const  T& x3 )
{
    return OmMin( OmMin( x1, x2 ), x3 );
    
}

template< typename T >
T OmPow( T b, T e )
{
    T res = 1;
    
    if ( e >= 0 )
    {    
        OmLoopI( e )
        {
            res *= b;
        }
    }
    else
    {
        OmLoopI( OmAbs( e ) )
        {
            res /= b;
        }
    }
    
    return res;
}

template<>
inline OmFloat64 OmPow< OmFloat64 >( OmFloat64 x, OmFloat64 e )
{
    return pow( x, e );
}

template<>
inline OmFloat32 OmPow< OmFloat32 >( OmFloat32 x, OmFloat32 e )
{
    return powf( x, e );
}

inline OmFloat32 OmCos( OmFloat32 x )
{
    return cosf( x );
}

template< typename T >
const T OmAbs( typename Arg< T >::Type t )
{
    if ( t < 0 )
    {
        return -t;
    }
    
    return t;
}

template< typename T >
T OmSqr( typename Arg< T >::Type x )
{
    return x * x;
}

template< typename T >
void OmSwap( T* p1, T* p2 )
{
    T temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

template < typename T >
inline OmBool OmIsValid( T t )
{
    return ( 0 != t );
}

template< typename T >
inline OmBool OmIsValid( T* pT )
{
    return 0 != pT;
}

template< typename T >
inline OmBool OmSetIfValid( T* pT, typename Arg< T >::Type  value )
{
    if ( OmIsValid( pT ) )
    {
        *pT = value;
        return OmTrue;
    }
    
    return OmFalse;
}

template< typename T >
inline OmBool OmIsNull( T t )
{
    return ( 0 == t );
}

inline void OmMemCopy( void* pTarg, const void* pSource, OmUInt numBytes )
{
    memcpy( pTarg, pSource, numBytes );
}

inline void OmClearMem( void* pMem, OmUInt bytes )
{
    OmChar* pBytes = reinterpret_cast< OmChar* > ( pMem );

    OmLoopI( bytes )
    {
        pBytes[ i ] = 0;
    } 
}

inline OmBool
OmMemoryCompare(
    const void* pM1,
    const void* pM2,
    OmUInt numBytes )
{
    return ( 0 == memcmp( pM1, pM2, numBytes ) );
}

template< typename T1, typename T2 >
void OmBitSet( T1* pV, T2 bits )
{
    (*pV) = (*pV) | bits;
}

template< typename T1, typename T2 >
void OmClearBit( T1* pV, T2 bit )
{
    (*pV) = (*pV) & ~(1 << bit );
}

template< typename T1, typename T2 >
OmBool OmBitIsSet( T1 a, T2 b )
{
    return ( ( a & b ) > 0 );
}

template< typename T >
inline T OmRoundUpPow2( T n )
{
    if ( 0 == n )
    {
        return n;
    }
    
    T x = 1;

    while(x < n)
    {
        x <<= 1;
    }

    return x;
}

inline OmUInt32 OmRoundUpPow2( OmUInt32 x )
{
    x--;
    x |= x >> 1;  // handle  2 bit numbers
    x |= x >> 2;  // handle  4 bit numbers
    x |= x >> 4;  // handle  8 bit numbers
    x |= x >> 8;  // handle 16 bit numbers
    x |= x >> 16; // handle 32 bit numbers
    x++;
    
    return x;
}

inline OmUInt64 OmRoundUpPow2( OmUInt64 x )
{
    x--;
    x |= x >> 1;  // handle  2 bit numbers
    x |= x >> 2;  // handle  4 bit numbers
    x |= x >> 4;  // handle  8 bit numbers
    x |= x >> 8;  // handle 16 bit numbers
    x |= x >> 16; // handle 32 bit numbers
    x |= x >> 32; // handle 64 bit numbers
    x++;
    
    return x;
}

template< typename T >
inline T OmRoundNumberToMultiple( const T& number, const T& multiple )
{
    T remainder = number % multiple;
    
    if ( remainder == 0 )
    {
        return number;
    }
    
    return ( ( multiple - remainder ) + number );
}

inline OmFloat32 OmFMod( const OmFloat32 & x, const OmFloat32& y )
{
    return fmodf( x, y );
}

inline OmFloat64 OmFMod( const OmFloat64 & x, const OmFloat64& y )
{
    return fmod( x, y );
}

#define OmEqual( x, y ) ( (x) == (y) )

inline void OmAssignAnd( OmBool* pBool, OmBool x )
{
    (*pBool ) = (*pBool) & x;
}

inline OmBool OmLogAnd( OmBool x1, OmBool y1 )
{
    return ( x1 && y1 );
    
}

inline OmBool OmLogAnd( OmBool x1, OmBool x2, OmBool x3 )
{
    return ( x1 && x2 && x3 );
}

inline OmBool OmLogOr( OmBool x1, OmBool x2 )
{
    return ( x1 || x2 );
}

inline OmBool OmLogOr( OmBool x1, OmBool x2, OmBool x3 )
{
    return ( x1 || x2 || x3 );
}

inline OmBool OmLogNot( OmBool x1 )
{
    return ! x1;
}

inline OmFloat OmCeil( OmFloat f )
{
    return (OmFloat) ceil( f );
}


#define OmSub( x, y ) ((x) - (y))
#define OmNotEqual( x, y ) ((x) != (y))

#define OmGt( x, y ) ((x) > (y))
#define OmLt( x, y ) ((x) < (y))
#define OmLtEq( x, y ) ((x) <= (y))

#define OmCond( x, y, z ) ( ( x ) ? ( y ) : ( z ) )

#define OmUpCast static_cast

#define OmSPrintf sprintf

#define OmDeRef( x ) (*(x))


inline OmFloat64 OmDegToRad( OmFloat64 deg )
{
    return deg * ( M_PI / 180 );
}

inline OmFloat64 OmRadToDeg( OmFloat64 rad )
{
    return rad * ( 180 / M_PI );
}

template<typename T>
inline bool oneOf(const T& item, const std::initializer_list<T>& items)
{
    for(auto&& i: items) {
        if (item == i) {
            return true;
        }
    }

    return false;
}



template< typename T >
inline const T OmDiv( typename Arg< T >::Type t1, typename Arg< T >::Type t2 )
{
    return t1 / t2;
}

#define OmDeleteSetNull( pX ) { delete *(pX); *(pX) = 0; }
