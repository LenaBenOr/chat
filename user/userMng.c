#include "userMng.h"
#include "hashmap.h"
#include <stdlib.h> /*malloc*/
#include <string.h> /*strcmp*/
#include <stdio.h> /*for test only*/

struct UserMng
{
    HashMap *m_userDB;
    int m_numOfUsers;
};

typedef struct InfoContext
{
    int m_UserId;
    char *m_username;
    char *m_password;
}InfoContext;

static size_t HashUsersFunction(void* _data);
static int EqualityUsersFunction(void* _firstData, void* _secondData);
static void valUserDestroy(void* _value);
static void KeyUserDestroy(void* _key);
static int CheckInfoActionFunction(const void* _key, void* _value, void* _context);
static int SaveActionFunction(const void* _key, void* _value, void* _context);
static int CheckInfo(UserMng *_userMng ,char *_username, char *_password);


UserMng * CreateUserMng(size_t _maxUsersCapacity)
{
    UserMng *userMng;
    if (_maxUsersCapacity <2)
    {
        return NULL;
    }
    if ((userMng = (UserMng*)malloc(sizeof(UserMng))) == NULL)
    {
        return NULL;
    }
    userMng->m_userDB = HashMap_Create(_maxUsersCapacity, &HashUsersFunction, &EqualityUsersFunction);
    if (userMng->m_userDB == NULL)
    {
        free(userMng);
        return NULL;
    }
    userMng->m_numOfUsers = 0;
    return userMng;
}

void DestoryUserMng( UserMng **_userMng)
{
    if (_userMng != NULL && *_userMng != NULL)
    {
        HashMap_Destroy(&((*_userMng)->m_userDB), &KeyUserDestroy, &valUserDestroy); /*the key is user id dont need to destroy*/
        free(*_userMng);
        *_userMng = NULL;
    }
}

UserMngResault AddUserToMng(UserMng *_userMng,int _clientId ,char *_username, char *_password)
{
    User *newUser;
    int *idKey;
    if (_clientId <= 3)
    { /*client id is the socket that client open*/
        return USERMNG_CLIENT_ID_ERROR;
    }
    idKey = (int*)malloc(sizeof(int));
    if (idKey == NULL || _userMng == NULL || _username == NULL || _password == NULL)
    {
        return USER_NOT_INITIALIZE;
    }
    if (CheckInfo(_userMng, _username, _password) != -1)
    {
        return USERMNG_ADD_DUPLICATE;
    }
    *idKey = _clientId; 
    if ((newUser = CreateUser(_clientId, _username, _password)) == NULL)
    {
        return USERMNG_ADD_FAIL;
    }
    if (MAP_SUCCESS != HashMap_Insert(_userMng->m_userDB ,idKey, newUser))
    {
        return USERMNG_CLIENT_ID_ERROR;
    }
    _userMng->m_numOfUsers ++;
    return USERMNG_SUCCESS;
}

UserMngResault MakeActive(UserMng *_userMng,int _clientId,int _activeSet, char *_username, char *_password)
{ /*need also the usename and the paasword*/
    void *userValue;
    if (_userMng == NULL)
    {
        return USERMNG_NOT_INITIALIZE;
    }
    if ( _clientId <3)
    {
        return USERMNG_CLIENT_ID_ERROR;
    }
    if (MAP_SUCCESS == HashMap_Find(_userMng->m_userDB, &_clientId, &userValue))
    {
        if ( USER_SUCCESS == SetUserActive(userValue,_activeSet, _username, _password))
        {
            return USERMNG_SUCCESS;
        }
    }
    return USERMNG_ACTIVE_FAIL;
}
UserMngResault SaveUserMng(UserMng *_userMng)
{
    FILE * f;
    if ((f = fopen("saveUsers.txt", "w+")) == NULL)
	{
		return USER_SAVE_ERROR;
	}
    fprintf(f, "%d \n" ,_userMng->m_numOfUsers);
    HashMap_ForEach(_userMng->m_userDB, &SaveActionFunction , f);
    
	fclose(f);
    return USERMNG_SUCCESS;
}

UserMngResault LoadUserMng(UserMng *_userMng)
{

}
/*----------------------------------------------static functions here -----------------------------*/
static int CheckInfo(UserMng *_userMng ,char *_username, char *_password)
{
    InfoContext context;
    context.m_UserId = -1; /*default value if didnt found user with the same info*/
    context.m_username = _username;
    context.m_password = _password;
    if (_userMng == NULL || _username == NULL || _password == NULL)
    {
        return -1;
    }
    HashMap_ForEach(_userMng->m_userDB, &CheckInfoActionFunction , &context);
    return context.m_UserId;
}

static size_t HashUsersFunction(void* _key)
{
    return *(int*)_key *2;
}
static int EqualityUsersFunction(void* _firstData, void* _secondData)
{
    return ( GetUserId(_firstData) == GetUserId(_secondData));
}
static void valUserDestroy(void* _value)
{
    User *user;
    if (_value != NULL)
    {
        user = (User*)_value;
        DestroyUser( &user);
    }
}
static void KeyUserDestroy(void* _key)
{
    if (_key != NULL)
    {
        free(_key);
    }
}

static int CheckInfoActionFunction(const void* _key, void* _value, void* _context)
{
    User *tmpUser = CreateUser(0, ((InfoContext*)_context)->m_username, ((InfoContext*)_context)->m_password );
    if ((tmpUser != NULL) && (CmpUsers((User*)_value, tmpUser)))
    {
        ((InfoContext*)_context)->m_UserId = *(int*)_key;
        return 0; /*stor runnimg on the foreach*/
    }
    return 1; 
}
static int SaveActionFunction(const void* _key, void* _value, void* _context)
{
    FILE *f = (FILE*)_context;
    User *u = (User*)_value;
    fprintf(f, "%d %s %s %d\n" ,*(int*)_key, GetUsername(_value), GetUserPassword(_value), GetUserActive(_value)); 
}
/*
int main()
{   
    UserMng *useMng = CreateUserMng(10);
    char username1[10] = "user1";
    char username2[10] = "user2";
    char username3[10] = "user3";
    char password[10] = "genpass";
    
    AddUserToMng (useMng,111, username1, password);
    AddUserToMng (useMng,222, username2, password);
    AddUserToMng (useMng,333, username3, password);
    
    SaveUserMng(useMng);

    DestoryUserMng(&useMng);
  
    return 0;
}
*/
/*---------------------------test function --------------------------------------------------*/
static void TestCheckInfo()
{
    UserMng *useMng = CreateUserMng(10);
    char username1[10] = "user1";
    char username2[10] = "user2";
    char username3[10] = "user3";
    char username4[10] = "user4";
    char password[10] = "gen pass";
    int info;
    
    AddUserToMng (useMng,111, username1, password);
    AddUserToMng (useMng,222, username2, password);
    AddUserToMng (useMng,111, username3, password);
    info = CheckInfo(useMng, username2, password);
    if (info == 222)
    {
        printf("check info correct ---PASS\n");
    }
    info = CheckInfo(useMng, username4, password);
    if (info == -1)
    {
        printf("check info incorrect ---PASS\n");
    }
    DestoryUserMng(&useMng);
}
static void TestAddUserToMng()
{
    UserMng *useMng = CreateUserMng(10);
    char username1[10] = "user1";
    char username2[10] = "user2";
    char username3[10] = "user3";
    char password[10] = "gen pass";
    
    if (USERMNG_SUCCESS == AddUserToMng (useMng,111, username1, password))
    {
        printf("add user 1  ---PASS\n");
    }
    if (USERMNG_SUCCESS == AddUserToMng (useMng,222, username2, password))
    {
        printf("add user 2  ---PASS\n");
    }
    if (USERMNG_SUCCESS != AddUserToMng (useMng,111, username3, password))
    {
        printf("add user 3 same id as 1 ---PASS\n");
    }
    DestoryUserMng(&useMng);
}
static void TestCmpUsers()
{
    User *u1, *u2, *u3;
    char pas[10] = "pass";
    char un1[10] = "aaa";
    char un2[10] = "bbb";
    char un3[10] = "aaa";

    u1 = CreateUser(111, un1, pas);
    u2 = CreateUser(222, un2, pas);
    u3 = CreateUser(333, un3, pas);

    if(CmpUsers(u1,u2))
    {
        printf("user 1 = user 2  ---FAIL\n");
    }
    else
    {
        printf("user 1 != user 2  ---PASS\n");
    }
    if(CmpUsers(u1,u3))
    {
        printf("user 1 = user 3  ---PASS\n");
    }
    else
    {
        printf("user 1 != user 3  ---FAIL\n");
    }
    if(CmpUsers(u1,u1))
    {
        printf("user 1 = user 1  ---PASS\n");
    }
    else
    {
        printf("user 1 != user 1  ---FAIL\n");
    }
    if(CmpUsers(u1,NULL))
    {
        printf("user 1 = NULL  ---PASS\n");
    }
    else
    {
        printf("user 1 != NULL  ---FAIL\n");
    }
    DestroyUser(&u1);
    DestroyUser(&u2);
    DestroyUser(&u3);
}
