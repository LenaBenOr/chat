#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#define MAX_USER_CAPACITY 1024
#define TRUE 1
#define FALSE 0

#define NAME_SIZE 50
#define PASS_SIZE 50

typedef unsigned char byte;
typedef struct UserInfo UserInfo;

struct UserInfo
{  
    byte m_nameLength;
    byte m_passwordLength;
    char m_userName[NAME_SIZE];
    char m_password[PASS_SIZE];
};

typedef enum Tag 
{
    REGISTRATION,
    LOGIN,
    REPLY,
    REQUEST_MAKE_ACTIVE,
    REQUEST_MAKE_NOT_ACTIVE,


    UNKNOWN

} Tag;


typedef enum ERR_TYPES
{
    SUCCSSES = 0,
    ERR
} ERR_TYPES;

/* Input: 
    Input: 
        _buffer: an input buffer containing a packed message
        _bufLen: the length of the buffer
    Output:
        A tag identifying the message, or UNKNOWN on error
*/
Tag DetectTag(const char* _buffer, int _lenBuffer);

/*
    Input: 
        _buffer: an output buffer that will contain the packed message
        _bufLen: the length of the buffer
        _userInfo: Registration info to pack
    Output:
        Return Value: the length of the data written to the buffer, or -1 on error.
        _buffer: contains the packed message on success
*/
int PackRegistration(char* _buffer, int _bufLen, const UserInfo* _userInfo);

/*
    Input: 
        _buffer: an output buffer that will contain the packed message
        _bufLen: the length of the buffer
        _userInfo: Login info to pack
    Output:
        Return Value: the length of the data written to the buffer, or -1 on error.
        _buffer: contains the packed message on success
*/
int PackLogin(char* _buffer, int _bufLen, const UserInfo* _userInfo);

/*
    Input: 
        _buffer: an input buffer containing a packed message
        _bufLen: the length of the buffer
        _userInfo: Registration info will be unpacked into this struct
    Output:
        Return Value: TRUE on success, FALSE on error.
        _userInfo: contains the user information on success
*/
int UnpackRegistration(const char* _buffer, int _bufLen, UserInfo* _userInfo);

/*
    Input: 
        _buffer: an input buffer containing a packed message
        _bufLen: the length of the buffer
        _userInfo: Login info will be unpacked into this struct
    Output:
        Return Value: TRUE on success, FALSE on error.
        _userInfo: contains the user information on success
*/
int UnpackLogin(const char* _buffer, int _bufLen, UserInfo* _userInfo);

/*
    Input: 
        _buffer: an output buffer that will contain the packed message
        _bufLen: the length of the buffer
        _errType: Error/Succes info to pack
    Output:
        Return Value: the length of the data written to the buffer, or -1 on error.
        _buffer: contains the packed message on success
*/
int PackReply(char* _buffer, int _bufLen, ERR_TYPES _errType);

/*
    Input: 
        _buffer: an input buffer containing a packed message
        _bufLen: the length of the buffer
        _errType: Error/Success info will be unpacked into this parameter
    Output:
        Return Value: TRUE on success, FALSE on error.
        _errType: contains the user information on success
*/
int UnpackReply(const char* _buffer, int _bufLen, ERR_TYPES *_errType);

#endif /*__PROTOCOL_H__*/