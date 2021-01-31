#pragma once

#include "core/parse/Decoder.hpp"

class StringDecoder
{
    public:
    
        typedef char CharType;

        StringDecoder(
            const char* pBegin,
            const char* pEnd)
            : mpBegin(pBegin)
            , mpEnd(pEnd)
            , mpCursor(mpBegin)
        {
        }
    
        OmDecoderResult NextChar( CharType* pChar )
        {
            if(mpCursor >= mpEnd)
            {
                return OmDecoderResult_Eof;
            }
            
            *pChar = *mpCursor;
            
            mpCursor++;
            
            return OmDecoderResult_Ok;
        }

        const char* mpBegin;
        const char* mpEnd;
        const char* mpCursor;
};
