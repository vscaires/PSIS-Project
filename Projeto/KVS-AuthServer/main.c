#include "KVS_AuthServer.h"

#define MYPORT "4950"    // the port users will be connecting to

#define MAXBUFLEN 256

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes, flag, err;
    struct sockaddr_storage their_addr;
    char group[MAXBUFLEN], secret[MAXBUFLEN];
    socklen_t addr_len;


    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; 
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; 

    if ((rv = getaddrinfo(NULL, MYPORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("KVS-AuthServer: socket");
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("KVS-AuthServer: bind");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "KVS-AuthServer: failed to bind socket\n");
        return 2;
    }

    freeaddrinfo(servinfo);

    printf("KVS-AuthServer: waiting to recvfrom...\n");

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