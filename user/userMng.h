#ifndef __USERMNG_H__
#define __USERMNG_H__
#include <stddef.h> /*size_t*/
#include "user.h"

typedef struct UserMng UserMng;

typedef enum UserMngResault
{
 USERMNG_SUCCESS,
 USERMNG_NOT_INITIALIZE,
 USERMNG_ADD_FAIL,
 USERMNG_ADD_DUPLICATE,
 USERMNG_CLIENT_ID_ERROR,
 USERMNG_ACTIVE_FAIL,
 USER_SAVE_ERROR,
 USER_LOAD_ERROR  
}UserMngResault;


/*
Description: Create Users Management
Input: max amount of users to manage >1
Output: pointer to users management 
Error: NULL if fail
*/
UserMng * CreateUserMng(size_t _maxUsersCapacity);

/*
Description:
Input:
Output:
Error:
*/
void DestoryUserMng( UserMng **_userMng);

/*
Description:
Input:
Output:
Error:
*/
UserMngResault AddUserToMng(UserMng *_userMng,int _clientId ,char *_username, char *_password);

/*
Description:
Input:
Output:
Error:
*/
UserMngResault MakeActive(UserMng *_userMng,int _clientId,int _activeSet,char *_username, char *_password);

/*
Description:
Input:
Output:
Error:
*/
UserMngResault SaveUserMng(UserMng *_userMng);

/*
Description:
Input:
Output:
Error:
*/
UserMngResault LoadUserMng(UserMng *_userMng);


#endif /*__USERMNG_H__*/

