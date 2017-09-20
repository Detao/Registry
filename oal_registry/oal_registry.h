#ifndef _OAL_REGISTRY_H_
#define _OAL_REGISTRY_H_
#include "oal_type.h"

// call once when start up
OAL_RESULT OAL_RegistryInit();

// user save the value of name
OAL_RESULT OAL_RegistrySetStringValue(char *name, char *value);

// user get the value of name
OAL_RESULT OAL_RegistryGetStringValue(char *name, char *value, int valueLen);


#endif


