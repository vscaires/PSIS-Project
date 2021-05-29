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
#include <errno.h>


#define FIFO_NAME "/tmp/fifo_flags"
#define SOCKNAME "/tmp/KVS-LocalServer"

typedef struct dict_t_struct {
    char *key;
    void *value;
    struct dict_t_struct *next;
} dict_t;

dict_t **dictAlloc(void) {
    return malloc(sizeof(dict_t));
}

void dictDealloc(dict_t **dict) {
    free(dict);
}

void *getItem(dict_t *dict, char *key) {
    dict_t *ptr;
    for (ptr = dict; ptr != NULL; ptr = ptr->next) {
        if (strcmp(ptr->key, key) == 0) {
            return ptr->value;
        }
    }
    
    return NULL;
}

void delItem(dict_t **dict, char *key) {
    dict_t *ptr, *prev;
    for (ptr = *dict, prev = NULL; ptr != NULL; prev = ptr, ptr = ptr->next) {
        if (strcmp(ptr->key, key) == 0) {
            if (ptr->next != NULL) {
                if (prev == NULL) {
                    *dict = ptr->next;
                } else {
                    prev->next = ptr->next;
                }
            } else if (prev != NULL) {
                prev->next = NULL;
            } else {
                *dict = NULL;
            }
            
            free(ptr->key);
            free(ptr);
            
            return;
        }
    }
}

void addItem(dict_t **dict, char *key, void *value) {
    delItem(dict, key); /* If we already have a item with this key, delete it. */
    dict_t *d = malloc(sizeof(struct dict_t_struct));
    d->key = malloc(strlen(key)+1);
    strcpy(d->key, key);
    d->value = value;
    d->next = *dict;
    *dict = d;
}

int main(){

    // mkfifo(FIFO_NAME, "r");
    // printf("Error mkfifo: %s\n", strerror(errno));
    // printf("waiting for writers...\n");
    // int fd = open(FIFO_NAME, O_RDONLY);
    // printf("got a writer\n");

    struct sockaddr_un server_addr;
    dict_t **kvs = dictAlloc();

    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    /* Clear structure */
    server_addr.sun_family = AF_UNIX;
    /* UNIX domain address */
    strcpy(server_addr.sun_path, SOCKNAME);

    unlink(SOCKNAME);
    int server_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if(server_sock == -1){
        perror("socket creation");
        exit(-1);
    }
    if(bind(server_sock, (struct sockaddr *) &server_addr, sizeof(struct sockaddr_un)) == -1){
        perror("bind");
        exit(-1);
    }
    if(listen(server_sock, 5) == -1){
        perror("Listen");
        exit(-1);
    }

    int client_socket;
    char flag_read[8], key[10000], c, value[10000];

    int i = 0;
    while(1){
        client_socket = accept(server_sock, NULL, NULL);
        printf("accepted one client -> new socket %d\n", client_socket);
        
        for(int o = 0; o < 2; i++){
            // read(fd, flag_read, sizeof(flag_read));
            // if(strcmp(flag_read, "put") == 0){
                read(client_socket, key, sizeof(key));
                sleep(5);
                read(client_socket, value, sizeof(value));
                if(getItem(*kvs, key) == NULL || getItem(*kvs, key) != value){
                    addItem(kvs, key, value);
                    printf("Key-value added\n");
                }else{
                    printf("Key-value already exists\n");   
                }
        }
        //}while(1);

        close(client_socket);
        printf("Socket closed\n");
    }
    dictDealloc(kvs);
    return 0;
}