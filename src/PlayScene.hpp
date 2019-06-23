#ifndef PLAYSCENE_HPP
#define PLAYSCENE_HPP

#include "IScene.hpp"
#include <BattleShipGame/Game.h>
#include <BattleShipGame/Board.h>
#include <BattleShipGame/Ship.h>
#include <vector>
#include <string>
#include "Slider.hpp"

#define BTN_PLAYPAUSE 0
#define BTN_GOTOEND 1
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
	TA::BattleShipGame *_bsgame;
	void InitOcean(int dx,int dy,int size);
	void InitShip(const std::vector<TA::Ship>& ships,int dx,int dy,std::string color);
	void InitSplitter(int dx,int dy,int size);
	void InitDebug(const TA::Board& board,int dx,int dy);
	void UpdateShip(std::vector<TA::Ship>& ships_red,std::vector<TA::Ship>& ships_blue ,int rx,int ry,int bx,int by);
	//rx : position for left-up point of board of RedShip
	//bx : position for left-up point of board of BlueShip
	bool ShouldUpdateShip(std::vector<TA::Ship>& new_ships,std::vector<TA::Ship>& last_ships);
	void UpdateHitted(TA::Board& board,int dx,int dy,std::string color);
	void AddShip(TA::Ship add_ship,int dx,int dy,std::string color);
	int BlockSize = 40;
	static int Step;//which step is draw in the GameHistory
	int SizeofHistory;
	Slider *sliderHistory;
	std::vector<TA::Ship> last_ships_red;
	std::vector<TA::Ship> last_ships_blue;
	static bool Playing;
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
	void ButtonOnClick(int btnID);

	Engine::Group* Ocean;
	Engine::Group* Ships;
	Engine::Group* Turrents;
	Engine::Group* Hitted;
	Engine::Group* Debug;

};


#endif
