#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define SOCKET_ADDR "/tmp/KVS-LocalServer"

int main(){

    struct sockaddr_un server_addr, remote;
    int t, s2;
    char *group_id, *secret;
    char buf[1024];

    group_id = "group_id";
    secret = "secret";

    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    /* Clear structure */
    server_addr.sun_family = AF_UNIX;
    /* UNIX domain address */
    strcpy(server_addr.sun_path, SOCKET_ADDR);

    unlink(SOCKET_ADDR);

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

    accept(server_sock,0,0);
    printf("Connected\n");


    for(;;) {
        int done, n;
        printf("Waiting for a connection...\n");
        t = sizeof(remote);
        if ((s2 = accept(server_sock, (struct sockaddr *)&remote, &t)) == -1) {
            perror("accept");
            exit(1);
        }
        printf("Connected.\n");
        done = 0; 
        do {
            n = recv(s2, buf, 100, 0);
            if (n <= 0) {
                if (n < 0) perror("recv");
                    done = 1;
            }
            if (!done)
                if (send(s2, buf, n, 0) < 0) {
                    perror("send");
                    done = 1; 
                }
        } while (!done);
    }


    close(server_sock);

    return 0;
}