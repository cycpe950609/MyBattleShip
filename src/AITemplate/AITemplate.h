#pragma once

#include <BattleShipGame/Wrapper/AI.h>

class AI : public AIInterface
{
public:
    virtual std::vector<std::pair<int,int>> init() override
    {
        return {};
    }

    virtual void callbackReportEnemy(std::vector<std::pair<int,int>>) override
    {

    }

    virtual int queryWhereToHit(TA::Board, int) override
    {
        return 0;
    }

    virtual void callbackReportHit(bool)  override
    {

    }

    virtual std::vector<std::pair<int,int>> queryHowToMoveShip(std::vector<TA::Ship>) override
    {
        return {};
    }
};
