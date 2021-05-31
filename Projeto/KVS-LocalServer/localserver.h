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

#define SOCKNAME "/tmp/KVS-LocalServer"

typedef struct dict_t_struct {
    char *key;
    void *value;
    struct dict_t_struct *next;
} dict_t;

dict_t **dictAlloc(void);
void dictDealloc(dict_t **dict);
void *getItem(dict_t *dict, char *key);
void delItem(dict_t **dict, char *key);
void addItem(dict_t **dict, char *key, void *value);

#endif