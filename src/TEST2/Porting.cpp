#include <BattleShipGame/Wrapper/Porting.h>
#include "AITemplate.h"

void* getai()
{
    AIInterface *ptr = new AI();
    return ptr;
}
