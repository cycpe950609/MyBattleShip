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
    DrawOcean(100,40,_bsgame->BoardRed.size());
    DrawSplitter(100,40 + _bsgame->BoardRed.size()*BlockSize,_bsgame->BoardRed.size());
    DrawOcean(100,40 + _bsgame->BoardRed.size()*BlockSize + BlockSize,_bsgame->BoardBlue.size()); 
    DrawShip(_bsgame->GameHistory[0].ShipRed,100,40,"red");
    DrawShip(_bsgame->GameHistory[0].ShipBlue,100,40 + _bsgame->BoardRed.size()*BlockSize + BlockSize,"blue");
    
    sliderHistory = new Slider(40,100+_bsgame->BoardRed.size()*BlockSize + BlockSize,_bsgame->BoardRed.size()*BlockSize*2 + BlockSize, 20);
    sliderHistory->SetOnValueChangedCallback(std::bind(&PlayScene::HistorySlideOnValueChanged, this, std::placeholders::_1));
    AddNewControlObject(sliderHistory);
    
    this->SizeofHistory = _bsgame->GameHistory.size();
    
    sliderHistory->SetValue(1/((float)SizeofHistory));
    

}
void PlayScene::HistorySlideOnValueChanged(float value) {

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
}
void PlayScene::DrawOcean(int dx,int dy,int size)
{
    for(int y = 0;y < size;y++)
	for(int x = 0; x < size;x++)
	    Ocean->AddNewObject(new Engine::Image("play/water.png",y * BlockSize + dy, x * BlockSize + dx, BlockSize, BlockSize));
}
void PlayScene::DrawShip(const std::vector<Ship>& ships,int dx,int dy,std::string color)
{
    for(Ship it : ships)
    {
	 for(int i = 0;i < it.size;i++)
	     for(int j = 0;j < it.size;j++)
	     {
		 std::string image_name;
		 if(i == it.size/2 && j == it.size/2)//eye
		 {
		     if(it.state == Ship::State::Sink)
			 image_name = color + std::string("-ship-eye-death.png");
		     else
			 image_name = color + std::string("-ship-eye-live.png");
		 }
		 else
		     image_name = color + std::string("-ship-center.png");
		 Ships->AddNewObject(new Engine::Image(std::string("play/") + image_name,i * BlockSize + dy + it.y * BlockSize,j * BlockSize + dx + it.x * BlockSize,BlockSize,BlockSize));
		  
	     }
    }
}
void PlayScene::DrawSplitter(int dx,int dy,int size)
{
    for(int i = 0;i < size;i++)
	Ocean->AddNewObject(new Engine::Image("play/splitter.png",dy, i * BlockSize + dx, BlockSize, BlockSize));
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
    


    if(DebugMode)
    {
	 
    }
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
     
}
