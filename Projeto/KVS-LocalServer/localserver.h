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
#include <arpa/inet.h>

#define SOCKNAME "/tmp/KVS-LocalServer"
#define MAX_CLIENTS 100
#define MAX_SIZE 256
#define MAX_PID 6
#define MAX_FLAG 8

typedef struct keyvalue{
    struct keyvalue* next; 
    char key[MAX_SIZE];
    char *value;
}keyvalue;

typedef struct groupsecret{

    struct groupsecret* nextgroup;
    struct keyvalue* head_2ndlist;
    char group_id[MAX_SIZE];
    char secret[MAX_SIZE];

}groupsecret;


groupsecret *gs;
struct sockaddr_in server_addr;
int inet_socket;
char client_PID[MAX_CLIENTS][6];

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
void gen_random(char *s, int len);


#endif