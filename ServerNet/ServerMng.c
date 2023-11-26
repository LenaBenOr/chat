#include "serverDRV.h"
#include "userMng.h"
#include "protocol.h"
#include "ServerMng.h"
#define BUFFER_SIZE 4000 
#define MAX_USERS_CAPACITY 100

struct ServerMng
{
    Server *m_server;
    UserMng *m_userMng;

};

static int MyGotMsg(int _clientId, void* _data, size_t _dataLen, void *_context);
static void RegistrationRply(int _clientId, void *_context, UserInfo* _userInfo);
static void LoginRply(int _clientId, void *_context, UserInfo* _userInfo);

ServerMng *CreateServerMng()
{
    ServerMng *serverMngr;
    if ((serverMngr = (ServerMng*)malloc(sizeof(ServerMng))) == NULL)
    {
        return NULL;
    }
    serverMngr->m_server = createServer(SERVER_PORT, MyGotMsg,serverMngr, NULL, NULL, NULL, NULL);
    /*serverMngr in the create server above is needed in MyGotMsg as a context*/
    if (serverMngr->m_server == NULL)
    {
        free(serverMngr);
        return NULL;
    }
    serverMngr->m_userMng = CreateUserMng(MAX_USERS_CAPACITY);
    if (serverMngr->m_userMng == NULL)
    {
        DestroyServer(&(serverMngr->m_server));
        free(serverMngr);
        return NULL;
    }
    return serverMngr;
}

void DestroyServerMng(ServerMng **_serverMngr)
{
    if (_serverMngr != NULL && *_serverMngr != NULL)
    {
        DestroyServer(&((*_serverMngr)->m_server));
        DestoryUserMng(&((*_serverMngr)->m_userMng));
        free(*_serverMngr);
        *_serverMngr = NULL;
    }
}
void RunServerMngr(ServerMng *_serverMngr)
{
    RunServer(_serverMngr->m_server);
}
/*------------------------------------------------------------------------------------*/
static int MyGotMsg(int _clientId, void* _data, size_t _dataLen, void *_context)
{
    enum Tag tag;
    UserInfo* userInfo;
    if (_data == NULL || _context == NULL)
    {
        return FALSE;
    }
    tag = DetectTag(_data, _dataLen);
    switch (tag)
    {
    case REGISTRATION:
        {
            if (UnpackRegistration(_data, _dataLen, userInfo) == TRUE)
            {
                RegistrationRply(_clientId, _context, userInfo);
            }
            break;
        }
    
    case LOGIN:
        {
            if (UnpackLogin(_data, _dataLen, userInfo) == TRUE)
            {
                LoginRply(_clientId, _context, userInfo);
            }
            break;
        }

    case REPLY:
        
        break;

    case REQUEST_MAKE_ACTIVE:
        
        break;

    case REQUEST_MAKE_NOT_ACTIVE:
        
        break; 

    case UNKNOWN:
        
        break; 

    default:
        break;
    }


    
    return 1;
}

/*The server manager is in the context*/
static void RegistrationRply(int _clientId, void *_context, UserInfo* _userInfo)
{
    char buffer[BUFFER_SIZE];
    enum ERR_TYPES errType;
    int dataLen;
    if ( USERMNG_SUCCESS == AddUserToMng(((ServerMng*)_context)->m_userMng,_clientId ,_userInfo->m_userName, _userInfo->m_password))
    {
        printf("user %s registerd, id = %d\n", _userInfo->m_userName,_clientId);
        printf("user %d name length\n", _userInfo->m_nameLength);
        printf("user %s password\n", _userInfo->m_password);
        printf("user %d password length\n", _userInfo->m_passwordLength);
        errType = SUCCSSES;
    }
    else
    {
        errType = ERR;
    }
    dataLen = PackReply(buffer,BUFFER_SIZE, errType);
    SendMsg(_clientId, buffer, dataLen);
}

static void LoginRply(int _clientId, void *_context, UserInfo* _userInfo)
{
    char buffer[BUFFER_SIZE];
    enum ERR_TYPES errType;
    int dataLen;
    if ( USERMNG_SUCCESS == MakeActive(((ServerMng*)_context)->m_userMng,_clientId, TRUE ,_userInfo->m_userName, _userInfo->m_password))
    {
        errType = SUCCSSES;
    }
    else
    {
        errType = ERR;
    }
    dataLen = PackReply(buffer,BUFFER_SIZE, errType);
    SendMsg(_clientId, buffer, dataLen);
}
