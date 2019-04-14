#pragma once

#include <vector>

namespace TA
{
    struct Car
    {
        /* 
            it mean car hold those position b[i][j]
            for i = x, x+1, ..., x+size-1
            for j = y, y+1, ..., y+size-1
        */
        std::size_t size;
        std::size_t x;
        std::size_t y;
    };

    class Board
    {
    public:
        enum class State
        {
            Unknown,
            Land,
            AttackedCar
        };
        Board(std::size_t width, std::size_t height)
            :m_width(width)
            ,m_height(height)
            ,m_board(width, std::vector<State>(height, State::Unknown))
        {}

        std::size_t width()  const { return m_width;  }
        std::size_t height() const { return m_height; }
    private:
        std::size_t m_width;
        std::size_t m_height;
        std::vector<std::vector<State>> m_board;
    };
} // Namespace TA
