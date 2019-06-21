#include <functional>
#include <string>

#include "AudioHelper.hpp"
#include "GameEngine.hpp"
#include "ImageButton.hpp"
#include "Label.hpp"
#include "Point.hpp"
#include "StartScene.hpp"
#include <iostream>

#define BTN_WELCOME 0
#define BTN_EXIT 1


void StartScene::Initialize() {

	int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
	int h = Engine::GameEngine::GetInstance().GetScreenSize().y;

	int halfW = w / 2;
	int halfH = h / 2;

	//enter
	Engine::ImageButton* btn;
	btn = new Engine::ImageButton("welcome/dirt.png", "welcome/floor.png", halfW - 250, halfH / 2 , 500, 100);
	btn->SetOnClickCallback(std::bind(&StartScene::ButtonOnClick, this, BTN_WELCOME));
	AddNewControlObject(btn);
	AddNewObject(new Engine::Label("Enter Game", "pirulen.ttf", 48, halfW, halfH  / 2+50, 0, 0, 0, 255, 0.5, 0.5));

	//exit
	btn = new Engine::ImageButton("welcome/dirt.png", "welcome/floor.png", halfW - 200, halfH / 2 + 150, 400, 100);
	btn->SetOnClickCallback(std::bind(&StartScene::ButtonOnClick, this, BTN_EXIT));
	AddNewControlObject(btn);
	AddNewObject(new Engine::Label("Exit", "pirulen.ttf", 48, halfW, halfH / 2 + 200, 0, 0, 0, 255, 0.5, 0.5));


	/*
	int halfW = w / 2;
	int halfH = h / 2;
	AddNewObject(new Engine::Label("You Win!", "pirulen.ttf", 48, halfW, halfH / 2, 255, 255, 255, 255, 0.5, 0.5));
	Engine::ImageButton* btn;
	btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH * 3 / 2 - 50, 400, 100);
	btn->SetOnClickCallback(std::bind(&WinScene::BackOnClick, this, 2));
	AddNewControlObject(btn);
	AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));
	AudioHelper::PlayAudio("win.wav");*/
}
//void StartScene::Update(float deltaTime) {
    /*
	ticks += deltaTime;
	if (ticks > 4 && ticks < 100 &&
		dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"))->MapId == 2) {
		ticks = 100;
		AudioHelper::PlayBGM("happy.ogg");
	}*/
//}
void StartScene::ButtonOnClick(int btnID) {
	// Change to select scene.
	
	if(btnID == BTN_WELCOME)
	    std::cout << "BtnClick\n";
	    //Engine::GameEngine::GetInstance().ChangeScene("stage-select");
	//else if(btnID == BTN_EXIT)
	//  Engine::GameEngine::GetInstance().destroy();
}
