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
#include <pthread.h>
#include <netinet/in.h>
#include <netdb.h>

#define SOCKNAME "/tmp/KVS-LocalServer"
#define SERVERPORT "4950"    // the port users will be connecting to



typedef struct keyvalue{
    struct keyvalue* next; 
    char key[256];
    char value[1000];
}keyvalue;

typedef struct groupsecret{

    struct groupsecret* nextgroup;
    struct keyvalue* head_2ndlist;
    char group_id[256];
    char secret[256];

}groupsecret;

typedef struct status{
    int client_PID;
    int establishing_time;
    int close_time; 
}status;


int eRead(int fd, void *buf, size_t count);
int eWrite(int fd, void *buf, size_t count);

groupsecret *initList(void);
groupsecret *insertNew_group(groupsecret* groupsecret_head,char* group,char* secret,int err);
groupsecret* search_group(groupsecret* groupsecret_head, char* group_ID_asked);
int countKeys_perGroup(groupsecret* groupsecret_head);
void free_All(groupsecret* groupsecret_head);
groupsecret* deleteGroup(groupsecret **groupsecret_head, char *group);

keyvalue *insertNew_keyvalue(keyvalue* head_2ndlist,char* key,char* value,int err);
char* read_value(keyvalue* p);
keyvalue* change_value(keyvalue* p, char* newvalue);
void* search_keyvalue(keyvalue* head_2ndList,char* key_asked);
void remove_keyvalue_pair(keyvalue* head_2ndList, char* key_asked);
void free_keyvalue_List(keyvalue* head_2ndlist);

groupsecret *commands(groupsecret *head);
groupsecret *create_group(groupsecret *head);
groupsecret *delete_group(groupsecret *head);
void show_group(groupsecret *head);
void show_status(groupsecret *head);
void exit_server(groupsecret *head);


#endif