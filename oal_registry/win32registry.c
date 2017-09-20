#include "oal_registry.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>


#define ROOT_END         "</root>"
#define VALUE_END        "</value>"
#define VALUE_START      "<value>"
#define ROOT_END_SIZE    7
#define VALUE_END_SIZE   8
#define VALUE_START_SZIE 7
#define TAG_SIZE         44      //The size of a set of tags
#define NAMETOVALUE_SIZE 15      //</name>\n<value>
#define File             "Registry.xml"

//write file form head
static OAL_RESULT Fprint(char* str,FILE *fp)
{
    if((!str))
    {
        return OAL_FAILED;
    }
    fprintf(fp,"%s",str);
    fflush(fp);
    return OAL_SUCCESS;
}
//read one piece
static OAL_RESULT Fread(char* ptr, FILE *fp)
{
    if(!ptr)
    {
        return OAL_FAILED;
    }
    while(fread(ptr,1,1,fp))
    {
        ptr+=1;
        if(memcmp(ptr-ROOT_END_SIZE,ROOT_END,ROOT_END_SIZE) == 0)
          {
             return OAL_SUCCESS;
          }
    }
    return OAL_FAILED;
}
OAL_RESULT OAL_RegistryInit()
{
     FILE *fp=fopen(File,"r");
    if(fp){
            fclose(fp);
            return OAL_SUCCESS;
    }
    else{
            fp=fopen(File,"w");
         if(!fp)
         {
             return OAL_FAILED;
         }
         Fprint("<?xml version='1.0' encoding=\"GBK\"?>\n",fp);
         fclose(fp);
        }
    return OAL_SUCCESS;
}
OAL_RESULT OAL_RegistrySetStringValue(char *name, char *value)
{
     OAL_S32 Buffersize            //buffer size
             ,Filecount            //file   size
             ,Fvaluecount=0;       //file value size
     OAL_S32 Valuecount;           //The incoming value size
     OAL_S32 Namecount;            // The incoming name szie
     OAL_S8 *FileBuffer,GetBuffer[100];
     OAL_S8 *ValueStart,*ValueEnd;
    if((!name)||(!value))
    {
        return OAL_FAILED;
    }
    FILE *fp=fopen(File,"r");
    if(!fp)
    {
        return OAL_FAILED;
    }
    Valuecount=strlen(value);
    Namecount=strlen(name);
    fseek(fp,0,2);
    Filecount=ftell(fp);
    fseek(fp,0,0);
    Buffersize=Filecount+Namecount+Valuecount+TAG_SIZE;
    FileBuffer=(char *)malloc(Buffersize+1);
    if(!FileBuffer)
    {
        fclose(fp);
        return OAL_FAILED;
    }
    memset(FileBuffer,0,Buffersize+1);
    while(!feof(fp))
    {
       fgets(GetBuffer,sizeof(GetBuffer),fp);
       strcat(FileBuffer,GetBuffer);
       memset(GetBuffer,0,100);
    }
    if((ValueStart=strstr(FileBuffer,name)))
    {
        OAL_S32 DifferenceCount;
        ValueStart=ValueEnd=ValueStart+Namecount+NAMETOVALUE_SIZE;
        while((*ValueEnd) != '<')
        {
            Fvaluecount++;
            ValueEnd++;
        }
        DifferenceCount=Valuecount-Fvaluecount;
        if(DifferenceCount>0)
        {
            OAL_S32 AfterEnd_Size=strlen(ValueEnd);           //after the length of value
            OAL_S32 buffercount=strlen(FileBuffer);
            ValueEnd=FileBuffer+buffercount;
            OAL_S8 *BufferEnd=ValueEnd+DifferenceCount;       //Move to the end of the expansion
            AfterEnd_Size+=1;                  //'\0'
            while(AfterEnd_Size--)
            {
               *(BufferEnd--)=*(ValueEnd--);
            }
            while(*value)
            {
                *(ValueStart++)=*(value++);
            }
        }
        else if(DifferenceCount<0)
        {
            DifferenceCount=-DifferenceCount;   //Need to narrow the size
            while(*value)
            {
                *(ValueStart++)=*(value++);
            }
            while(*ValueEnd)
            {
                *(ValueStart++)=*(ValueEnd++);
            }
            *ValueStart='\0';
        }
        else
        {
            while(*value)
            {
                *(ValueStart++)=*(value++);
            }
        }
    }
    else
    {
        strcat(FileBuffer,"<root>\n<name>");
        strcat(FileBuffer,name);
        strcat(FileBuffer,"</name>\n<value>");
        strcat(FileBuffer,value);
        strcat(FileBuffer,"</value>\n</root>");
    }
    fclose(fp);
    fp=fopen(File,"w");
    if(!fp)
    {
        free(FileBuffer);
        return OAL_FAILED;
    }
    Fprint(FileBuffer,fp);
    fclose(fp);
    free(FileBuffer);
    return OAL_SUCCESS;
}
OAL_RESULT OAL_RegistryGetStringValue(char *name, char *value, int valueLen)
{
    OAL_S8 buffer[512];
    OAL_S8 Fvalue[300];
    OAL_S8 *start=Fvalue;
    OAL_S8 *get;
    if((!name)||(!value))
    {
        return OAL_FAILED;
    }
    FILE* fp=fopen(File,"r");
    if(!fp)
    {
        return OAL_FAILED;
    }
    fseek(fp,0,0);
    while(Fread(buffer,fp)==OAL_SUCCESS)
    {
        if(get=strstr(buffer,name))
        {
            get=strstr(buffer,VALUE_START);
            get+=VALUE_START_SZIE;
            while(memcmp(start - VALUE_END_SIZE,VALUE_END,VALUE_END_SIZE) != 0)
            {
                *start=*get;
                start++;
                get++;
            }
            *(start-VALUE_END_SIZE)='\0';
            if(strlen(Fvalue)>valueLen)
            {
               for(int i=0;i<valueLen-1;i++)
               {
                   *(value++)=Fvalue[i];
               }
               *value='\0';
            }
            else
                strcpy(value,Fvalue);
            fclose(fp);
            return OAL_SUCCESS;
        }
    }
    fclose(fp);
    return OAL_FAILED;
}
