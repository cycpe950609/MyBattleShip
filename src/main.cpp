#include <dlfcn.h>

#include <functional>
#include <iostream>
#include <memory>

#include <BattleShipGame/Wrapper/Implement.h>

bool load(const char *libpath, Content &table)
{
    table.handle = dlopen(libpath, RTLD_LAZY);

    if( !table.handle ) {
        std::cout << ".so open:"<< dlerror();
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

    table.sum = reinterpret_cast<decltype(table.sum)>(tryLoad("sum"));

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

    std::cout<< P1.sum() << std::endl;
    std::cout<< P2.sum() << std::endl;

    /*std::future_status status;
    std::future<int> val = std::async(std::launch::async, sum, 1);
    status = val.wait_for(std::chrono::milliseconds(1000));

    if( status != std::future_status::ready )
    {
        exit(-1);
    }

    auto rval = val.get();*/

    dlclose(P1.handle);
    dlclose(P2.handle);
    return 0;
}
