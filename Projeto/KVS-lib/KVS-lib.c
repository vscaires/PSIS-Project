#include "KVS-lib.h"

#define SOCKET_ADDR "/tmp/KVS-LocalServer"
int sock;
struct sockaddr_un app_sock_addr;


int main (){    
    char * group_id;
    char * secret;
    group_id = "group_id";
    secret = "secret";

    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if(sock == -1){
        perror("socket creation");
        exit(-1);
    }

    memset(&app_sock_addr, 0, sizeof(struct sockaddr_un));
    /* Clear structure */
    app_sock_addr.sun_family = AF_UNIX;
    /* UNIX domain address */
    strcpy(app_sock_addr.sun_path, SOCKET_ADDR);

    if(connect(sock, & app_sock_addr, sizeof(app_sock_addr))){
        perror("connect");
        exit(-1);
    }
    
    printf("Connected.\n");

    char buf[1024];
    int t;
    while(printf("> "), fgets(buf, 1024, stdin), !feof(stdin)) {
        if (send(sock, group_id, strlen(group_id), 0) == -1) {
            perror("send");
            exit(1); 
        }
        if (send(sock, secret, strlen(secret), 0) == -1) {
            perror("send");
            exit(1); 
        }
        if ((t=recv(sock, buf, 1024, 0)) > 0) {
            buf[t] = '\0';
            if(strcmp(buf, group_id) == 0){
                printf("Group ID found\n");
            }
            if(strcmp(buf, secret) ==0)
                printf("secret found\n");
            printf("echo> %s", buf);
        } else {
            if (t < 0) perror("recv");
            else printf("Server closed connection\n");
            exit(1);
        } 
    }

    return 0;
}


int put_value(char * key, char * value){
    if (send(sock, key, strlen(key), 0) == -1) {
        perror("send key");
        exit(-1); 
    }
    if (send(sock, value, strlen(value), 0) == -1) {
        perror("send value");
        exit(-1); 
    }

    return 1;
}

int get_value(char * key, char ** value){
    if (send(sock, key, strlen(key), 0) == -1) {
        perror("send key");
        exit(-1); 
    }
    
    char buf[1024];
    if (read(sock, buf, 1024) < 0){
        perror("receiving datagram packet");
        return(-2);
    }
}

int delete_value(char * key){
    if (send(sock, key, strlen(key), 0) == -1) {
        perror("send key");
        exit(-1); 
    }
}

int register_callback(char * key, void (*callback_function)(char *)){

}

int close_connection(){
    close(sock);
}