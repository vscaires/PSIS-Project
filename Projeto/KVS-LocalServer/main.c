#include "localserver.h"

int main(){

    int client_socket, i = 0;
    char key[256], c[1], *value;
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

    while(1){
        client_socket = accept(server_sock, NULL, NULL);
        printf("accepted one client -> new socket %d\n", client_socket);

        read(client_socket, &i, sizeof(i)); 
        do{
            if(i == 1){ //put_value
                read(client_socket, &i, sizeof(i));
                read(client_socket, key, i);
                
                read(client_socket, &i, sizeof(i));
                value = malloc(i*sizeof(char));
                read(client_socket, value, i);
                
                if(getItem(*kvs, key) == NULL || getItem(*kvs, key) != value){
                    addItem(kvs, key, value);
                    printf("Key-value added\n");
                }else{
                    printf("Key-value already exists\n");   
                }
                memset(key, 0, sizeof(key));
                memset(value, 0, sizeof(value));

            }else if(i == 2){ // get_value
                read(client_socket, &i, sizeof(i));
                read(client_socket, key, i);

                if(getItem(*kvs, key) == NULL){
                    i = -1;
                    write(client_socket, &i, sizeof(i));
                    printf("Key-value not found ...\n");
                }else{
                    (char*)getItem(*kvs, key); //incomplete
                    i = strlen((char*)getItem(*kvs, key));
                    write(client_socket, &i, sizeof(i));
                    write(client_socket, (char*)getItem(*kvs, key), i);
                    printf("Key-value found!\n");   
                }
                memset(key, 0, sizeof(key));
                memset(value, 0, sizeof(value));

            }else if(i == 3){ // delete_value
                read(client_socket, &i, sizeof(i));
                read(client_socket, key, i);

                if(getItem(*kvs, key) == NULL){
                    printf("Key-value not found ...\n");
                }else{
                    delItem(kvs, key);
                    printf("Key-value found and deleted!\n");   
                }
            }
        }while(read(client_socket, &i, sizeof(i)) > 0);

        close(client_socket);
        printf("Socket closed\n");
    }
    dictDealloc(kvs);
    return 0;
}

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
