#include "core/CString.hpp"

#include "core/debug.hpp"
#include "core/Type.hpp"
#include "core/Util.hpp"

const OmChar* OM_EMPTY_C_STRING = "";

static void ToBinaryCString( OmUInt8 b, char buf[ 9 ] )
{

    OmLoopI( 8 )
    {
        buf[ ( 7 - i ) ] = ( b & ( 1 << i ) ) == 0 ? '0' : '1';
    }
    
    buf [ 8 ] = '\0';
}

void OmCStringDumpHex( const OmChar* pSIn )
{
    const OmUInt NUM_COLUMNS = 4;
    
    const OmUInt8* pS = reinterpret_cast< const OmUInt8* >( pSIn );
    
    if ( OmIsNull( pS ) )
    {
        Log( "NULL" );
    }
    
    OmBool first = OmTrue;
    
    OmUInt columnCount = 0;
    
    while( '\0' != *pS )
    {
        if ( ! first )
        {
            Log( " " );
        }
        else
        {
            first = OmFalse;
        }
        
        OmChar buf[ 256 ];
        
        OmChar binBuf[ 9 ];
        ToBinaryCString( *pS, binBuf );
        
        snprintf( buf, sizeof( buf ), "%c[0x%X](%s)", (*pS),*pS, binBuf );
        
        LogFmt( "%s", buf );
        
        pS++;
        
        columnCount++;
        
        if ( columnCount >= NUM_COLUMNS )
        {
            first = OmTrue;
            columnCount = 0;
            Log( "\n" );
        }
    }
}

