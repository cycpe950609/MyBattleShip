#pragma once

#include <type_traits>

using SUMFUNC   = int  (*)(void);


extern "C" int sum();

struct Content
{
    SUMFUNC sum;

    void *handle; // place for dlopen handle

    static_assert( std::is_same<SUMFUNC, decltype(&::sum)>::value );
};
