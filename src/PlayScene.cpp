#include "AudioHelper.hpp"
#include "GameEngine.hpp"
#include "Group.hpp"
#include "IObject.hpp"
#include "Image.hpp"
#include "Label.hpp"
#include "PlayScene.hpp"


bool PlayScene::DebugMode = false;

PlayScene::PlayScene(TA::BattleShipGame* bsgame)
{
    this->_bsgame = bsgame;

}
void PlayScene::Initialize()
{
    _bsgame->run();
}
void PlayScene::Update(float deltaTime)
{
     
}
void PlayScene::Draw() const
{
    IScene::Draw();
    if(DebugMode)
    {
	 
    }
     
}
void PlayScene::OnMouseDown(int button, int mx, int my)
{
     
}
void PlayScene::OnMouseMove(int mx, int my)
{
     
}
void PlayScene::OnMouseUp(int button, int mx, int my)
{
     
}
void PlayScene::OnKeyDown(int keyCode)
{
     
}
