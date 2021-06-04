#include "KVS_AuthServer.h"



int main(int argc, char *argv[])
{
    int server_sock, addr_len, numbytes, err, check, flag;
    struct sockaddr_in server_addr;
    struct sockaddr_storage their_addr;
    char group[256], secret[256];

    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    /* Clear structure */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(22);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(server_sock, (const struct sockaddr *) &server_addr, sizeof(server_addr)) == -1){
        perror("bind");
        exit(-1);
    }

    auth *list = NULL, *aux = NULL;

    while(1){
        addr_len = sizeof their_addr;
        if ((numbytes = recvfrom(server_sock, flag, sizeof(flag) - 1 , NULL,
                (struct sockaddr *)&their_addr, &addr_len)) == -1) {
            perror("recvfrom");
            exit(-1);
        }
        if ((numbytes = recvfrom(server_sock, group, sizeof(group) - 1 , NULL,
                (struct sockaddr *)&their_addr, &addr_len)) == -1) {
            perror("recvfrom");
            exit(-1);
        }
        if ((numbytes = recvfrom(server_sock, secret, sizeof(secret) - 1 , NULL,
                (struct sockaddr *)&their_addr, &addr_len)) == -1) {
            perror("recvfrom");
            exit(-1);
        }
        if(flag == 0) //Insert
            list = insert_new_auth(list, group, secret, err);
        
        if(flag == 1) //Authentication
        {
            aux = search_auth(list, group);
            if(aux == NULL){
                check = 1;
                if ((numbytes = sendto(server_sock, check, sizeof(check), NULL,
                (struct sockaddr *)&their_addr, &addr_len)) == -1) {
                perror("sendto");
                exit(1);
            }else{
                if(authentication(aux,))
                check = 2;
                if ((numbytes = sendto(server_sock, check, sizeof(check), NULL,
                (struct sockaddr *)&their_addr, &addr_len)) == -1) {
                perror("sendto");
                exit(1);
            }
        }
    }


    }

    return 0;
}