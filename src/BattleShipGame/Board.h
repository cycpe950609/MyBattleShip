#pragma once

#include <vector>

namespace TA
{
    class Board
    {
    public:
        enum class State
        {
            Unknown,
            Empty,
            Hit
        };
        Board(std::size_t size)
            :m_size(size)
            ,m_board(size, std::vector<State>(size, State::Unknown))
        {}

        std::size_t size()  const { return m_size;  }
        auto& operator[](int x) { return m_board[x]; }

    private:
        std::size_t m_size;
        std::vector<std::vector<State>> m_board;
    };
} // Namespace TA
