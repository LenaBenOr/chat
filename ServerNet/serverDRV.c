#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include "serverDRV.h"

#define DATA_SIZE 1024
#define BACK_LOG 1024
#define MAX_FD 1024
#define MAX_CLIENTS 1000
#define TRUE 1
#define FAIL -1 


struct Server 
{
    int                 m_sockMaster; 
    List*               m_clientList;
    int                 m_clientCounter;
    GotMsg              m_gotMsg;
    NewClient           m_newCient;
    CloseClient         m_closeClient;
    FailClient          m_failClient;
    FlagFunc            m_flagFunc;
    void*               m_context;
    fd_set              m_readfds;
    int                 m_stopFlag;
};


static void SocketDestroy(void* _item);
static int InitializeServer(uint16_t _port);
static int HandleClient(void* _element, void* _context);
static ErrServer CheckCurrentClients(Server* _server);
static void CheckNewClients(Server *_server);
static int ServerAccept(Server* _server);
static int SendData(int _sock, char *_data, int _dataLen);
static int RecieveData(Server*, int _sock, char *_buffer, int _dataLen);
static void TerminateServer(Server* _server);
static int InitializeFDSET(Server* _server);
static int AddSocket(void* _element, void* _context);
static void PrintList(List* _list);


/*************************************************************************************************************/
Server* createServer(uint16_t _port, GotMsg _gotMsg,void *_context,
                     NewClient _newCient, CloseClient _closeClient, FailClient _failClient,
                     FlagFunc _flagFunc)
{
    Server* server; 

    if(_gotMsg == NULL)
    {
        return NULL; 
    }
    server = (Server*)calloc(1, sizeof(Server));
    if(server == NULL)
    {
        return NULL;
    }

    server->m_clientList = ListCreate();
    if (!server->m_clientList)
    {
        free(server);
        return NULL;
    }

    server->m_sockMaster = InitializeServer(_port);
    if (!server->m_sockMaster)
    {
        ListDestroy(&server->m_clientList, NULL);
        free(server);
    }


    server->m_gotMsg = _gotMsg;
    server->m_context = _context;
    server->m_newCient = _newCient;
    server->m_closeClient = _closeClient;
    server->m_failClient =_failClient;
    server->m_flagFunc = _flagFunc;
    server->m_stopFlag = 0;
    
    return server;
}

/************************************************************************************************************************/
void DestroyServer(Server** _server)
{
    if(_server == NULL || *_server == NULL)
    {
        return;
    }

    ListDestroy(&(*_server)->m_clientList, SocketDestroy);
    close((*_server)->m_sockMaster);

    free(*_server);
    *_server = NULL;
    return;
}

/******************************************************************************************************************/
/*
move this function to server manager */


ErrServer RunServer(Server* _server)
{
  
    int     nfds;
    int     activity;

    if(_server == NULL)
    {
        return SERVER_NOT_INITIALIZED;
    }
 
    while(!_server->m_stopFlag)
    {
        nfds = InitializeFDSET(_server);

        activity = select(nfds + 1, &_server->m_readfds, NULL, NULL, NULL);
        if ((activity < 0) && (errno != EINTR))
        {
            perror("select error");
            return SERVER_RUN_FAILED;
        }

        CheckNewClients(_server);
        CheckCurrentClients(_server);

   }
   return SERVER_SUCCESS;
}


/***************************************************************************************************/
int SendMsg(int _sock, char *_msg, int _msgLen)
{
    int sent_bytes;

    
    if(_msg == NULL || _msgLen <= 0)
    {
        return -1;
    }

    sent_bytes = send(_sock, _msg, _msgLen, 0);
    if (sent_bytes < 0) 
    {
        perror("send failed");
    }
    return sent_bytes;
}

/***************************************************************************************************/
void StopRun(Server* _server)
{
    if(_server == NULL)
    {
        return; 
    }
    _server->m_stopFlag = 1;
}

/***************************************************************************************************/

int FlagFunction()
{
    return 1; 
}

/***************************************************************************************************/
/*Static Functions*/
/***************************************************************************************************/
static int InitializeServer(uint16_t _port)
{  
    int activity;
    int new_socket;
    int flags; 
    int optval = 1;
    struct sockaddr_in sin;
    

    int sock = socket(AF_INET, SOCK_STREAM, 0); /* open channel */

    if (sock < 0) 
    {
        perror("socket() failed");
        return 0;
    }

    memset(&sin, 0 ,sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(SERVER_ADDR); /* fix??????????? casting warnnig here !!!!!!!*/
    sin.sin_port = htons(_port);

    /*optional, the server(bind) can say the port is busy, not waiting, after socket before bind */
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) /* reuse address - SO_REUSEADDR; before use optval = 1*/
    {
        perror("reuse failed");
        return 0;
    }

    if (bind(sock, (struct sockaddr *) &sin, sizeof(sin)) < 0) 
    {
        perror("bind failed");
        return 0;
    }

    /*the client needs to tell us he wants to send a message */
    /*we need one socket needs to listen to requests from client, there will be a que for all the client request*/
    /*just for server OS and TCP */
    /*
        back_log - queue size (few houndreds to 1024, now 1 is enough the bigger the queue wastes OS recourses)
    */
    if (listen(sock, BACK_LOG) < 0) 
    {
        perror("listen failed");
        return 0;
    }

    return sock;
}


static int ServerAccept(Server* _server)
{
    /*readfds tells us which sockets are pending */

    /* the same listening_sock we opened in the begining, waits for connect from the client*/

    /* new socket is established and it lives untill the connection is lasting, when the connection is done, the socket closes.*/
    int flags;
    char buffer[4096];
    struct sockaddr_in client_sin;
    unsigned int addr_len = sizeof(client_sin);
    int client_sock;
    
    /*If something happened on the master socket , then its an incoming connection*/

    if (FD_ISSET(_server->m_sockMaster, &_server->m_readfds))
    {
        client_sock = accept(_server->m_sockMaster,
                            (struct sockaddr *) &client_sin,
                            &addr_len);

        if (client_sock < 0) 
        {
            return -1;
        }
    } 
    else
    {
        return -1;
    }

    return client_sock;
}

static int SendData(int _sock, char *_data, int _dataLen)
{
    int sent_bytes = send(_sock, _data, _dataLen, 0);

    if (sent_bytes < 0) 
    {
        perror("send failed");
    }
    return sent_bytes;
}

static int RecieveData(Server* _server, int _sock, char *_buffer, int _dataLen)
{
    int read_bytes;
    
    if (!FD_ISSET(_sock, &_server->m_readfds))
    {
        return 0;
    }
    
    read_bytes = recv(_sock, _buffer, _dataLen, 0); /* instead recieve meed to put GotMsg*/
   
    if (read_bytes == 0)
    {
        /* connection is closed */
        return -1;
    } 
    else if (read_bytes < 0) 
    {
        if (errno != EAGAIN && errno != EWOULDBLOCK)
        {
            /*error*/
            perror("recv failed");
            return -1;
        }
        /* All ok, just no data right now */
        return 0;
    } 
    else 
    {
        (_server->m_gotMsg)(_sock,_buffer,read_bytes,_server->m_context);
        /*OK. But no guarantee read_bytes == expected_data_len*/
        return read_bytes;
    }
}

static void TerminateServer(Server* _server)
{
    close(_server->m_sockMaster);
}

/*struct _context 
{
    struct sockaddr_in* sin 
    int *sock
    List* clientList;

}*/ 

static int HandleClient(void* _element, void* _context)
{
    Server* server = (Server *)_context;
    int sock = *(int*)_element;
    int data_len;
    char buf[DATA_SIZE]; /*needs to be void * */

    data_len = RecieveData(server, sock, buf, DATA_SIZE - 1);

    if (data_len < 0)
    {
        return 0;
    }

    if (data_len == 0)
    {
        return 1;
    }

    buf[data_len] = 0;
    printf("Recieved from %d: %s\n", sock, buf);

    if (server->m_gotMsg)
    {
        server->m_gotMsg(sock, buf, data_len, server->m_context);
    }
  
}

static void PrintList(List* _list)
{
	ListItr iter;

	if(_list == NULL)
	{
		return;
	}
	for (iter = ListItrBegin(_list); iter != ListItrEnd(_list); iter = ListItrNext(iter))
	{
		printf("%d, ", *(int *)ListItrGet(iter));
	}
    printf("\n");
}

static void CheckNewClients(Server *_server)
{
    int clientSock;
    void *sockToPush;
    if ((clientSock = ServerAccept(_server)) > 0)
    {
        if(_server->m_clientCounter >= MAX_CLIENTS)
        {
            if (_server->m_failClient)
            {
                _server->m_failClient(clientSock, _server->m_context);
            }
            close(clientSock);
            printf("server is full\n");
            return;
        }
        
        _server->m_clientCounter++;
        sockToPush = (int*)malloc(sizeof(int));
        *(int *)sockToPush = clientSock;
        printf("Client %d connected\n", clientSock);
        ListPushHead(_server->m_clientList, sockToPush);
    }
    return;
}


/*talks to clients, has list of all the open sockets*/
static ErrServer CheckCurrentClients(Server* _server)
{
    ListItr iter;
    void * tempSock;

    iter = ListItrForEach(ListItrBegin(_server->m_clientList), ListItrEnd(_server->m_clientList), HandleClient, _server);

    if (iter != ListItrEnd(_server->m_clientList))
    {
        tempSock = ListItrRemove(iter);
        printf("Client %d disconnected\n", *(int *)tempSock);
        close(*(int *)tempSock);
        _server->m_clientCounter--; 
        printf("Connected Clients: %d\n", _server->m_clientCounter);
        free(tempSock);
    }

    return SERVER_SUCCESS;
}


typedef struct
{
    fd_set  *m_readfds;
    int      m_maxfd;
} SocketContext;


/* all the socket that select listens to*/
static int AddSocket(void* _element, void* _context)
{
    SocketContext *context = (SocketContext *)_context;
    int socket = *(int *)_element;
    
    if (socket > context->m_maxfd)
    {
        context->m_maxfd = socket;
    }

    FD_SET(socket, context->m_readfds);

    return 1;
}

/* 
    fills the fdset and returns the maximum fd
*/
static int InitializeFDSET(Server* _server)
{
    SocketContext context;
    context.m_maxfd = _server->m_sockMaster;
    context.m_readfds = &_server->m_readfds;

    FD_ZERO(&_server->m_readfds);
    FD_SET(_server->m_sockMaster, &_server->m_readfds);

    ListItrForEach(ListItrBegin(_server->m_clientList), ListItrEnd(_server->m_clientList), AddSocket, &context);

    return context.m_maxfd;
}


static void SocketDestroy(void* _item)
{
    int sock = *(int *)_item;
    close(sock);
    free(_item);
}
/*
int MyGotMsg(int _clientId, void* _data, size_t _dataLen, void *_context)
{
    printf("Got Msg: %.*s\n", (int)_dataLen, (char *)_data);
    SendMsg(_clientId, _data, _dataLen);
}

int main()
{
    Server *server;

    server = createServer(inet_addr(SERVER_ADDR), SERVER_PORT, MyGotMsg, NULL, NULL, NULL, NULL);
    if (!server)
    {
        printf("Failed to create server!");
        return 1;
    }

    RunServer(server, NULL);

    DestroyServer(server);
    
    return 0;
}
*/

