#pragma once

#include <BattleShipGame/Wrapper/AI.h>
#include <BattleShipGame/Ship.h>
#include <algorithm>
#include <random>
#include <ctime>
#include <queue>
#include <iostream>

#define B_SIZE 20

class pt
{
public:
    int x;
    int y;
};
/*
class area
{
public:
    int l;
    int r;
    int u;
    int d;
};*/

//extern int seven_sinked, five_sinked, three_sinked; //if sinked, set it to 1; if both of "three" sinked, set it to 2
static int attack_state; //past-attack state
/*
state list:
0. init my_board
1. init 4 pts
2. hit and find border
3. find the center and hit it
4. queue is Empty and find next pt to attack
5. kill all for guarantee
*/
static int left = -2, right = -2, up = -2, down = -2;
static pt last_hit;
std::queue<pt> hit_pt; //using queue record pt be hit
//std::queue<area> not_deal; //the areas aren't dealed
//static TA::Board my_board(B_SIZE);
static int past_enemyNum = 4;

using namespace TA;

class AI : public AIInterface
{
    int enemyNum;
    int Boardsize;
    std::vector<std::vector<bool>> myBoard;
    int map[9];
    std::vector<std::pair<int,int>> way;
public:
	AI() : AIInterface(),
		   Boardsize(20),
		   myBoard(20, std::vector<bool>(20, false))
	{
		for (int i = 0; i < 9; i++) map[i] = 0;
	}

    virtual std::vector<TA::Ship> init(int size ,std::vector<int> ship_size, bool order, std::chrono::milliseconds runtime) override
    {
        (void)ship_size;
        (void)runtime;

        std::vector<TA::Ship> tmp;
        tmp.push_back({3,  6, 11,TA::Ship::State::Available});
        tmp.push_back({3,  9,  9,TA::Ship::State::Available});
        tmp.push_back({5, 12,  7,TA::Ship::State::Available});
        tmp.push_back({7, 11, 12,TA::Ship::State::Available});
        
        for(int i=0;i<size;++i)
            for(int j=0;j<size;++j)
                way.emplace_back(i,j);

        std::mt19937 mt;
        mt.seed( std::time(nullptr) + 7122 + (order?1:0) );
        std::shuffle(way.begin(), way.end(), mt);
        return tmp;
    }

    virtual void callbackReportEnemy(std::vector<std::pair<int,int>> attack) override
    {
		enemyNum = attack.size();
		for (int i = 0; i < enemyNum; i++) {
			int a = attack[i].first;
			int b = attack[i].second;
				 if (a <= 6 			&& b <= 6			 ) map[0]++;
			else if (a <= 6 			&& (6 < b && b <= 13)) map[1]++;
			else if (a <= 6 			&& 			13  < b  ) map[2]++;
			else if ((6 < a && a <= 13) && b <= 6			 ) map[3]++;
			else if ((6 < a && a <= 13) && (6 < b && b <= 13)) map[4]++;
			else if ((6 < a && a <= 13) && 			13  < b  ) map[5]++;
			else if (		  13  <  a  && b <= 6			 ) map[6]++;
			else if (		  13  <  a 	&& (6 < b && b <= 13)) map[7]++;
			else if (		  13  <  a  && 			13  < b  ) map[8]++;
		}
    }

    virtual std::pair<int,int> queryWhereToHit(TA::Board board) override
    {
	while(1)
    {
        if(attack_state == 0)
        {
            for(int i = 6; i < B_SIZE; i+=7)
            {
                for(int j = 6; j < B_SIZE; j+=7)
                {
                    if(board[i][j] == TA::Board::State::Unknown)
                    {
                        last_hit.x = i;
                        last_hit.y = j;
                        return {i, j};
                    }
                    else if(board[i][j] == TA::Board::State::Hit)
                    {
                        last_hit.x = i;
                        last_hit.y = j;
                        hit_pt.push(last_hit);
                    }
                }
            }

            if(board[1][1] == TA::Board::State::Unknown)
            {
                last_hit.x = 1;
                last_hit.y = 1;
                return {1, 1};
            }
            else if(board[1][1] == TA::Board::State::Hit)
            {
                last_hit.x = 1;
                last_hit.y = 1;
                hit_pt.push(last_hit);
            }

            if(board[6][1] == TA::Board::State::Unknown)
            {
                last_hit.x = 6;
                last_hit.y = 1;
                return {6, 1};
            }
            else if(board[6][1] == TA::Board::State::Hit)
            {
                last_hit.x = 5;
                last_hit.y = 1;
                hit_pt.push(last_hit);
            }

            if(board[2][7] == TA::Board::State::Unknown)
            {
                last_hit.x = 2;
                last_hit.y = 7;
                return {2, 7};
            }
            else if(board[2][7] == TA::Board::State::Hit)
            {
                last_hit.x = 2;
                last_hit.y = 7;
                hit_pt.push(last_hit);
            }
            attack_state = 1;
        }

        if(attack_state == 1)
        {
            if(past_enemyNum > enemyNum)
            {
                past_enemyNum = enemyNum;
                hit_pt.pop();
                if(hit_pt.empty())
                {
                    attack_state = 2;
                }
                continue;
            }
            for(int i = -3; i < 4; i++)
            {
                for(int j = -3; j < 4; j++)
                {
                    if(hit_pt.front().x + i >= 0 && hit_pt.front().x + i < B_SIZE && hit_pt.front().y + j >= 0 && hit_pt.front().y + j < B_SIZE)
                    {
                        if(board[hit_pt.front().x + i][hit_pt.front().y + j] == TA::Board::State::Unknown)
                        {
                            last_hit.x = hit_pt.front().x + i;
                            last_hit.y = hit_pt.front().y + j;
                            past_enemyNum = enemyNum;
                            return {last_hit.x, last_hit.y};
                        }
                    }
                }
            }
            hit_pt.pop();
            attack_state = 2;
        }

        if(attack_state == 2)
        {
            if(!hit_pt.empty())
            {
                attack_state = 1;
                past_enemyNum = enemyNum;
                continue;
            }
            std::random_device rd;
            std::default_random_engine gen = std::default_random_engine(rd());
            std::uniform_int_distribution<int > dis(0,19);
            int a = dis(gen);
            int b = dis(gen);

            if(board[a][b] == TA::Board::State::Unknown)
            {
                last_hit.x = a;
                last_hit.y = b;
                attack_state = 3;
                return {last_hit.x, last_hit.y};
            }
            else continue;
        }

        if(attack_state == 3)
        {
            past_enemyNum = enemyNum;
            if(board[last_hit.x][last_hit.y] == TA::Board::State::Hit)
            {
                hit_pt.push(last_hit);
                attack_state = 1;
            }
            else
            {
                attack_state = 2;
            }
            continue;
        }
    }
    }





    virtual void callbackReportHit(bool)  override
    {

    }

    std::vector<std::pair<int,int>> queryHowToMoveShip(std::vector<TA::Ship> Movable)
    {
   std::vector<std::pair<int,int>> Move;
		for (int i = 0; i < Movable.size(); i++) {
			int a = Movable[i].x;
			int b = Movable[i].y;
            if (Movable[i].state != Ship::State::Available) {
                Move.push_back({a, b});
                continue;
            }
			int area;
				 if (a <= 6 			&& b <= 6			 ) area = 0;
			else if (a <= 6 			&& (6 < b && b <= 13)) area = 1;
			else if (a <= 6 			&& 			13  <  b ) area = 2;
			else if ((6 < a && a <= 13) && b <= 6			 ) area = 3;
			else if ((6 < a && a <= 13) && (6 < b && b <= 13)) area = 4;
			else if ((6 < a && a <= 13) && 			13  <  b ) area = 5;
			else if (		  13  <  a  && b <= 6			 ) area = 6;
			else if (		  13  <  a  && (6 < b && b <= 13)) area = 7;
			else if (		  13  <  a  && 			13  <  b ) area = 8;
			int Max = 0, Marea;
			for (int j = 0; j < 9; j++) {
				if (Max < map[j]) {
					Max = map[j];
					Marea = j;
				}
			}
			int x = 0, y = 0;
			int diff = area - Marea;
			if (diff > 0 && diff % 3 != 0) x =  1;
			if (diff < 0 && diff % 3 != 0) x = -1;
			if (diff > 0 && diff / 3 != 0) y =  1;
			if (diff < 0 && diff / 3 != 0) y = -1;
			bool flag1 = true;
			bool flag2 = true;
			for (int u = 0; u < Movable[i].size; u++) {
				if (a+x+u < 0 || a+x+u >= Boardsize) {flag1 = false;    break;}
				for (int v = 0; v < Movable[i].size; v++) {
                    if (myBoard[a+x+u][b+v] == true) {flag1 = false;    break;}
					for (int k = 0; k < i; k++) {
                        if (Move[k].first <= a+x+u && a+x+u < Move[k].first + Movable[k].size &&
                            Move[k].second <= b+v  && b+v < Move[k].second + Movable[k].size)
                            {flag1 = false; break;}
					}
				}
				if (flag1 == false) break;
			}
			for (int v = 0; v < Movable[i].size; v++) {
				if (b+y+v < 0 || b+y+v >= Boardsize) {flag2 = false;    break;}
				for (int u = 0; u < Movable[i].size; u++) {
                    if (myBoard[a+u][b+y+v] == true) {flag2 = false;    break;}
					for (int k = 0; k < i; k++) {
                        if (Move[k].first  <= a+u && a+u < Move[k].first  + Movable[k].size &&
                            Move[k].second <= b+y+v && b+y+v < Move[k].second + Movable[k].size)
                            {flag2 = false; break;}
					}
				}
				if (flag2 == false) break;
			}

                 if (flag1) Move.push_back({a + x, b});
			else if (flag2) Move.push_back({a, b + y});
			else 	        Move.push_back({a, b});
		}
		bool flag = true;
		bool board[20][20];
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 20; j++) {
				board[i][j] = false;
			}
		} 
		for (int i = 0; i < Movable.size(); i++) {
			for (int u = 0; u < Movable[i].size; u++) {
				for (int v = 0; v < Movable[i].size; v++) {
					if (board[Move[i].first+u][Move[i].second+v] == true) {
						flag = false;
						break;
					}
					else board[Move[i].first+u][Move[i].second+v] == true;
				}
			}
		}
		if (!flag) {
			std::vector<std::pair<int,int>> Move1;
			for (int i = 0; i < Movable.size(); i++) {
				int a = Movable[i].x;
				int b = Movable[i].y;
				Move1.push_back({a, b});
			}
			return Move1;
		}
		
    	return Move;
    }
};
