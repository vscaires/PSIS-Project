#ifndef LOCALSERVER_H
#define LOCALSERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>

#define SOCKNAME "/tmp/KVS-LocalServer"


typedef struct keyvalue{
    struct keyvalue* next; 
    char* key;
    char* value;
}keyvalue;

typedef struct groupsecret{

    struct groupsecret* nextgroup;
    struct keyvalue* head_2ndlist;
    char* group_id;
    char* secret;

}groupsecret;


int eRead(int fd, void *buf, size_t count);
int eWrite(int fd, void *buf, size_t count);

groupsecret *initList(void);

void free_keyvalue_List(keyvalue* head_2ndlist);
void free_All(groupsecret* groupsecret_head);

groupsecret *insertNew_group(groupsecret* groupsecret_head,char* group,char* secret,int err);
keyvalue *insertNew_keyvalue(keyvalue* head_2ndlist,char* key,char* value,int err);

groupsecret* search_group(groupsecret* groupsecret_head, char* group_ID_asked);

char* read_value(keyvalue* p);
keyvalue* change_value(keyvalue* p, char* newvalue);
void* search_keyvalue(keyvalue* head_2ndList,char* key_asked);
void remove_keyvalue_pair(keyvalue* head_2ndList, char* key_asked);

void commands();

int countKeys_perGroup(groupsecret* groupsecret_head);


#endif