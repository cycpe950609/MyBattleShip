#include "AudioHelper.hpp"
#include "GameEngine.hpp"
#include "Group.hpp"
#include "IObject.hpp"
#include "Image.hpp"
#include "Label.hpp"
#include "PlayScene.hpp"
#include <iostream>
#include <BattleShipGame/Board.h>
#include <BattleShipGame/Ship.h>
#include <vector>
#include "Image.hpp"
#include <string>
#include "Slider.hpp"
#include <cmath>
using namespace TA;

bool PlayScene::DebugMode = false;
PlayScene::PlayScene(TA::BattleShipGame *bsgame)
{
    _bsgame = bsgame;     
}
void PlayScene::Initialize()
{
    AddNewObject(Ocean = new Engine::Group());
    AddNewObject(Ships = new Engine::Group());
    AddNewObject(Debug = new Engine::Group());
    InitOcean(100,40,_bsgame->BoardRed.size());
    InitSplitter(100,40 + _bsgame->BoardRed.size()*BlockSize,_bsgame->BoardRed.size());
    InitOcean(100,40 + _bsgame->BoardRed.size()*BlockSize + BlockSize,_bsgame->BoardBlue.size()); 

    InitShip(_bsgame->GameHistory[0].ShipRed,100,40,"red");
    last_ships_red = _bsgame->GameHistory[0].ShipRed;
    InitShip(_bsgame->GameHistory[0].ShipBlue,100,40 + _bsgame->BoardRed.size()*BlockSize + BlockSize,"blue");
    last_ships_blue = _bsgame->GameHistory[0].ShipBlue;

    sliderHistory = new Slider(40,100+_bsgame->BoardRed.size()*BlockSize + BlockSize,_bsgame->BoardRed.size()*BlockSize*2 + BlockSize, 20);
    sliderHistory->SetOnValueChangedCallback(std::bind(&PlayScene::HistorySlideOnValueChanged, this, std::placeholders::_1));
    AddNewControlObject(sliderHistory);
    
    this->SizeofHistory = _bsgame->GameHistory.size();
    
    sliderHistory->SetValue(1/((float)SizeofHistory));
    

}
void PlayScene::HistorySlideOnValueChanged(float value) 
{
    int real_step = floor(value*SizeofHistory) - 1;//every time we setValue of Slider in Step+1, so real_step are value*sizeofHistory - 1
    if((float)real_step - value*SizeofHistory >= 0.5)//4捨5入(Is it used ?)
    	real_step++;
    if(real_step < 0) real_step = 0;
    if(real_step > SizeofHistory) real_step = SizeofHistory;
    Step = real_step;
    //std::cout << "Step: " << real_step << std::endl;
}
void PlayScene::Update(float deltaTime)
{
     static float time = 0;
     time += deltaTime;
     //std::cout << "UpdateTest" << deltaTime << std::endl;
     if(time >= 2.0)//2s
     {
	  time = 0;
	  this->Step++;//go ahead step in History every 2s
	  //TODO: Add Bullet
	  sliderHistory->SetValue(((float)Step+1)/((float)SizeofHistory));
     }
     UpdateShip(_bsgame->GameHistory[Step].ShipRed,_bsgame->GameHistory[Step].ShipBlue,100,40,100,40 + _bsgame->BoardRed.size()*BlockSize + BlockSize);
}
//Init
void PlayScene::InitOcean(int dx,int dy,int size)/*{{{*/
{
    for(int y = 0;y < size;y++)
	for(int x = 0; x < size;x++)
	    Ocean->AddNewObject(new Engine::Image("play/water.png",y * BlockSize + dy, x * BlockSize + dx, BlockSize, BlockSize));
}
void PlayScene::InitShip(const std::vector<Ship>& ships,int dx,int dy,std::string color)
{
    for(int i = 0;i < ships.size();i++)
	AddShip(ships[i],dx,dy,color);
}
void PlayScene::InitSplitter(int dx,int dy,int size)
{
    for(int i = 0;i < size;i++)
	Ocean->AddNewObject(new Engine::Image("play/splitter.png",dy, i * BlockSize + dx, BlockSize, BlockSize));
}
void PlayScene::InitDebug(const Board& board,int dx,int dy)
{
    for(int y = 0;y < board.size();y++)
	for(int x = 0; x < board.size();x++)
	    if(board[y][x] != TA::Board::State::Unknown)
		Debug->AddNewObject(new Engine::Image("play/hitted.png",y * BlockSize + dy, x * BlockSize + dx, BlockSize, BlockSize));
}/*}}}*/
//Update
bool cmp(const Ship& a,const Ship &b)
{
     if(a.x == b.y)
	 return a.y < b.y;
     else
	 return a.x < b.x;
}
void PlayScene::UpdateShip(std::vector<TA::Ship>& ships_red,std::vector<TA::Ship>& ships_blue ,int rx,int ry,int bx,int by)
{
    bool should_update = false;
    if(ShouldUpdateShip(ships_red,last_ships_red)) should_update = true;
    if(ShouldUpdateShip(ships_blue,last_ships_blue)) should_update = true;

    if(should_update)
    {
	 Ships->Clear();
	 InitShip(ships_red,rx,ry,"red");
	 InitShip(ships_blue,bx,by,"blue");
	 last_ships_red = ships_red;
	 last_ships_blue = ships_blue;
	 std::cout << "Clear Ship : " << Step << std::endl;
    }
}
bool PlayScene::ShouldUpdateShip(std::vector<TA::Ship>& new_ships,std::vector<TA::Ship>& last_ships)
{
     int s = new_ships.size();
     for(int i = 0;i < s;i++)
	 if(new_ships[i].x != last_ships[i].x || new_ships[i].y != last_ships[i].y)
	     return true;
     return false;
}
void PlayScene::UpdateDebug(const TA::Board& board,int dx,int dy)
{
     
}
void PlayScene::ClearShip(TA::Ship del_ship,int dx,int dy,std::string color)
{
}
void PlayScene::AddShip(TA::Ship add_ship,int dx,int dy,std::string color)
{
    for(int i = 0;i < add_ship.size;i++)
        for(int j = 0;j < add_ship.size;j++)
        {
            std::string image_name;
            if(i == add_ship.size/2 && j == add_ship.size/2)//eye
            {
                if(add_ship.state == Ship::State::Sink)
           	 image_name = color + std::string("-ship-eye-death.png");
                else
           	 image_name = color + std::string("-ship-eye-live.png");
            }
            else
                image_name = color + std::string("-ship-center.png");
            Ships->AddNewObject(new Engine::Image(std::string("play/") + image_name,i * BlockSize + dy + add_ship.y * BlockSize,j * BlockSize + dx + add_ship.x * BlockSize,BlockSize,BlockSize));
        }
}
void PlayScene::Draw() const
{
    /*
    const Board p1b = _bsgame->GameHistory[Step].BoardRed;
    const Board p2b = _bsgame->GameHistory[Step].BoardBlue;
    const std::vector<Ship>  p1s = _bsgame->ShipRed;
    const std::vector<Ship>  p2s = _bsgame->ShipBlue;
*/
    IScene::Draw();
}
void PlayScene::OnMouseDown(int button, int mx, int my)
{
    IScene::OnMouseDown(button,mx,my); 
}
void PlayScene::OnMouseMove(int mx, int my)
{
    IScene::OnMouseMove(mx,my);
}
void PlayScene::OnMouseUp(int button, int mx, int my)
{
    IScene::OnMouseUp(button,mx,my);
}
void PlayScene::OnKeyDown(int keyCode)
{
     if(keyCode == ALLEGRO_KEY_TAB)
	 DebugMode = !DebugMode;
}
