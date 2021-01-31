#pragma once

#include "core/Arg.hpp"

#define PROPERTY_GET( TYPE, NAME ) \
    public: \
        inline Arg< TYPE >::Type get##NAME( void ) const { return m##NAME; } \
        inline TYPE& get##NAME( void ) { return m##NAME; } \
    private: \
        TYPE m##NAME;

#define PROPERTY( TYPE, NAME ) \
    public: \
        inline Arg< TYPE >::Type get##NAME( void ) const { return m##NAME; } \
        inline void set##NAME( Arg< TYPE >::Type v ) { m##NAME = v; } \
    private: \
        TYPE m##NAME;
        
#define PROPERTY_T( TYPE, NAME ) \
    public: \
        inline typename Arg< TYPE >::Type get##NAME( void ) const { return m##NAME; } \
        inline TYPE& get##NAME( void ) { return m##NAME; } \
        inline void set##NAME( typename Arg< TYPE >::Type v ) { m##NAME = v; } \
    private: \
        TYPE m##NAME;
