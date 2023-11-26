#include <stdio.h>
#include <stdlib.h> /*malloc, abort*/
#include "clientManager.h"
#include "clientNet.h"
#include "ui.h"
#include "protocol.h"
#include "list.h"
#define BUFF_SIZE 4000

struct ClientManger
{
    List* m_client; /*info about the client groups?*/
    void* m_context;
};
static void GoToMenue2();
/**********************************************************************************************/
ClientManger* ClientManagerCreate()
{
    ClientManger* clientMng;
    clientMng = (ClientManger*)malloc(sizeof(ClientManger));
    if(clientMng == NULL)
    {
        return NULL;
    }
    clientMng->m_client = ListCreate();
    if(clientMng->m_client == NULL)
    {
        free(clientMng);
        return NULL;
    }
    return clientMng;
}
/**********************************************************************************************/
void ClientManagerRun()
{
    UserInfo* info ;
    int num, fExt = FALSE, dataLen; 
    char buffer[BUFF_SIZE];
    ClientNet* clientNet = CreateClient();
    enum ERR_TYPES errType;
    while(fExt != TRUE)
    {
        printf("Welcome!\nPlease choose a number:\n\n1. Registration\n2. Log in\n3. Exit\n");
        scanf("%d", &num);
        switch (num)
        {
            case REGISTRATION_UI:
            {
                info = RegistrationOrLogin();
                dataLen = PackRegistration(buffer, BUFF_SIZE, info);
                SendingDataClient(clientNet, buffer, dataLen);
                ReceiveDataClient(clientNet, buffer, BUFF_SIZE);
                UnpackReply(buffer, BUFF_SIZE, &errType);
                if(errType == ERR)
                {
                    printf("registration failed, try other user name\n");
                }
                else
                {
                    printf("registration successed!\n");
                }
                break;
            }
            case LOGIN_UI:
            {
                info = RegistrationOrLogin();
                dataLen = PackLogin(buffer, BUFF_SIZE, info);
                SendingDataClient(clientNet, buffer, dataLen);
                ReceiveDataClient(clientNet, buffer, BUFF_SIZE);
                UnpackReply(buffer, BUFF_SIZE, &errType);
                if(errType == ERR)
                {
                    printf("login failed, incorrect user name or password\n");
                    break;
                }
                GoToMenue2(clientNet);
                break;
            }
            case EXIT_UI:
            {
                fExt = TRUE;
                break;
            }                
            default:
            {
                printf("Wrong number, try again\n");
                break;
            }
        }
    }
}
/**********************************************************************************************/
void ClientManagerDestroy(ClientManger** _clientMng)
{
    if(_clientMng != NULL && *_clientMng != NULL)
    {
        ListDestroy(&(*_clientMng)->m_client, NULL);
        free(*_clientMng);
        *_clientMng = NULL;
    }
}
/**********************************************************************************************/
static void GoToMenue2()
{
    int num, fExt = FALSE; 
    while(fExt != TRUE)
    {
        printf("Please choose a number:\n\n1. Join group\n2. Create group\n3. Leave group\n4. Log out\n");
        scanf("%d", &num);
        switch (num)
        {
            case JOIN_GROUP:
            {
                
                break;
            }
            case CREATE_GROUP:
            {
                
                break;
            }
            case LEAVE_GROUP:
            {
                
                break;
            }
            case LOG_OUT:
            {
                fExt = TRUE;
                break;
            }                
            default:
            {
                printf("Wrong number, try again\n");
                break;
            }
        }
    }
}