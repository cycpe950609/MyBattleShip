#pragma once

#include "Board.h"

#include <chrono>

namespace TA
{
    class BattleShipGame
    {
    public:
        BattleShipGame(
            std::size_t width,
            std::size_t height,
            std::chrono::milliseconds runtime_limit = std::chrono::milliseconds(0)
        ):
            m_width(width),
            m_height(height),
            m_runtime_limit(runtime_limit)
        {}
    private:
        std::size_t m_width;
        std::size_t m_height;
        std::chrono::milliseconds m_runtime_limit;
    } ;
};
