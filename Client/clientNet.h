#ifndef __CLIENTNET_H__
#define __CLIENTNET_H__

typedef struct ClientNet ClientNet;

typedef enum ClientNet_Result {
	CLIENT_NET_SUCCESS,
	CLIENT_NET_NOT_INITIALIZED
	
} ClientNet_Result;

/*
Description: create a pointer the client 
Input: none
Output: pointer to the client
Error: null if fail
*/
ClientNet* CreateClient();

/*
Description: sending message from the client
Input:  _clientNet: pointer to the client, 
		_data: pointer to the data that need to be tranfered
		_dataLen: the data length
Output: none
Error: none
*/
void SendingDataClient(ClientNet* _clientNet, char* _data, int _dataLen);

/*
Description: receive message from the server
Input:  _clientNet: pointer to the client
		_buffer: pointer to the buffer to hold the data from the server
		_dataLen: the buffer length
Output: none
Error: none
*/
void ReceiveDataClient(ClientNet* _clientNet, char* _buffer, int _BuffLen);

/*
Description: destroy the client
Input: _clientNet: pointer to pointer to the client that need to be destroye
		(must be initialized)
Output: none
Error: none
*/
void DestroyClient(ClientNet** _clientNet);

#endif /*__CLIENTNET_H__*/