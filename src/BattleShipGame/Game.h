#pragma once

#include <BattleShipGame/Board.h>
#include <BattleShipGame/Wrapper/AI.h>

#include <iostream>
#include <cassert>
#include <chrono>

namespace TA
{
    class BattleShipGame
    {
    public:
        BattleShipGame(
            std::size_t size,
            std::chrono::milliseconds runtime_limit = std::chrono::milliseconds(0)
        ):
            m_size(size),
            m_runtime_limit(runtime_limit),
            m_P1(nullptr),
            m_P2(nullptr)
        {}

        void setPlayer1(AIInterface *ptr) { assert(checkAI(ptr)); m_P1 = ptr; }
        void setPlayer2(AIInterface *ptr) { assert(checkAI(ptr)); m_P2 = ptr; }

        bool checkAI(AIInterface *ptr)
        {
            return ptr->abi() == AI_ABI_VER;
        }

    private:
        std::size_t m_size;
        std::chrono::milliseconds m_runtime_limit;

        AIInterface *m_P1;
        AIInterface *m_P2;
    } ;
};
