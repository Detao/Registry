#include "oal_registry.h"
#include <stdio.h>
int main()
{
    char* name[]={"�������","����Ʒ��","��ɫ",};
    char* value[]={"�¹�666","�µ�77","��ɫ88888"};
    //char name[100],value[100];
    char GValue[100];
    //int i=0;
    OAL_RegistryInit();
    //while(i++<10000)
    //{
       //sprintf(name,"%d--hahhahahhahahahhahah",999);
       //sprintf(value,"%d--hahahhahahahhahahha",5000);

    //}
    for(int i=0;i<3;i++)
    {
       if(OAL_RegistrySetStringValue(name[i],value[i])!=OAL_SUCCESS)
       {
           printf("SetStringValue Faild");
       }
    }
    for(int i=0;i<3;i++)
    {
       if(OAL_RegistryGetStringValue(name[i],GValue,100)!=OAL_SUCCESS)
       {
           printf("GetStringValue Faild");
           return 0;

       }
       printf("name:%s  value:%s\n",name[i],GValue);

    }
    return 0;
}
