#include "KVS_AuthServer.h"

#define MYPORT "4950"    // the port users will be connecting to
#define MAXBUFLEN 256


int main()
{
    struct sockaddr_in local_addr;
    struct sockaddr_in other_addr;
    socklen_t size_other_addr;
    char group[MAXBUFLEN], secret[MAXBUFLEN];
    int addr_len, err, numbytes;
    char flag[8];

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd == -1){
        perror("socket: ");
        exit(-1);
    }

    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = INADDR_ANY;
    local_addr.sin_port = htons(3001);

    int bind_err = bind(sockfd, (struct sockaddr * )&local_addr,
                                sizeof(local_addr));
    if(bind_err == -1){
        perror("bind");
        exit(-1);
    }

    printf("Socket created and binded!\n");

	printf("Ready to receive messages\n");

    auth *list = NULL, *aux = NULL;

    do{
        size_other_addr = sizeof(struct sockaddr_storage);
        /* Flag Receival */
        if ((numbytes = recvfrom(sockfd, flag, 8, 0,
                (struct sockaddr *)&other_addr, &size_other_addr)) == -1) {
            perror("recvfrom");
            exit(-1);
        }

        int client_port = ntohs(other_addr.sin_port);
		printf("received from %s %d\n", inet_ntoa(other_addr.sin_addr),
						client_port);
        printf("received %d bytes --- %s ---\n", numbytes, flag);
        /* Group Receival */
        if ((numbytes = recvfrom(sockfd, group, MAXBUFLEN, 0,
                (struct sockaddr *)&other_addr, &size_other_addr)) == -1) {
            perror("recvfrom");
            exit(-1);
        }

        printf("received from %s %d\n", inet_ntoa(other_addr.sin_addr),
						client_port);
        printf("received %d bytes --- %s ---\n", numbytes, group);
        /* Secret Receival */
        if ((numbytes = recvfrom(sockfd, secret, MAXBUFLEN, 0,
                (struct sockaddr *)&other_addr, &size_other_addr)) == -1) {
            perror("recvfrom");
            exit(-1);
        }
        printf("received from %s %d\n", inet_ntoa(other_addr.sin_addr),
						client_port);
        printf("received %d bytes --- %s ---\n", numbytes, secret);
        if(strcmp(flag,"0") == 0) //Insert
            list = insert_new_auth(list, group, secret, err);
        
        if(strcmp(flag,"1") == 0) //Authentication FLAG
        {
            aux = search_auth(list, group);
            if(aux == NULL){ //Wrong Group
                strcpy(flag, "1");
                if ((numbytes = sendto(sockfd, flag, 8, 0,
                            (struct sockaddr *)&other_addr, addr_len)) == -1) {
                    perror("sendto");
                    exit(1);
                continue;
                }
            }else{
                if(authentication(aux, secret) == 1){
                    strcpy(flag, "2"); //Wrong Secret
                    if ((numbytes = sendto(sockfd, &flag, sizeof(flag), 0,
                            (struct sockaddr *)&other_addr, addr_len)) == -1) {
                        perror("sendto");
                        exit(1);
                    }
                    continue;
                }
                strcpy(flag, "0"); //Success
                if ((numbytes = sendto(sockfd, &flag, sizeof(flag), 0,
                            (struct sockaddr *)&other_addr, addr_len)) == -1) {
                perror("sendto");
                exit(1);
                }
            }
        
        }


    }while(numbytes > 0);
    close(sockfd);
    return 0;
}