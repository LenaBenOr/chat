#ifndef __UI_H__
#define __UI_H__
#include "protocol.h"

typedef enum Menu1
{
    REGISTRATION_UI = 1,
    LOGIN_UI = 2,
    EXIT_UI = 3
}Menu1;

typedef enum Menu2
{
    JOIN_GROUP = 1,
    CREATE_GROUP = 2,
    LEAVE_GROUP = 3,
    LOG_OUT = 4
}Menu2;

UserInfo* RegistrationOrLogin();

#endif /*__UI_H__*/