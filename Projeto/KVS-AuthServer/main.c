#include "KVS_AuthServer.h"

#define MYPORT "4950"    // the port users will be connecting to

#define MAXBUFLEN 256


int main(void)
{
    struct sockaddr_in local_addr;
    struct sockaddr_in other_addr;
    socklen_t size_other_addr;
    char group[MAXBUFLEN], secret[MAXBUFLEN];

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd == -1){
        perror("socket: ");
        exit(-1);
    }

    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = INADDR_ANY;

    int bind_err = bind(sockfd, (struct sockaddr * )&local_addr,
                                sizeof(local_addr));
    if(bind_err == -1){
        perror("bind");
        exit(-1);
    }

    printf("Socket created and binded!\n");

    auth *list = NULL, *aux = NULL;

    while(1){
        addr_len = sizeof their_addr;
        /* Flag Receival */
        if ((numbytes = recvfrom(sockfd, &flag, sizeof(flag) - 1 , 0,
                (struct sockaddr *)&their_addr, &addr_len)) == -1) {
            perror("recvfrom");
            exit(-1);
        }
        /* Group Receival */
        if ((numbytes = recvfrom(sockfd, group, MAXBUFLEN - 1 , 0,
                (struct sockaddr *)&their_addr, &addr_len)) == -1) {
            perror("recvfrom");
            exit(-1);
        }
        /* Secret Receival */
        if ((numbytes = recvfrom(sockfd, secret, MAXBUFLEN - 1 , 0,
                (struct sockaddr *)&their_addr, &addr_len)) == -1) {
            perror("recvfrom");
            exit(-1);
        }
        if(flag == 0) //Insert
            list = insert_new_auth(list, group, secret, err);
        
        if(flag == 1) //Authentication
        {
            aux = search_auth(list, group);
            if(aux == NULL){ //Wrong Group
                flag = 1;
                if ((numbytes = sendto(sockfd, &flag, sizeof(flag), 0,
                            (struct sockaddr *)&their_addr, addr_len)) == -1) {
                perror("sendto");
                exit(1);
                continue;
                }
            }else{
                if(authentication(aux, secret) == 1){
                    flag = 2; //Wrong Secret
                    if ((numbytes = sendto(sockfd, &flag, sizeof(flag), 0,
                            (struct sockaddr *)&their_addr, addr_len)) == -1) {
                        perror("sendto");
                        exit(1);
                    }
                    continue;
                }
                flag = 0; //Success
                if ((numbytes = sendto(sockfd, &flag, sizeof(flag), 0,
                            (struct sockaddr *)&their_addr, addr_len)) == -1) {
                perror("sendto");
                exit(1);
                }
            }
        
        }


    }
    close(sockfd);
    return 0;
}