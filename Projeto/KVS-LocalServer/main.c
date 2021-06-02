#include "localserver.h"
groupsecret *gs;

void * thread_func(void *arg){
    int local_socket;
    int size, error, flag, i;
    char key[256], group[265], *values;

    local_socket  = (int) arg;

    printf("Local socket %d connected!\n", local_socket);
    eRead(local_socket, &size, sizeof(size));
    eRead(local_socket, group, size);
    printf("Group name : %s\n", group);

    if(search_group(gs, group) == NULL)
        gs = insertNew_group(gs, group, "", error);
    else
        gs = search_group(gs, group);
    int s;

    eRead(local_socket, &flag, sizeof(flag)); 
    do{
        if(flag == 1){ //put_value
            eRead(local_socket, &size, sizeof(size));
            eRead(local_socket, key, size);
            
            eRead(local_socket, &size, sizeof(size));
            values = malloc(size*sizeof(char));
            eRead(local_socket, values, size);
            
            if(search_keyvalue(gs->head_2ndlist, key) != NULL){
                s = strcmp(search_keyvalue(gs->head_2ndlist, key), values);
                printf("strcmp %d\n", s);
            }

            if(search_keyvalue(gs->head_2ndlist, key) == NULL || s != 0){
                gs->head_2ndlist = insertNew_keyvalue(gs->head_2ndlist, key, values, error);
                printf("Key-value added\n");
            }else{
                printf("Key-value already exists\n");   
            }
            free(values);

        }else if(flag == 2){ // get_value
            eRead(local_socket, &size, sizeof(size));
            eRead(local_socket, key, size);

            if(search_keyvalue(gs->head_2ndlist, key) == NULL){
                flag = -1;
                eWrite(local_socket, &flag, sizeof(flag));
                printf("Key-value not found ...\n");
            }else{
                size = strlen(search_keyvalue(gs->head_2ndlist, key));
                eWrite(local_socket, &size, sizeof(size));
                eWrite(local_socket, search_keyvalue(gs->head_2ndlist, key), flag);
                printf("Key-value found!\n");   
            }

        }else if(flag == 3){ // delete_value
            eRead(local_socket, &size, sizeof(size));
            eRead(local_socket, key, size);

            if(search_keyvalue(gs->head_2ndlist, key) == NULL){
                printf("Key-value not found ...\n");
            }else{
                remove_keyvalue_pair(gs->head_2ndlist, key);
                printf("Key-value found and deleted!\n");   
            }
        }
        memset(key, 0, sizeof(key));


    }while(eRead(local_socket, &flag, sizeof(flag)) > 0);
    
    close(local_socket);
    printf("Socket closed\n");
    
}

int main(){
    int client_socket;
    char c[1], *values, group[256];
    struct sockaddr_un server_addr;

    gs = initList();

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
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, thread_func, client_socket);
    }
 
    return 0;
}



