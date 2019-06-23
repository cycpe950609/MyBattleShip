#pragma once

#include <BattleShipGame/Board.h>
#include <BattleShipGame/Wrapper/AI.h>
#include <GUI/GUIInterface.h>

#include <iostream>
#include <cassert>
#include <chrono>
#include <cstdarg>
#include <future>
#include <type_traits>
#include <iostream>

namespace TA
{
    struct GameState
    {
	Board BoardRed;
	Board BoardBlue;
	std::vector<Ship> ShipRed;
	std::vector<Ship> ShipBlue;
	enum HitWay
	{
	    Red2Blue,
	    Blue2Red
	};
	HitWay hit;
	GameState(Board b_red,Board b_blue,std::vector<Ship> s_red,std::vector<Ship> s_blue,HitWay _hit):BoardRed(b_red),BoardBlue(b_blue),ShipRed(s_red),ShipBlue(s_blue),hit(_hit){}; 
    };
    class BattleShipGame
    {
    public:
        BattleShipGame(
            int size,
            std::chrono::milliseconds runtime_limit = std::chrono::milliseconds(1000)
        ):
            m_size(size),
            m_runtime_limit(runtime_limit),
            m_P1(nullptr),
            m_P2(nullptr),
            m_P1Board(size),
            m_P2Board(size)
        {
            gui = new ASCII;
            m_ship_size = {3,3,5,7};
        }
	BattleShipGame(const BattleShipGame&) = delete;
	BattleShipGame& operator=(const BattleShipGame&) = delete;

        void setPlayer1(AIInterface *ptr) { assert(checkAI(ptr)); m_P1 = ptr; }
        void setPlayer2(AIInterface *ptr) { assert(checkAI(ptr)); m_P2 = ptr; }

	std::vector<GameState> GameHistory;

        void run()
        {
            gui->title();
            if( !prepareState() ) return ;

            updateGuiGame();
	    /*{{{
	    GameHistory.push_back(GameState(BoardRed,BoardBlue,ShipRed,ShipBlue,TA::GameState::Red2Blue));
	    m_P1Board[2][2]	= TA::Board::State::Hit;
	    m_P1Board[15][7]	= TA::Board::State::Empty;
	    m_P1Ship[3].x = 3;
	    m_P1Ship[3].y = 12;
	    GameHistory.push_back(GameState(BoardRed,BoardBlue,ShipRed,ShipBlue,TA::GameState::Red2Blue));
	    m_P1Board[3][6]	= TA::Board::State::Hit;
	    m_P1Board[15][9]	= TA::Board::State::Empty;
	    GameHistory.push_back(GameState(BoardRed,BoardBlue,ShipRed,ShipBlue,TA::GameState::Red2Blue));
	    m_P1Board[5][5]	= TA::Board::State::Hit;
	    m_P1Board[5][17]	= TA::Board::State::Empty;
	    GameHistory.push_back(GameState(BoardRed,BoardBlue,ShipRed,ShipBlue,TA::GameState::Red2Blue));
	    GameHistory.push_back(GameState(BoardRed,BoardBlue,ShipRed,ShipBlue,TA::GameState::Red2Blue));
	    m_P1Board[10][19]	= TA::Board::State::Hit;
	    m_P1Board[15][17]	= TA::Board::State::Empty;
	    GameHistory.push_back(GameState(BoardRed,BoardBlue,ShipRed,ShipBlue,TA::GameState::Red2Blue));
	    m_P2Ship[1].x = 12;
	    m_P2Ship[1].y = 3;
	    GameHistory.push_back(GameState(BoardRed,BoardBlue,ShipRed,ShipBlue,TA::GameState::Red2Blue));
	    m_P1Board[12][12]	= TA::Board::State::Hit;
	    m_P1Board[1][0]	= TA::Board::State::Empty;
	    GameHistory.push_back(GameState(BoardRed,BoardBlue,ShipRed,ShipBlue,TA::GameState::Red2Blue));
	    GameHistory.push_back(GameState(BoardRed,BoardBlue,ShipRed,ShipBlue,TA::GameState::Red2Blue));
	    m_P1Board[0][0]	= TA::Board::State::Hit;
	    GameHistory.push_back(GameState(BoardRed,BoardBlue,ShipRed,ShipBlue,TA::GameState::Red2Blue));
	    GameHistory.push_back(GameState(BoardRed,BoardBlue,ShipRed,ShipBlue,TA::GameState::Red2Blue));
	    GameHistory.push_back(GameState(BoardRed,BoardBlue,ShipRed,ShipBlue,TA::GameState::Red2Blue));
	    GameHistory.push_back(GameState(BoardRed,BoardBlue,ShipRed,ShipBlue,TA::GameState::Red2Blue));
	    GameHistory.push_back(GameState(BoardRed,BoardBlue,ShipRed,ShipBlue,TA::GameState::Red2Blue));
	    GameHistory.push_back(GameState(BoardRed,BoardBlue,ShipRed,ShipBlue,TA::GameState::Red2Blue));
	    GameHistory.push_back(GameState(BoardRed,BoardBlue,ShipRed,ShipBlue,TA::GameState::Red2Blue));
	    GameHistory.push_back(GameState(BoardRed,BoardBlue,ShipRed,ShipBlue,TA::GameState::Red2Blue));
	    GameHistory.push_back(GameState(BoardRed,BoardBlue,ShipRed,ShipBlue,TA::GameState::Red2Blue));
	    GameHistory.push_back(GameState(BoardRed,BoardBlue,ShipRed,ShipBlue,TA::GameState::Red2Blue));
	    GameHistory.push_back(GameState(BoardRed,BoardBlue,ShipRed,ShipBlue,TA::GameState::Red2Blue));
	    *//*}}}*/
            //Todo: Play Game
	     int nowplaying = 1;/*{{{*/
            std::vector<std::pair<int, int>> atkcoor;

            while(1){
                //update information session
                if(nowplaying == 1){
                    putToGui("P1 update information.\n");
                    call(&AIInterface::callbackReportEnemy, m_P1, atkcoor);
                }
                else{
                    putToGui("P2 update information.\n");
                    call(&AIInterface::callbackReportEnemy, m_P2, atkcoor);
                }
                atkcoor.clear();

                //attack session
                bool end = false;
                std::pair<int, int> atkin;
                if(nowplaying == 1){
                    for( auto &ship : m_P1Ship){
                        //only alive ships can attack
                        if(ship.state == Ship::State::Sink) continue;
                        putToGui("Ask P1 where to hit.\n");
                        atkin = call(&AIInterface::queryWhereToHit, m_P1, m_P2Board);

                        //Illegal attack detecting
                        if(m_P2Board[atkin.first][atkin.second] != Board::State::Unknown){
                            putToGui("Illegal attack. P1 loses.\n");
                            call(&AIInterface::callbackReportHit, m_P1, false);
                            end = true;
                            break;
                        }
                        else{
                            putToGui("Attack executing.\n");
                        }

                        //Hit anything
                        bool Ishit = false;
                        for(auto &ship : m_P2Ship){
                            if(atkin.first >= ship.x && atkin.first <= ship.x + ship.size - 1){
                                if (atkin.second >= ship.y && atkin.second <= ship.y + ship.size - 1){
                                    Ishit = true;
                                    if(atkin.first == ship.x + ship.size/2 && atkin.second == ship.y + ship.size/2){
                                        putToGui("Ship hit at core! Ship sank!\n");
                                        ship.state = Ship::State::Sink;
                                    }
                                    else{
                                        putToGui("Ship hit!\n");
                                        if(ship.state != Ship::State::Sink) ship.state = Ship::State::Hit;
                                    }
                                    m_P2Board[atkin.first][atkin.second] = Board::State::Hit;
                                    continue;
                                }
                            }
                        }
                        if (!Ishit)
                        {
                            putToGui("Did not hit anything!\n");
                            m_P2Board[atkin.first][atkin.second] = Board::State::Empty;
                        }
                        call(&AIInterface::callbackReportHit, m_P1, Ishit);
                        atkcoor.push_back(atkin);
                        updateGuiGame();

                        //check gameover
                        bool win = checkGameover();
                        if(win){
                            putToGui("P1 wins!\n");
                            end = true;
                            break;
                        }
                    }
                }
                else{
                    for( auto &ship : m_P2Ship){
                        //only alive ships can attack
                        if(ship.state == Ship::State::Sink) continue;
                        putToGui("Ask P1 where to hit.\n");
                        atkin = call(&AIInterface::queryWhereToHit, m_P2, m_P1Board);

                        //Illegal attack detecting
                        if(m_P1Board[atkin.first][atkin.second] != Board::State::Unknown){
                            putToGui("Illegal attack. P2 loses.\n");
                            call(&AIInterface::callbackReportHit, m_P2, false);
                            end = true;
                            break;
                        }
                        else{
                            putToGui("Attack executing.\n");
                        }

                        //Hit anything
                        bool Ishit = false;
                        for(auto &ship : m_P1Ship){
                            if(atkin.first >= ship.x && atkin.first <= ship.x + ship.size - 1){
                                if (atkin.second >= ship.y && atkin.second <= ship.y + ship.size - 1){
                                    Ishit = true;
                                    if(atkin.first == ship.x + ship.size/2 && atkin.second == ship.y + ship.size/2){
                                        putToGui("Ship hit at core! Ship sank!\n");
                                        ship.state = Ship::State::Sink;
                                    }
                                    else{
                                        putToGui("Ship hit!\n");
                                        if(ship.state != Ship::State::Sink) ship.state = Ship::State::Hit;
                                    }
                                    m_P1Board[atkin.first][atkin.second] = Board::State::Hit;
                                    continue;
                                }
                            }
                        }
                        if (!Ishit)
                        {
                            putToGui("Did not hit anything!\n");
                            m_P1Board[atkin.first][atkin.second] = Board::State::Empty;
                        }
                        call(&AIInterface::callbackReportHit, m_P2, Ishit);
                        atkcoor.push_back(atkin);
                        updateGuiGame();

                        //check gameover
                        bool win = checkGameover();
                        if(win){
                            end = true;
                            break;
                        }
                    }
                }

                if(end){
                    putToGui("Game end!\n");
                    break;
                }

                //moving session
                end = false;
                std::vector<std::pair<int, int>> movcoor;
                if(nowplaying == 1){
                    movcoor = call(&AIInterface::queryHowToMoveShip, m_P1, m_P1Ship);
                    bool valid = checkMoveValid(movcoor, m_P1Ship);
                    if(valid){
                        //move ship
                        int id = -1;
                        for( std::vector<Ship>::iterator i = m_P1Ship.begin(); i != m_P1Ship.end(); i++)
                        {
                            id++;
                            putToGui("Ship %d move from (%d,%d) to (%d,%d).\n",id ,i->x, i->y, movcoor[id].first, movcoor[id].second);
                            i->x = movcoor[id].first;
                            i->y = movcoor[id].second;
                        }
                        updateGuiGame();
                    }
                    else{
                        putToGui("P1 lose!\n");
                        end = true;
                    }
                }
                else{
                    movcoor = call(&AIInterface::queryHowToMoveShip, m_P2, m_P2Ship);
                    bool valid = checkMoveValid(movcoor, m_P2Ship);
                    if(valid){
                        //move ship
                        int id = -1;
                        for( std::vector<Ship>::iterator i = m_P2Ship.begin(); i != m_P2Ship.end(); i++)
                        {
                            id++;
                            putToGui("Ship %d move from (%d,%d) to (%d,%d).\n",id ,i->x, i->y, movcoor[id].first, movcoor[id].second);
                            i->x = movcoor[id].first;
                            i->y = movcoor[id].second;
                        }
                        updateGuiGame();
                    }
                    else{
                        putToGui("P2 lose!\n");
                        end = true;
                    }
                }

                if(end){
                    putToGui("Game end!\n");
                    break;
                }

                //change session
                if(nowplaying == 1){
                    putToGui("Change to P2!\n");
                    nowplaying = 2;
                }
                else{
                    putToGui("Change to P1!\n");
                    nowplaying = 1;
                }

	    GameHistory.push_back(GameState(BoardRed,BoardBlue,ShipRed,ShipBlue,TA::GameState::Red2Blue));
            }/*}}}*/


        } 

	//Set function pointer to PlayScene
	/*void SetGUICall(UpdateGUI op)
	{
	     updategui = op;
	}*/
	typedef void (*UpdateHIT)();//TODO : pass what ?
	void SetHITCall(UpdateHIT op)
	{
	     updatehit = op;
	}
	const Board &BoardRed  = m_P1Board;
	const Board &BoardBlue = m_P2Board;
        const std::vector<Ship> &ShipRed  = m_P1Ship;
        const std::vector<Ship> &ShipBlue = m_P2Ship;
   private:
	//typedef void (*UpdateGUI(Board p1b, std::vector<Ship> p1s, Board p2b, std::vector<Ship> p2s));
	//UpdateGUI updategui;
	UpdateHIT updatehit;
	
	bool checkMoveValid(std::vector<std::pair<int ,int>> coor, std::vector<Ship> ships)
        {
            if( ships.size() != coor.size() )
            {
                putToGui("Invalid move. Move number not match : real %zu ,expect %zu,\n",coor.size(), ships.size());
                return false;
            }

            std::vector<std::vector<int>> map(m_size, std::vector<int>(m_size));

            int id = -1;
            for( auto [size, x, y, state] : ships )
            {
                id++;
                if(state != Ship::State::Available){
                    if(coor[id].first != x || coor[id].second != y){
                        putToGui("Invalid move. Trying to move non-Available ships.\n");
                        return false;
                    }
                }
                else{
                    if((coor[id].first != x && coor[id].second != y) || (coor[id].first - x < -1 || coor[id].first - x > 1) || (coor[id].second - y < -1 || coor[id].second - y > 1)){
                        putToGui("Invalid move. Trying to move more than one step.\n");
                        return false;
                    }
                }

                for( int dx = 0 ; dx < size ; dx++ ){
                    for( int dy = 0 ; dy < size ; dy++ ){
                        int nx = coor[id].first + dx;
                        int ny = coor[id].second + dy;

                        if( nx < 0 || nx >= m_size || ny < 0 || ny >= m_size )
                        {
                            putToGui("Ship %d out of range at (%d,%d),\n", id, nx, ny);
                            return false;
                        }
                        if( map[nx][ny] != 0 )
                        {
                            putToGui("Ship %d and %d overlapping at (%d,%d),\n", id,  map[nx][ny]-1, nx, ny);
                            return false;
                        }
                        map[nx][ny] = id + 1;
                    }
                }
            }
            return true;
        }



        virtual void updateGuiGame()
        {
            gui->updateGame(m_P1Board, m_P1Ship, m_P2Board, m_P2Ship);
        }

        bool checkGameover()
        {
            bool flag = true;

            for( Ship s : m_P1Ship )
                if( s.state != Ship::State::Sink )
                    flag = false;
            if( flag )
            {
                putToGui("P2 win!\n");
                return true;
            }
            for( Ship s : m_P2Ship )
                if( s.state != Ship::State::Sink )
                    return false;
            putToGui("P1 win!\n");
            return true;
        }

        bool prepareState()
        {
            std::vector<Ship> initPos;

            putToGui("P1 Prepareing...\n");
            initPos = call(&AIInterface::init, m_P1, m_size, m_ship_size, true, m_runtime_limit);
            if( !checkShipPosition(initPos) )
            {
                putToGui("P1 Init() Invaild...\n");
                return false;
            }
            for( auto &ship : initPos )
                ship.state = Ship::State::Available;
            m_P1Ship = initPos;
            putToGui("Done.\n");

            initPos.clear();

            putToGui("P2 Prepareing...\n");
            initPos = call(&AIInterface::init, m_P2, m_size, m_ship_size, false, m_runtime_limit);
            if( !checkShipPosition(initPos) )
            {
                putToGui("P2 Init() Invaild...\n");
                return false;
            }
            for( auto &ship : initPos )
                ship.state = Ship::State::Available;
            m_P2Ship = initPos;
            putToGui("Done.\n");
            return true;
        }

        template<typename Func ,typename... Args, 
            std::enable_if_t< std::is_void<
                    std::invoke_result_t<Func, AIInterface, Args...>
                >::value , int> = 0 >
        void call(Func func, AIInterface *ptr, Args... args)
        {
            std::future_status status;
            auto val = std::async(std::launch::async, func, ptr, args...);
            status = val.wait_for(std::chrono::milliseconds(m_runtime_limit));

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
            status = val.wait_for(std::chrono::milliseconds(m_runtime_limit));

            if( status != std::future_status::ready )
            {
                exit(-1);
            }
            return val.get();
        }

        virtual void putToGui(const char *fmt, ...)
        {
            va_list args1;
            va_start(args1, fmt);
            va_list args2;
            va_copy(args2, args1);
            std::vector<char> buf(1+std::vsnprintf(nullptr, 0, fmt, args1));
            va_end(args1);
            std::vsnprintf(buf.data(), buf.size(), fmt, args2);
            va_end(args2);

            if( buf.back() == 0 ) buf.pop_back();
            gui->appendText( std::string(buf.begin(), buf.end()) );
        }

        bool checkAI(AIInterface *ptr) 
        {
            return ptr->abi() == AI_ABI_VER;
        }

        bool checkShipPosition(std::vector<Ship> ships)
        {
            
            if( ships.size() != m_ship_size.size() )
            {
                putToGui("Ship number not match : real %zu ,expect %zu,\n",ships.size(), m_ship_size.size());
                return false;
            }
            
            std::sort(ships.begin(), ships.end(), [](Ship a, Ship b){
                return a.size < b.size; 
            });

            std::vector<std::vector<int>> map(m_size, std::vector<int>(m_size));

            int id = -1;
            for( auto [size, x, y, state] : ships )
            {
                id++;
                if( size != m_ship_size[id] )
                {
                    putToGui("Ship %d size not match : real %zu ,expect %zu,\n", id, size, m_ship_size[id]);
                    return false;
                }

                for( int dx = 0 ; dx < size ; dx++ )
                    for( int dy = 0 ; dy < size ; dy++ )
                        {
                            int nx = x + dx;
                            int ny = y + dy;

                            if( nx < 0 || nx >= m_size || ny < 0 || ny >= m_size )
                            {
                                putToGui("Ship %d out of range at (%d,%d),\n", id, nx, ny);
                                return false;
                            }

                            if( map[nx][ny] != 0 )
                            {
                                putToGui("Ship %d and %d overlapping at (%d,%d),\n", id,  map[nx][ny]-1, nx, ny);
                                return false;
                            }
                            map[nx][ny] = id + 1;
                        }
            }
            return true;
        }

        int m_size;
        std::vector<int> m_ship_size;
        std::chrono::milliseconds m_runtime_limit;

        AIInterface *m_P1;
        AIInterface *m_P2;
        GUIInterface *gui;

        std::vector<Ship> m_P1Ship;
        std::vector<Ship> m_P2Ship;
        Board m_P1Board;
        Board m_P2Board;
    } ;
}
