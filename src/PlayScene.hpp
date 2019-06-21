#ifndef PLAYSCENE_HPP
#define PLAYSCENE_HPP

#include "IScene.hpp"
#include <BattleShipGame/Game.h>

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
	TA::BattleShipGame* _bsgame;
    public:
	PlayScene(TA::BattleShipGame *bsgame);
	void Initialize() override;
	void Update(float deltaTime) override;
	void Draw() const override;
	void OnMouseDown(int button, int mx, int my) override;
	void OnMouseMove(int mx, int my) override;
	void OnMouseUp(int button, int mx, int my) override;
	void OnKeyDown(int keyCode) override;
};


#endif
