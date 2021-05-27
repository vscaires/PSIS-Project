#include "KVS-lib.h"




int establish_connection (char * group_id, char * secret){    
    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if(sock == -1){
        perror("socket creation");
        exit(-1);
    }
   
    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    /* Clear structure */
    server_addr.sun_family = AF_UNIX;
    /* UNIX domain address */
    strcpy(server_addr.sun_path, SOCKNAME);

    if(connect(sock, (const struct sockaddr *)&server_addr, sizeof(server_addr))){
        perror("connect");
        exit(-1);
    }
    printf("connected\n");

    return 0;
}


int put_value(char * key, char * value){
    char flag[8], buf[10000];

    strcpy(flag, "put");
    if(write(sock, flag, sizeof(flag)) < 0){
        perror("write flag");
        exit(-1);
    }
    
    if (write(sock, key, strlen(value)) < 0) {
        perror("write key");
        exit(-2); 
    }

    if (write(sock, value, strlen(value)) < 0) {
        perror("write value");
        exit(-3); 
    }

    return 1;
}

int get_value(char * key, char ** value){
    char flag[8];
    strcpy(flag, "get");
    if (write(sock, flag, strlen(flag)) == -1) {
        perror("send flag");
        exit(-1); 
    }

    if (write(sock, key, strlen(key)) == -1) {
        perror("send key");
        exit(-2); 
    }
    // malloc
    do{
        sleep(1);
    }while(read(sock, value, sizeof(value)) > 0);
}

int delete_value(char * key){
    char flag[8];
    strcpy(flag, "delete");
    if (write(sock, flag, strlen(flag)) == -1) {
        perror("send flag");
        exit(-1); 
    }

    if (send(sock, key, strlen(key), 0) == -1) {
        perror("send key");
        exit(-2); 
    }
}

int register_callback(char * key, void (*callback_function)(char *)){

}

int close_connection(){
    printf("Exiting sock %d\n", sock);
    close(sock);
}