#pragma once

#include <vector>

namespace TA
{
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

        bool hit;

        bool isInner(int _x, int _y) const {
            return x <= _x && _x < x + size && y <= _y && _y < y + size;
        }
        bool isOutOfBound(int w, int h) const {
            
        }
        bool setNewPosition();
    };

    class ShipHolder
    {
        std::vector<Ship>
    };
}

