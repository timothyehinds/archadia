#pragma once

#include "core/Arg.hpp"

template<typename T>
struct LifetimePolicyNone
{
    public:

        static void addRef(const T& t) { }
        static void release(const T& t) { }
};

template<typename T>
struct LifetimePolicyDelete
{
public:
    static void addRef(const T& t) {}
    static void release(const T & t) { delete &t; }
};
