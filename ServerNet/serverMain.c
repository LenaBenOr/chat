#include <stdio.h>
#include "ServerMng.h"


int main()
{
    ServerMng *serverMngr = CreateServerMng();
    if (!serverMngr)
    {
        printf("create server manager failed\n");
        return 0;
    }
    RunServerMngr(serverMngr);
    DestroyServerMng(&serverMngr);
    return 0;
}

