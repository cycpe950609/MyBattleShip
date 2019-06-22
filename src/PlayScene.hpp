#ifndef PLAYSCENE_HPP
#define PLAYSCENE_HPP

#include "IScene.hpp"
#include <BattleShipGame/Game.h>
#include <BattleShipGame/Board.h>
#include <BattleShipGame/Ship.h>
#include <vector>
#include <string>
#include "Slider.hpp"

class PlayScene final : public Engine::IScene
{
    private:
	enum TileType
	{
	    BOAT_SIDE,
	    BOAT_CENTER,
	    BOAT_CORNER,
	    WATER,
	    SPLITTER
	};
	enum TileMode
	{
	    HITTED,
	    NO_THING
	};
	static bool DebugMode;
	TA::BattleShipGame *_bsgame;
	void DrawOcean(int dx,int dy,int size);
	void DrawShip(const std::vector<TA::Ship>& ships,int dx,int dy,std::string color);
	void DrawSplitter(int dx,int dy,int size);
	int BlockSize = 40;
	int Step = 0;//which step is draw in the GameHistory
	int SizeofHistory;
	Slider *sliderHistory;
    public:
	PlayScene(TA::BattleShipGame *bsgame);
	void Initialize() override;
	void Update(float deltaTime) override;
	void Draw() const override;
	void OnMouseDown(int button, int mx, int my) override;
	void OnMouseMove(int mx, int my) override;
	void OnMouseUp(int button, int mx, int my) override;
	void OnKeyDown(int keyCode) override;
	void HistorySlideOnValueChanged(float value);
	
	Engine::Group* Ocean;
	Engine::Group* Ships;
	Engine::Group* Hitted;

};


#endif
