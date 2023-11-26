#ifndef __SERVERMNG_H_
#define __SERVERMNG_H_

typedef struct ServerMng ServerMng;


/*
Description:
Input:
Output:
Error:
*/
ServerMng *CreateServerMng();


/*
Description:
Input:
Output:
Error:
*/
void DestroyServerMng(ServerMng **_serverMngr);


void RunServerMngr(ServerMng *_serverMngr);


#endif /*__SERVERMNG_H_*/

