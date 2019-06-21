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
	btn = new Engine::ImageButton("welcome/dirt.png", "welcome/floor.png", halfW - 150, halfH / 2 + 425 , 300, 50);
	btn->SetOnClickCallback(std::bind(&StartScene::ButtonOnClick, this, BTN_WELCOME));
	AddNewControlObject(btn);
	AddNewObject(new Engine::Label("Enter Game", "source.ttf", 30, halfW, halfH  / 2+450, 0, 0, 0, 255, 0.5, 0.5));
	//Title
	/*
	    puts(R"(    ____        __  __  __    _____ __    _     )");
            puts(R"(   / __ )____ _/ /_/ /_/ /__ / ___// /_  (_)___ )");
            puts(R"(  / __  / __ `/ __/ __/ / _ \\__ \/ __ \/ / __ \)");
            puts(R"( / /_/ / /_/ / /_/ /_/ /  __/__/ / / / / / /_/ /)");
            puts(R"(/_____/\__,_/\__/\__/_/\___/____/_/ /_/_/ .___/ )");
            puts(R"(                                       /_/      )");
	*/
	AddNewObject(new Engine::Label(R"(    ____        __  __  __    _____ __    _     )", "source.ttf", 25, halfW, halfH + 200 - 300, 255 , 255 , 255 , 255, 0.5, 0.5));
	AddNewObject(new Engine::Label(R"(   / __ )____ _/ /_/ /_/ /__ / ___// /_  (_)___ )", "source.ttf", 25, halfW, halfH + 200 - 270, 255 , 255 , 255 , 255, 0.5, 0.5));
	AddNewObject(new Engine::Label(R"(  / __  / __ `/ __/ __/ / _ \\__ \/ __ \/ / __ \)", "source.ttf", 25, halfW, halfH + 200 - 240, 255 , 255 , 255 , 255, 0.5, 0.5));
	AddNewObject(new Engine::Label(R"( / /_/ / /_/ / /_/ /_/ /  __/__/ / / / / / /_/ /)", "source.ttf", 25, halfW, halfH + 200 - 210, 255 , 255 , 255 , 255, 0.5, 0.5));
	AddNewObject(new Engine::Label(R"(/_____/\__,_/\__/\__/_/\___/____/_/ /_/_/ .___/ )", "source.ttf", 25, halfW, halfH + 200 - 180, 255 , 255 , 255 , 255, 0.5, 0.5));
	AddNewObject(new Engine::Label(R"(                                       /_/      )", "source.ttf", 25, halfW, halfH + 200 - 150, 255 , 255 , 255 , 255, 0.5, 0.5));

}
void StartScene::ButtonOnClick(int btnID) {
	// Change to select scene.
	
	if(btnID == BTN_WELCOME)
	    Engine::GameEngine::GetInstance().ChangeScene("play");
	//else if(btnID == BTN_EXIT)
	//  Engine::GameEngine::GetInstance().destroy();
}
