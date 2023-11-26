#ifndef __USER_H__
#define __USER_H__

#define USERNAME_SIZE 10
#define PASSWORD_SIZE 10
#define TRUE 1
#define FALSE 0


typedef struct User User;

typedef enum UserResult 
{
    USER_SUCCESS,
    USER_NOT_INITIALIZE,
    USER_ALLOCATION_FAILED,
    USER_ACTIVE_FAIL
}UserResult;

/*
Description:
Input:
Output:
Error:
*/
User *CreateUser(int _userId, char *_username, char *_password);

/*
Description:
Input:
Output:
Error:
*/
void DestroyUser(User **_user);

/*
Description:
Input:
Output:
Error:
*/
int GetUserId(User *_user);
char* GetUsername(User *_user);
char* GetUserPassword(User *_user);
int GetUserActive(User *_user);

UserResult SetUserActive(User *_user, int _set,char *_username, char *_password);

/*
Description: commpare btween 2 users thier username 
Input: 2 pointer to users
Output: TRUE if the same else FALSE
Error: FALSE if pointer uninitilaize
*/
int CmpUsers(User *_firstUser, User *_secondUser);

#endif /*__USER_H__*/