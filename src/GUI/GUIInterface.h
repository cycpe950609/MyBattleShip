#pragma once

#include <cstdio>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unistd.h>

namespace TA
{
    class GUIInterface
    {
    public:
        virtual void title() = 0;
        virtual void appendText(std::string str) = 0;
    };

    

    using std::printf;
    #define ESC "\033"
    class ASCII: public GUIInterface
    {
        const static int GRAPH_HIGHT = 22;
        const static int GRAPH_WIDTH = 80;
        
        const static int TEXT_HIGHT = 10;
        const static int TEXT_WIDTH = 80;

        std::string m_preparedText;
        std::string m_textbuf;

        void cls()
        {
            printf( ESC "[H" ESC "[J" );
        }

        void gotoxy(int y, int x)
        {
            printf( ESC "\033[%d;%df", y, x);
        }

        void updateTextBuf()
        {
            std::stringstream ss(m_textbuf);
            const std::string ban(TEXT_WIDTH, '-');
            std::string tmp, last;

            m_preparedText.clear();
            for(int L=0 ; L < TEXT_HIGHT - 2; ++L)
            {
                if( last == "" )
                {
                    getline(ss, last);
                }
                tmp = last.substr(0, std::min((size_t)TEXT_WIDTH, last.size()) );
                if( tmp.size() == last.size() ) last = "";
                else last = last.substr(TEXT_WIDTH);
    
                m_preparedText = tmp + "\n" + m_preparedText;
            }
            m_textbuf = m_textbuf.substr(0, TEXT_HIGHT * TEXT_WIDTH);
            m_preparedText =  ban + "\n" + m_preparedText + ban;
        }

        void showText()
        {
            gotoxy(GRAPH_HIGHT+1, 0);
            printf( ESC "[J" );
            gotoxy(GRAPH_HIGHT+1, 0);
            puts(m_preparedText.c_str());
            std::fflush(stdout);
        }

        

    public:
        virtual void title() override
        {
            cls();
            puts(R"(    ____        __  __  __    _____ __    _     )");
            puts(R"(   / __ )____ _/ /_/ /_/ /__ / ___// /_  (_)___ )");
            puts(R"(  / __  / __ `/ __/ __/ / _ \\__ \/ __ \/ / __ \)");
            puts(R"( / /_/ / /_/ / /_/ /_/ /  __/__/ / / / / / /_/ /)");
            puts(R"(/_____/\__,_/\__/\__/_/\___/____/_/ /_/_/ .___/ )");
            puts(R"(                                       /_/      )");
        }

        virtual void appendText(std::string str)
        {
            m_textbuf = str + m_textbuf;
            updateTextBuf();
            showText();
        }
    };
    #undef ESC
}