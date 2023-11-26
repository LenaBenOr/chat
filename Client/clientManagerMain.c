#include <stdio.h>
#include "clientManager.h"

int main()
{    
    ClientManger* clientMng = ClientManagerCreate();
    ClientManagerRun();
    ClientManagerDestroy(&clientMng);
    return 0;
}