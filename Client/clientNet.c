#include <stdio.h>
#include <stdlib.h> /*malloc, abort*/
#include <sys/socket.h> /*AF_INET*/
#include <string.h> /*memset*/
#include <netinet/in.h> /*INADDR_ANY, sin*/
#include "clientNet.h"
#define SERVER_PORT 1030
#define DATA_SIZE 100

struct ClientNet
{
    struct sockaddr_in m_sin;
    int m_sock;
};
/****************************************************************************************************/
ClientNet* CreateClient()
{    
    ClientNet* clientNet;
    clientNet = (ClientNet*)malloc(sizeof(ClientNet));
    if(clientNet == NULL)
    {
        return NULL;
    }
    clientNet->m_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (clientNet->m_sock < 0) 
    {
        perror("socket() failed");
        abort();
    }
    memset(&clientNet->m_sin, 0, sizeof(clientNet->m_sin));
    clientNet->m_sin.sin_family = AF_INET;
    clientNet->m_sin.sin_addr.s_addr = inet_addr("127.0.0.1");
    clientNet->m_sin.sin_port = htons(SERVER_PORT);

    if (connect(clientNet->m_sock, (struct sockaddr *) &clientNet->m_sin, sizeof(clientNet->m_sin)) < 0) 
    {
        perror("connection failed");
        abort();
    }
    return clientNet;
}
/****************************************************************************************************/
void SendingDataClient(ClientNet* _clientNet, char* _data, int _dataLen) /*void* convert*/
{
    int sent_bytes = send(_clientNet->m_sock, _data, _dataLen, 0);
    if (sent_bytes < 0) 
    {
        close(_clientNet->m_sock);
        perror("send failed");
    }
    
    printf("msg sent from client\n");
}
/****************************************************************************************************/
void ReceiveDataClient(ClientNet* _clientNet, char* _buffer, int _BuffLen)
{
    int read_bytes = recv(_clientNet->m_sock, _buffer, _BuffLen, 0);

    if (read_bytes == 0) 
    {
        close(_clientNet->m_sock); /*connection is closed*/
    } 
    else if (read_bytes < 0) /*error*/
    { 
        close(_clientNet->m_sock);
        perror("recv failed");
    }
    else 
    {
        
        _buffer[read_bytes] = '\0';
        printf("msg from server is: %s\n", _buffer);
        /* OK. But no guarantee read_bytes == expected_data_len*/
    }
}
/****************************************************************************************************/
void DestroyClient(ClientNet** _clientNet)
{
    if(_clientNet != NULL && *_clientNet != NULL)
    {
        close((*_clientNet)->m_sock);
        free(*_clientNet);
        *_clientNet = NULL;
    }
}
/****************************************************************************************************/
int GetSocket(ClientNet* _clientNet)
{
    return _clientNet->m_sock;
}
/*int main()
{
    char data[DATA_SIZE] = "hello from client\n";
    char buffer[DATA_SIZE];
    ClientNet* clientNet = CreateClient();
    SendingDataClient(clientNet, data, strlen(data));
    ReceiveDataClient(clientNet, buffer, DATA_SIZE);
    DestroyClient(&clientNet);
    /*int exit_status = system("gnome-terminal");
    return 0;
}*/