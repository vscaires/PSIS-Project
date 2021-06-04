#include "localserver.h"

groupsecret *gs;


void * thread_func(void *arg){
    int local_socket;
    int size, error, i, flag, numbytes;
    char key[256], group[256], secret[256], *values;
    char flag_auth[8], check[8];

    local_socket  = (int) arg;

    printf("Local socket %d connected!\n", local_socket);
    eRead(local_socket, &size, sizeof(size));
    eRead(local_socket, group, size);
    printf("Group name : %s\n", group);
    

    if(search_group(gs, group) == NULL){
        close(local_socket);
        printf("Invalid Group\n");
        printf("Socket closed\n");
        return;
    }

    eRead(local_socket, &size, sizeof(size));
    eRead(local_socket, secret, size);
    printf("Secret : %s\n", secret);

    printf("Trying to authenticate...\n");
    strcpy(flag_auth, "1"); /*Authentication FLAG*/
    if (numbytes = sendto(inet_socket, flag_auth, sizeof(flag_auth), 0,
                            (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("sendto (flag)");
        exit(1);
    }
    if (numbytes = sendto(inet_socket, group, sizeof(group), 0,
                            (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("sendto (group)");
        exit(1);
    }
    if (numbytes = sendto(inet_socket, secret, sizeof(secret), 0,
                            (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("sendto (secret)");
        exit(1);
    }
    if ((numbytes = recvfrom(inet_socket, check, 8, 0,
                (struct sockaddr *)&server_addr, sizeof(server_addr))) == -1) {
            perror("recvfrom");
            exit(-1);
    }

    if(strcmp(check, "ok") != 0){
        printf("Not Authenticated\n");
        close(local_socket);
    }

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
            }

            if(search_keyvalue(gs->head_2ndlist, key) == NULL || s != 0){
                gs->head_2ndlist = insertNew_keyvalue(gs->head_2ndlist, key, values, error);
                // printf("Key-value added\n");
            }else{
                // printf("Key-value already exists\n");   
            }
            free(values);

        }else if(flag == 2){ // get_value
            eRead(local_socket, &size, sizeof(size));
            eRead(local_socket, key, size);

            if(search_keyvalue(gs->head_2ndlist, key) == NULL){
                flag = -1;
                eWrite(local_socket, &flag, sizeof(flag));
                // printf("Key-value not found ...\n");
            }else{
                size = strlen(search_keyvalue(gs->head_2ndlist, key));
                eWrite(local_socket, &size, sizeof(size));
                eWrite(local_socket, search_keyvalue(gs->head_2ndlist, key), flag);
                // printf("Key-value found!\n");   
            }

        }else if(flag == 3){ // delete_value
            eRead(local_socket, &size, sizeof(size));
            eRead(local_socket, key, size);

            if(search_keyvalue(gs->head_2ndlist, key) == NULL){
                // printf("Key-value not found ...\n");
            }else{
                remove_keyvalue_pair(gs->head_2ndlist, key);
                // printf("Key-value found and deleted!\n");   
            }
        }
        memset(key, 0, sizeof(key));


    }while(eRead(local_socket, &flag, sizeof(flag)) > 0);
    
    close(local_socket);
    // ("Socket closed\n");
    
}

void * commands_fun(void * arg){
    while(1){
        gs = commands(gs);
        // sleep(3);
    }
}

int main(int argc, char *argv[]){
    
    int unix_socket;
    char c[1], *values, group[256];
    char name[100];

    gs = initList();

    /* UNIX Stream Socket */
    struct sockaddr_un unix_addr;

    memset(&unix_addr, 0, sizeof(struct sockaddr_un));
    /* Clear structure */
    unix_addr.sun_family = AF_UNIX;
    /* UNIX domain address */
    strcpy(unix_addr.sun_path, SOCKNAME);

    unlink(SOCKNAME);
    int server_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if(server_sock == -1){
        perror("socket creation");
        exit(-1);
    }
    if(bind(server_sock, (struct sockaddr *) &unix_addr, sizeof(struct sockaddr_un)) == -1){
        perror("bind");
        exit(-1);
    }
    if(listen(server_sock, 5) == -1){
        perror("Listen");
        exit(-1);
    }

    /* INET DGRAM Socket */
    char buff[100];
    int nbytes;
    struct sockaddr_in local_addr;

    if(argc<2){
        printf("Remote address required\n");
        exit(-1);
    }
    inet_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if(inet_socket == -1){
        perror("socket: ");
        exit(-1);
    }
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = INADDR_ANY;
    local_addr.sin_port = htons(3000);

    int bind_err = bind(inet_socket, (struct sockaddr *)&local_addr,
                                        sizeof(local_addr));

    if(bind_err == -1){
        perror("bind");
        exit(-1);
    }

    printf(" Socket created \n Ready to send\n");

    server_addr.sin_family = AF_INET;
    inet_aton(argv[1], &server_addr.sin_addr);
    server_addr.sin_port = htons(3001);
    int numbytes;
    
    
    

    pthread_t t_id;
    pthread_create(&t_id, NULL, commands_fun, NULL);
    while(1){
        unix_socket = accept(server_sock, NULL, NULL);
        // printf("accepted one client -> new socket %d\n", unix_socket);
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, thread_func, unix_socket);
    }
 
    return 0;
}



