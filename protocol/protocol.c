#include "protocol.h"
#include <stdio.h>
#include <string.h>

static int PackLoginOrRegistration(Tag _type, char* _buffer, int _bufLen, const UserInfo* _userInfo);
static int UnpackLoginOrRegistration(Tag _type, const char* _buffer, int _bufLen, UserInfo* _userInfo);

int PackRegistration(char* _buffer, int _bufLen, const UserInfo* _userInfo)
{
    return PackLoginOrRegistration(REGISTRATION, _buffer, _bufLen, _userInfo);
}

int PackLogin(char* _buffer, int _bufLen, const UserInfo* _userInfo)
{
    return PackLoginOrRegistration(LOGIN, _buffer, _bufLen, _userInfo);
}

int UnpackRegistration(const char* _buffer, int _bufLen, UserInfo* _userInfo)
{
    return UnpackLoginOrRegistration(REGISTRATION, _buffer, _bufLen, _userInfo);
}

int UnpackLogin(const char* _buffer, int _bufLen, UserInfo* _userInfo)
{
    return UnpackLoginOrRegistration(LOGIN, _buffer, _bufLen, _userInfo);
}

int PackReply(char* _buffer, int _bufLen, ERR_TYPES _errType)
{
    char *writePtr = _buffer;

    if (!_buffer || (_bufLen < sizeof(byte) + sizeof(byte) + sizeof(byte)))
    {
        return -1;
    }

    *(byte*)writePtr = REPLY; /* insert tag reply into buffer*/
    writePtr += sizeof(byte); /* Tag length*/

    *(byte*)writePtr = sizeof(byte); /* insert err length into buffer*/
    writePtr += sizeof(byte); /* err length*/


    *(byte*)writePtr = _errType; /* insert the errType into buffer*/
    writePtr += sizeof(byte); /* errType length*/

    return writePtr - _buffer;
}

int UnpackReply(const char* _buffer, int _bufLen, ERR_TYPES *_errType)
{
    const char *readPtr = _buffer;

    if (!_buffer || !_errType || (_bufLen < sizeof(byte) + sizeof(byte) + sizeof(byte)))
    {
        return -1;
    }


    if( *(byte*)readPtr != REPLY)
    {
        return FALSE; 
    }
    readPtr += sizeof(byte);  /* increase the readPTR by size of reply*/

    if (*(byte*)readPtr != sizeof(byte))
    {
        return FALSE;
    }

    readPtr += sizeof(byte);  /* increase the readPTR by size of size of length of the _errType*/


    *_errType = *(byte*)readPtr; /* set the errType value in buffer*/
    return TRUE;
}

Tag DetectTag(const char* _buffer, int _lenBuffer)
{
    Tag tag;
    if(_lenBuffer < sizeof(byte))
    {
        return UNKNOWN;
    }

    tag = *(byte*)_buffer;

    if (tag > UNKNOWN)
    {
        tag = UNKNOWN;
    }

    return tag;
}

/*
int main()
{

    UserInfo userInfo, userInfo2; 
    int userId = 100, userId2;
    int bufLen;
    char buffer[200];
    Tag tag;

    memcpy(userInfo.m_userName, "lena", sizeof("lena")); 
    memcpy(userInfo.m_password, "24Lk5", sizeof("24Lk5"));
    userInfo.m_nameLength = sizeof("lena");
    userInfo.m_passwordLength = sizeof("24Lk5");
    
    bufLen = PackLogin(buffer, 200, &userInfo);

    tag = DetectTag(buffer, bufLen);


    switch(tag)
    {
        case LOGIN:
            UnpackLogin(buffer, bufLen, &userInfo2);
            printf(" user name %s\n",userInfo2.m_userName);
            printf(" user password %s\n",userInfo2.m_password);
            printf(" lena length %d\n",userInfo2.m_nameLength);
            printf(" password length %d\n",userInfo2.m_passwordLength);
            break;
        default:
            break;
    }
    
}
*/



static int PackLoginOrRegistration(Tag _type, char* _buffer, int _bufLen, const UserInfo* _userInfo)
{
    char *writePtr = _buffer;
    int msgLen;
    int totalMsgLen;

    if (!_userInfo)
    {
        return -1;
    }

    totalMsgLen = 2 * sizeof(byte) + _userInfo->m_nameLength + _userInfo->m_passwordLength;

    if (_bufLen < sizeof(_type) + totalMsgLen)
    {
        return -1;
    }
    
    *(byte*)writePtr = _type; /* insert tag request into buffer*/
    writePtr += sizeof(byte); /* Tag length*/

 
    *(byte*)writePtr = totalMsgLen; /* insert msg length into buffer*/
    writePtr += sizeof(byte); /* Msg length*/

    
    *(byte*)writePtr = _userInfo->m_nameLength; /* insert user length into buffer*/
    writePtr += sizeof(byte); /* length user name*/

 
    memcpy(writePtr,_userInfo->m_userName, sizeof(char) * _userInfo->m_nameLength); /* sizeof(char) = 1*/ /* insert user name into buffer*/
    writePtr += sizeof(char) * _userInfo->m_nameLength;  /* length user name*/
    
   
    *(byte*)writePtr = _userInfo->m_passwordLength; /* insert password length into buffer*/
    writePtr += sizeof(byte); /* length password*/


    memcpy(writePtr,_userInfo->m_password, sizeof(char) * _userInfo->m_passwordLength); /* sizeof(char) = 1*/ /* insert password into buffer*/
    writePtr += sizeof(char) * _userInfo->m_passwordLength;  /* length user name*/
    
    msgLen =  writePtr - _buffer; 

    return msgLen;
}

static int UnpackLoginOrRegistration(Tag _type, const char* _buffer, int _bufLen, UserInfo* _userInfo)
{
    const char *readPtr = _buffer;
    int totalMsgLen;

    if (!_buffer || !_userInfo)
    {
        return FALSE;
    }
    
    if (_bufLen < sizeof(byte))
    {
        return FALSE;
    }


    /* message type */
    if(*(byte*)readPtr != _type)
    {
        return FALSE; 
    }

    readPtr += sizeof(byte); 

    totalMsgLen = *(byte*)readPtr;  /* size of length of the message*/
    if (_bufLen < totalMsgLen + sizeof(byte))
    {
        return FALSE;
    }
    readPtr += sizeof(byte); 

    _userInfo->m_nameLength = *(byte*)readPtr;  /* set user name length*/
    readPtr += sizeof(byte); /* move the read ptr to user name*/
    
    memcpy(_userInfo->m_userName, readPtr, sizeof(char) * _userInfo->m_nameLength); /* set user name*/
    readPtr += sizeof(char) * _userInfo->m_nameLength; /* move read ptr in size of the user name length*/

    _userInfo->m_passwordLength = *(byte*)readPtr;  /* set user password length*/
    readPtr += sizeof(byte); /* move the read ptr to user name*/

    memcpy(_userInfo->m_password, readPtr, sizeof(char) * _userInfo->m_passwordLength); /* set user password*/
    readPtr += sizeof(char) * _userInfo->m_passwordLength; /* move read ptr in size of the user password length*/

    return TRUE;
}
