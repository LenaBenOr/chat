#include <stdlib.h> /*malloc*/
#include <stdio.h> /*printf*/
#include <string.h> /*strlen*/
#include "ui.h"
#include "protocol.h"


UserInfo* RegistrationOrLogin()
{
    UserInfo* info;
    info = (UserInfo*)malloc(sizeof(UserInfo));
    printf("Please enter user name:\n");
    scanf("%s", info->m_userName);
    printf("Please enter password:\n");
    scanf("%s", info->m_password);
    info->m_nameLength = strlen(info->m_userName);
    info->m_passwordLength = strlen(info->m_password);
    return info;
}


/*struct UserInfo
{
    char m_userName[NAME_SIZE];
    char m_password[PASS_SIZE];
    int m_nameLength;
    int m_passwordLength;
   
};*/


