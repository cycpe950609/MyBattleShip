#include <dlfcn.h>

#include <functional>
#include <iostream>
#include <memory>

#include <BattleShipGame/Game.h>
#include <BattleShipGame/Wrapper/Content.h>
#include "GameEngine.hpp"
#include "StartScene.hpp"
#include "PlayScene.hpp"

bool load(const char *libpath, Content &table)
{
    table.handle = dlopen(libpath, RTLD_LAZY);

    if( !table.handle ) {
        std::cout << ".so open:" << dlerror();
        return false;
    }

    bool flag = true;
    auto tryLoad = [&](const char *func)->void*{
        void *func_ptr = dlsym(table.handle, func);
        if( func_ptr == nullptr )
        {
            std::cout << "No sym:" << dlerror() << std::endl;
            flag = false;
            return nullptr;
        }
        return func_ptr;
    };

    table.getai = reinterpret_cast<decltype(table.getai)>(tryLoad("getai"));

    return flag;
}

int main()
{
    Content P1, P2;

    if( !load("./a1.so", P1) ){
        std::cout<<"P1 Fail";
        exit(-1);
    }

    if( !load("./a2.so", P2) ){
        std::cout<<"P2 Fail";
        exit(-1);
    }

    TA::BattleShipGame bgame(20);

    bgame.setPlayer1( (AIInterface*) P1.getai() );
    bgame.setPlayer2( (AIInterface*) P2.getai() );

    Engine::GameEngine& game =  Engine::GameEngine::GetInstance();
    game.AddNewScene("start",new StartScene);
    game.AddNewScene("play",new PlayScene(&bgame));
    Engine::GameEngine::GetInstance().Start("start", 60, 1600, 832);

    //bgame.run();
    dlclose(P1.handle);
    dlclose(P2.handle);
    return 0;
}
