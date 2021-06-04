#ifndef AUTHSERVER_H
#define AUTHSERVER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct auth{
    struct auth* next_auth; 
    char* group_id;
    char* secret;
}auth;

typedef struct locals{
    pid_t local_id;
    struct auth* head_auth;
    struct locals* next_locals;
    
    
}locals;

//
locals* initList(void);  

void free_all_locals(locals* main_head);
void free_auth_lists(locals* head_auth);
 
auth *insert_new_auth(auth* head_auth,char* group_new,char* secret_new,int err);
locals *insert_new_local(locals* main_head,pid_t local_id_new, int err);

int authentication(auth* p, char* secret_check);

auth* change_secret(auth* p, char* new_secret);
void remove_auth_pair(auth* head_auth, char* group_asked);

auth* search_auth(auth* head_auth,char* group_asked);
locals* search_local(locals* main_head, pid_t local_asked);

int countgroups_perLocal(locals* p);

#endif
