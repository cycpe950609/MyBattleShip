#pragma once 

#include <fstream>
#include <iostream>

class UnbufferedStream
{
    std::ofstream m_fout;
    bool m_display;
public:
    UnbufferedStream(std::string file, bool display = true)
    {
        m_fout.rdbuf()->pubsetbuf(nullptr, 0);
        m_fout.open(file);
    }

    template<typename T>
    UnbufferedStream& operator<<(T val)
    {
        m_fout << val;
        if( m_display ) std::cout << val << std::flush;
        return *this;
    }
};

