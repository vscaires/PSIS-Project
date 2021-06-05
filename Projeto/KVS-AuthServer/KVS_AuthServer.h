#ifndef AUTHSERVER_H
#define AUTHSERVER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>

typedef struct auth{
    struct auth* next_auth; 
    char* group_id;
    char* secret;
}auth;

// typedef struct locals{
//     char local_id[6];
//     struct auth* head_auth;
//     struct locals* next_locals;
    
// }locals;


auth* initList(void);  

// void free_all_locals(locals* main_head);
void free_auth_lists(auth* head_auth);
 
auth *insert_new_auth(auth* head_auth,char* group_new,char* secret_new,int err);
// locals *insert_new_local(locals* main_head,char * local_id_new, int err);

int authentication(auth* p, char* secret_check);

auth *remove_auth_pair(auth* head_auth, char* group_asked);

auth* search_auth(auth* head_auth,char* group_asked);
// locals* search_local(locals* main_head, char * local_asked);

// int countgroups_perLocal(locals* p);

void * thread_func(void * arg);

#endif
