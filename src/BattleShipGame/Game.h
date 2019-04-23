#pragma once

#include <BattleShipGame/Board.h>
#include <BattleShipGame/Wrapper/AI.h>

#include <iostream>
#include <cassert>
#include <chrono>
#include <future>
#include <type_traits>

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

        void run()
        {
            //callWithLimits(&AIInterface::callbackReportHit, m_P1, false);
            //callWithLimits(&AIInterface::init, m_P1);
            call(&AIInterface::init, m_P1);
            //call(&AIInterface::callbackReportHit, m_P1, true);
        } 

   private:

        template<typename Func ,typename... Args, 
            std::enable_if_t< std::is_void<
                    std::invoke_result_t<Func, AIInterface, Args...>
                >::value , int> = 0 >
        void call(Func func, AIInterface *ptr, Args... args)
        {
            std::future_status status;
            auto val = std::async(std::launch::async, func, ptr, args...);
            status = val.wait_for(std::chrono::milliseconds(1000));

            if( status != std::future_status::ready )
            {
                exit(-1);
            }
            val.get();
        }

        template<typename Func ,typename... Args, 
            std::enable_if_t< std::is_void<
                    std::invoke_result_t<Func, AIInterface, Args...>
                >::value == false, int> = 0 >
        auto call(Func func, AIInterface *ptr, Args... args)
            -> std::invoke_result_t<Func, AIInterface, Args...>
        {
            std::future_status status;
            auto val = std::async(std::launch::async, func, ptr, args...);
            status = val.wait_for(std::chrono::milliseconds(1000));

            if( status != std::future_status::ready )
            {
                exit(-1);
            }
            return val.get();
        }

        bool checkAI(AIInterface *ptr) 
        {
            return ptr->abi() == AI_ABI_VER;
        }
        std::size_t m_size;
        std::chrono::milliseconds m_runtime_limit;

        AIInterface *m_P1;
        AIInterface *m_P2;
    } ;
};
