#pragma once

#include <BattleShipGame/Board.h>
#include <BattleShipGame/Ship.h>

#include <vector>
#include <utility>

#define AI_ABI_VER 20190422

class AIInterface
{
    int a;
public:
    AIInterface() = default;
    virtual ~AIInterface() = default;

    int abi(){ return AI_ABI_VER; }

    /*
        pass game information in the beginning
        it should return how to put you ships
    */

    virtual std::vector<std::pair<int,int>> init() = 0;
    /*
        for each each round, the host will call callbackEnemyReport to pass where the enemy hit
        first player in first round, vector size will be zero
    */
    virtual void callbackReportEnemy(std::vector<std::pair<int,int>>) = 0;

    /*
        for each ship, if the ship state is ShipState::Available,
        The host will use queryHowToMoveShip(Ship) to query how to move
    */
    virtual int queryWhereToHit(TA::Board, int) = 0;

    /*
        it's be called after queryWhereToHit
        report the state of the enemy
    */
    virtual void callbackReportHit(bool) = 0;

    /*
        for each ship, if the ship state is ShipState::Available,
        The host will use queryHowToMoveShip(Ship) to query how to move

        this function should return (x, y) using std::pair

        return : new position of the ship
    */
    virtual std::vector<std::pair<int,int>> queryHowToMoveShip(std::vector<TA::Ship>) = 0;
};
