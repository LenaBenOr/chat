#include "user.h"
#include <stdlib.h> /*malloc*/
#include <string.h> /*strncpy*/


struct User
{
    int m_userId;
    char m_username[USERNAME_SIZE];
    char m_password[PASSWORD_SIZE];
    int m_active;  
};


User *CreateUser(int _userId, char *_username, char *_password)
{
    User *user;
    if (_username == NULL || _password == NULL)
    {
        return NULL;
    }
    if ((user = (User*)malloc(sizeof(User))) == NULL)
    {
        return NULL;
    }
    user->m_userId = _userId;
    strncpy(user->m_username ,_username, USERNAME_SIZE);
    strncpy(user->m_password, _password, PASSWORD_SIZE);
    user->m_active = FALSE;
    return user;
}

void DestroyUser(User **_user)
{
    if (_user != NULL && *_user != NULL)
    {
        free(*_user);
        *_user = NULL;
    }
}

int GetUserId(User *_user)
{
    if (_user == NULL)
    {
        return -1;
    }
    return _user->m_userId;
}
char* GetUsername(User *_user)
{
    if (_user == NULL)
    {
        return "";
    }
    return _user->m_username;
}
char* GetUserPassword(User *_user)
{
    if (_user == NULL)
    {
        return "";
    }
    return _user->m_password;
}
int GetUserActive(User *_user)
{
    if (_user == NULL)
    {
        return -1;
    }
    return _user->m_active;
}
UserResult SetUserActive(User *_user, int _set,char *_username, char *_password)
{
    /*get username and passwrod and only if matchs do set*/
    if (_user == NULL || _username == NULL || _password == NULL)
    {
        return USER_ACTIVE_FAIL;
    }
    if (_user->m_username == _username && _user->m_password == _password 
        && _user->m_active != _set)
    {
        _user->m_active = _set;
        return USER_SUCCESS;
    }
    return USER_ACTIVE_FAIL;
}
int CmpUsers(User *_firstUser, User *_secondUser)
{
    if (_firstUser == NULL || _secondUser == NULL)
    {
        return FALSE;
    }
    if (strcmp(_firstUser->m_username, _secondUser->m_username) == 0)
    {
        return TRUE;
    }
    return FALSE;
}

