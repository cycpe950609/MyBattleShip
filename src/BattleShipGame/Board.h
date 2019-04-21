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
        Board(std::size_t width, std::size_t height)
            :m_width(width)
            ,m_height(height)
            ,m_board(width, std::vector<State>(height, State::Unknown))
        {}

        std::size_t width()  const { return m_width;  }
        std::size_t height() const { return m_height; }
        auto& operator[](int x) { return m_board[x]; }
        State& at(int x,int y) { return m_board[x][y]; }

        bool isAlloToHit(int x, int y) const
        {
            return m_board[x][y] == State::Unknown;
        }
    private:
        std::size_t m_width;
        std::size_t m_height;
        std::vector<std::vector<State>> m_board;
    };
} // Namespace TA
