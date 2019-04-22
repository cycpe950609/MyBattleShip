#pragma once

#include <type_traits>
#include <BattleShipGame/Wrapper/AI.h>

using GETAIFUNC   =  void* (*)(void);

extern "C" void* getai();

struct Content
{
    AIInterface *ai;
    GETAIFUNC getai;

    void *handle; // place for dlopen handle
    
    static_assert( std::is_same<GETAIFUNC, decltype(&::getai)>::value );
};
