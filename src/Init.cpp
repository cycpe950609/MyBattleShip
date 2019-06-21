// [main.cpp]
// this template is provided for the 2D tower defense game.
// Program entry point.
// Returns program exit code.
#include "GameEngine.hpp"
#include "LOG.hpp"
#include "StartScene.hpp"
#include "Init.hpp"
#include <iostream>

void InitGame() {
	Engine::LOG::SetConfig(true);
	Engine::GameEngine& game = Engine::GameEngine::GetInstance();
	game.AddNewScene("start",new StartScene());
	// TODO 1 (1/2): Add a New Scene here and change the start scene below.
	//game.Start("start", 60, 1600, 832);
	std::cout << "TestTest" << std::endl;
}
