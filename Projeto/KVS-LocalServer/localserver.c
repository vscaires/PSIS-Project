#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define SOCKNAME "/tmp/KVS-LocalServer"

void * client_thread(void * arg){

    int local_socket = (int) arg;
    char buf[10000];
    printf("New thread -> local socket %d\n", local_socket);

    do{
        if(strcmp(buf, "put")){
            printf("WOW\n");
            write(local_socket, "put_value", sizeof("put_value"));
        }
    }while(read(local_socket, &buf, sizeof(buf)) >0);

    close(local_socket);
    printf("exiting thread\n");
    return NULL;
}

int main(){
    struct sockaddr_un server_addr;

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
    while(1){
        client_socket = accept(server_sock, NULL, NULL);
        printf("accepted one client -> new socket %d\n", client_socket);
        
        pthread_t t_id;
        pthread_create(t_id, NULL, client_thread, client_socket);

    }


}