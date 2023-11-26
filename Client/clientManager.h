#ifndef __CLIENTMANAGER_H__
#define __CLIENTMANAGER_H__
#define TRUE 1
#define FALSE 0

typedef struct ClientManger ClientManger;

typedef enum ClientMngResult
{
    CLIENT_MNG_SUCCESS,
    CLIENT_MNG_NOT_INITIALIZED,
    CLIENT_MNG_ADD_FAIL,
    CLIENT_MNG_REMOVE_FAIL
}ClientMngResult;

/*
Description: create the client managar
Input: none
Output: pointer to the client managar
Error: return NULL if fail
*/
ClientManger* ClientManagerCreate();

/*
Description: run the client managar
Input:
Output:
Error:
*/
void ClientManagerRun();

/*
Description: destroy the client managar
Input: _clientMng: pointer to pointer to the client managar that need to be destroye
		(must be initialized)
Output: none
Error: none
*/
void ClientManagerDestroy(ClientManger** _clientMng);

#endif /*__CLIENTMANAGER_H__*/