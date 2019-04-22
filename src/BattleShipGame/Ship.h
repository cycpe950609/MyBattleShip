#pragma once

#include <vector>

namespace TA
{
    enum class ShipState
    {
        Available,
        Hit,
        Sink
    };

    struct Ship
    {
        /* 
            it mean Ship hold those position b[i][j]
            for i = x, x+1, ..., x+size-1
            for j = y, y+1, ..., y+size-1
        */

        std::size_t size;
        std::size_t x;
        std::size_t y;

        ShipState state;
    };
}

