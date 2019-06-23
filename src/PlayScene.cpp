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
bool PlayScene::Playing = true;//If it is playing the GameHistory
PlayScene::PlayScene(TA::BattleShipGame *bsgame)
{
    _bsgame = bsgame;     
}
void PlayScene::Initialize()
{
    AddNewObject(Ocean = new Engine::Group());
    AddNewObject(Ships = new Engine::Group());
    AddNewObject(Turrents = new Engine::Group());
    AddNewObject(Debug = new Engine::Group());
    AddNewObject(Hitted = new Engine::Group());
    InitOcean(100,40,_bsgame->BoardRed.size());
    InitSplitter(100,40 + _bsgame->BoardRed.size()*BlockSize,_bsgame->BoardRed.size());
    InitOcean(100,40 + _bsgame->BoardRed.size()*BlockSize + BlockSize,_bsgame->BoardBlue.size()); 

    InitShip(_bsgame->GameHistory[0].ShipRed,100,40,"red");
    last_ships_red = _bsgame->GameHistory[0].ShipRed;
    InitShip(_bsgame->GameHistory[0].ShipBlue,100,40 + _bsgame->BoardRed.size()*BlockSize + BlockSize,"blue");
    last_ships_blue = _bsgame->GameHistory[0].ShipBlue;

    //Slider of GameHistory
    sliderHistory = new Slider(100,100+_bsgame->BoardRed.size()*BlockSize + BlockSize,_bsgame->BoardRed.size()*BlockSize*2 + BlockSize - 60*2, 20);//60*2 : two btn on both side of Slider
    sliderHistory->SetOnValueChangedCallback(std::bind(&PlayScene::HistorySlideOnValueChanged, this, std::placeholders::_1));
    AddNewControlObject(sliderHistory);
    
    this->SizeofHistory = _bsgame->GameHistory.size();
    
    sliderHistory->SetValue(1/((float)SizeofHistory));
    
    //Control for player
    Engine::ImageButton* btn;
    //Play/Pause
    btn = new Engine::ImageButton("other/mouse-out.png", "other/mouse-in.png",40,100+_bsgame->BoardRed.size()*BlockSize + BlockSize - 10, 50 , 40);
    btn->SetOnClickCallback(std::bind(&PlayScene::ButtonOnClick, this, BTN_PLAYPAUSE));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label(">", "source.ttf", 35,40+25,100+_bsgame->BoardRed.size()*BlockSize + BlockSize - 10 + 20, 0, 0, 0, 255, 0.5, 0.5));
    //Got to End
    int btnPy = 40 + _bsgame->BoardRed.size()*BlockSize*2 + BlockSize - 50;//50 : width of btn
    btn = new Engine::ImageButton("other/mouse-out.png", "other/mouse-in.png", btnPy,100+_bsgame->BoardRed.size()*BlockSize + BlockSize - 10, 50 , 40);
    btn->SetOnClickCallback(std::bind(&PlayScene::ButtonOnClick, this, BTN_GOTOEND));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label(">>", "source.ttf", 35,btnPy + 25,100+_bsgame->BoardRed.size()*BlockSize + BlockSize - 10 + 20, 0, 0, 0, 255, 0.5, 0.5));
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
     if(Playing)
	time += deltaTime;
     //std::cout << "UpdateTest" << deltaTime << std::endl;
     if(time >= 1.0)//1s
     {
	  time = 0;
	  this->Step++;//go ahead step in History every 2s
	  if(Step >= _bsgame->GameHistory.size())
	  {
	      Step = _bsgame->GameHistory.size();
	      Playing = false;
	  }
	  //TODO: Add Bullet
	  sliderHistory->SetValue(((float)Step+1)/((float)SizeofHistory));
	  UpdateHitted(_bsgame->GameHistory[Step].BoardRed,100,40);//Red
	  UpdateHitted(_bsgame->GameHistory[Step].BoardRed,100,40 + _bsgame->BoardRed.size()*BlockSize + BlockSize);//Blue
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
	 Turrents->Clear();
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
void PlayScene::UpdateHitted(TA::Board& board,int dx,int dy)
{
     Hitted->Clear();
     for(int y = 0;y < board.size() ;y++)
 	for(int x = 0; x < board.size();x++)
	{
	    if(board[y][x] == TA::Board::State::Hit)
		Hitted->AddNewObject(new Engine::Image("play/hit.png",y * BlockSize + dy, x * BlockSize + dx, BlockSize, BlockSize));
	    else if(board[y][x] == TA::Board::State::Empty)
		Hitted->AddNewObject(new Engine::Image("play/empty.png",y * BlockSize + dy, x * BlockSize + dx, BlockSize, BlockSize));
	}
	    

}
void PlayScene::UpdateDebug(const TA::Board& board,int dx,int dy)
{
     
}
void PlayScene::AddShip(TA::Ship add_ship,int dx,int dy,std::string color)
{
    //Corner
    Ships->AddNewObject(new Engine::Image(std::string("play/") + color + std::string("-ship-corner-ul.png"),0 * BlockSize + dy + add_ship.y * BlockSize,0 * BlockSize + dx + add_ship.x * BlockSize,BlockSize,BlockSize));//up-left
    Ships->AddNewObject(new Engine::Image(std::string("play/") + color + std::string("-ship-corner-ur.png"),(add_ship.size - 1) * BlockSize + dy + add_ship.y * BlockSize,(0) * BlockSize + dx + add_ship.x * BlockSize,BlockSize,BlockSize));//up-right
    Ships->AddNewObject(new Engine::Image(std::string("play/") + color + std::string("-ship-corner-dl.png"),0 * BlockSize + dy + add_ship.y * BlockSize,(add_ship.size - 1) * BlockSize + dx + add_ship.x * BlockSize,BlockSize,BlockSize));//down-left
    Ships->AddNewObject(new Engine::Image(std::string("play/") + color + std::string("-ship-corner-dr.png"),(add_ship.size - 1) * BlockSize + dy + add_ship.y * BlockSize,(add_ship.size - 1) * BlockSize + dx + add_ship.x * BlockSize,BlockSize,BlockSize));//down-right
    //Side
    for(int i = 0;i < add_ship.size - 2;i++)//Up
	Ships->AddNewObject(new Engine::Image(std::string("play/") + color + std::string("-side-up.png"),(i + 1) * BlockSize + dy + add_ship.y * BlockSize,0 * BlockSize + dx + add_ship.x * BlockSize,BlockSize,BlockSize));
    for(int i = 0;i < add_ship.size - 2;i++)//Right
	Ships->AddNewObject(new Engine::Image(std::string("play/") + color + std::string("-side-right.png"),(add_ship.size - 1) * BlockSize + dy + add_ship.y * BlockSize,(i + 1) * BlockSize + dx + add_ship.x * BlockSize,BlockSize,BlockSize));
    for(int i = 0;i < add_ship.size - 2;i++)//Down
	Ships->AddNewObject(new Engine::Image(std::string("play/") + color + std::string("-side-down.png"),(i + 1) * BlockSize + dy + add_ship.y * BlockSize,(add_ship.size - 1) * BlockSize + dx + add_ship.x * BlockSize,BlockSize,BlockSize));
    for(int i = 0;i < add_ship.size - 2;i++)//Left
	Ships->AddNewObject(new Engine::Image(std::string("play/") + color + std::string("-side-left.png"),(0) * BlockSize + dy + add_ship.y * BlockSize,(i + 1) * BlockSize + dx + add_ship.x * BlockSize,BlockSize,BlockSize));
    //Center:Turrent
    Turrents->AddNewObject(new Engine::Image(std::string("play/turret-") + color + std::string(".png"),(add_ship.size/2) * BlockSize + dy + add_ship.y * BlockSize,(add_ship.size/2)* BlockSize + dx + add_ship.x * BlockSize,BlockSize,BlockSize));
    //Other
    for(int i = 1; i < add_ship.size - 1;i++)
	for(int j = 1;j < add_ship.size - 1;j++)
            Ships->AddNewObject(new Engine::Image(std::string("play/") + color + std::string("-ship-center.png"),i * BlockSize + dy + add_ship.y * BlockSize,j * BlockSize + dx + add_ship.x * BlockSize,BlockSize,BlockSize));
	    





/*
    for(int i = 0;i < add_ship.size;i++)
        for(int j = 0;j < add_ship.size;j++)
        {
            std::string image_name;
            image_name = color + std::string("-ship-center.png");
            Ships->AddNewObject(new Engine::Image(std::string("play/") + image_name,i * BlockSize + dy + add_ship.y * BlockSize,j * BlockSize + dx + add_ship.x * BlockSize,BlockSize,BlockSize));
	    if(i == add_ship.size/2 && j == add_ship.size/2)
		Turrents->AddNewObject(new Engine::Image(std::string("play/turret-") + color + std::string(".png"),i * BlockSize + dy + add_ship.y * BlockSize,j * BlockSize + dx + add_ship.x * BlockSize,BlockSize,BlockSize));
        }
	*/
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
void PlayScene::ButtonOnClick(int btnID)
{
     if(btnID == BTN_PLAYPAUSE)
	 Playing = !Playing;
     else if(btnID == BTN_GOTOEND)
	 Step = _bsgame->GameHistory.size() - 1;
}
