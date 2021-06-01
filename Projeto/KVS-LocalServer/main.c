#include "localserver.h"

int main(){

    int client_socket, i = 0, error;
    char key[256], c[1], *value, group[256];
    struct sockaddr_un server_addr;

    groupsecret *gs = initList();

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
        eRead(client_socket, &i, sizeof(i));
        eRead(client_socket, group, i);
        printf("Group name : %s\n", group);
        
        groupsecret *aux = insertNew_group(gs, group, "", error);

        eRead(client_socket, &i, sizeof(i)); 
        do{
            if(i == 1){ //put_value
                eRead(client_socket, &i, sizeof(i));
                eRead(client_socket, key, i);
                
                eRead(client_socket, &i, sizeof(i));
                value = malloc(i*sizeof(char));
                eRead(client_socket, value, i);
                
                if(search_keyvalue(aux->head_2ndlist, key) == NULL || strcmp(search_keyvalue(aux->head_2ndlist, key), value) != 0){
                    insertNew_keyvalue(aux->head_2ndlist, key, value, error);
                    printf("Key-value added\n");
                }else{
                    printf("Key-value already exists\n");   
                }
                memset(key, 0, sizeof(key));
                memset(value, 0, sizeof(value));

            }else if(i == 2){ // get_value
                eRead(client_socket, &i, sizeof(i));
                eRead(client_socket, key, i);

                if(search_keyvalue(aux->head_2ndlist, key) == NULL){
                    i = -1;
                    eWrite(client_socket, &i, sizeof(i));
                    printf("Key-value not found ...\n");
                }else{
                    i = strlen(search_keyvalue(aux->head_2ndlist, key));
                    eWrite(client_socket, &i, sizeof(i));
                    eWrite(client_socket, search_keyvalue(aux->head_2ndlist, key), i);
                    printf("Key-value found!\n");   
                }
                memset(key, 0, sizeof(key));
                memset(value, 0, sizeof(value));

            }else if(i == 3){ // delete_value
                eRead(client_socket, &i, sizeof(i));
                eRead(client_socket, key, i);

                if(search_keyvalue(aux->head_2ndlist, key) == NULL){
                    printf("Key-value not found ...\n");
                }else{
                    remove_keyvalue_pair(aux->head_2ndlist, key);
                    printf("Key-value found and deleted!\n");   
                }
            }
        }while(eRead(client_socket, &i, sizeof(i)) > 0);
        
        close(client_socket);
        printf("Socket closed\n");
    }
    countKeys_perGroup(gs);
    free_All(gs);
    printf("Key-Value data destroyed! \n");

    return 0;
}



