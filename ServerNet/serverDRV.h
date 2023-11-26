#ifndef __SERVERDRV_H__
#define __SERVERDRV_H__

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <errno.h>

#include "list.h"
#include "list_functions.h"
#include "list_itr.h"
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 1030
#define DATA_SIZE 1024
#define BACK_LOG 1024
#define MAX_FD 1024
#define MAX_CLIENTS 1000
#define TRUE 1
#define FAIL -1 



typedef struct Server Server;

typedef enum ErrServer{
    SERVER_SUCCESS = 0,
    SERVER_NOT_INITIALIZED,
    SERVER_ALLOCATION_FAILED,
    SERVER_RUN_FAILED
} ErrServer;

typedef int (*GotMsg)(int _clientId, void* _data, size_t _dataLen, void *_context);
typedef int (*NewClient)(int _clientId, void* _context);
typedef int (*CloseClient)(int _clientId, void* _context);
typedef int (*FailClient)(int _clientId, void* _context);
typedef int (*FlagFunc)(void* _context);


Server* createServer(uint16_t _port, GotMsg _gotMsg, void *_context,
                     NewClient _newCient, CloseClient _closeClient, FailClient _failClient,
                     FlagFunc _flagFunc);

void DestroyServer(Server** _server);


ErrServer RunServer(Server* _server);


int SendMsg(int _sock, char *_msg, int _msgLen);


void StopRun(Server* _server);


int FlagFunction();

#endif /*__SERVERDRV_H__*/