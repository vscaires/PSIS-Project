#include "localserver.h"

groupsecret *commands(groupsecret *head){
    int command;

    printf("\n");
    printf("***************************************************\n");
    printf("*             KVS-LocalServer Commands            *\n");
    printf("*                                                 *\n");
    printf("* 1 - Create Group;                               *\n");
    printf("* 2 - Delete Group;                               *\n");
    printf("* 3 - Show Group Info;                            *\n");
    printf("* 4 - Show Application Status;                    *\n");
    printf("*                                                 *\n");
    printf("* 0 - Exit;                                       *\n");
    printf("*                                                 *\n");
    printf("***************************************************\n");

    scanf("%d", &command);
    switch(command)
    {
        case 1:
            printf("Create Group\n");
            head = create_group(head);
            break;

        case 2:
            printf("Delete Group\n");
            head = delete_group(head);
            break;

        case 3:
            printf("Show Group Info\n");
            show_group(head);
            break;

        case 4:
            printf("Show Application Status\n");
            show_status(head);
            break;

        case 0:
            printf("Exit\n");
            exit_server(head);
            break;

        default:
            printf("Error! Command is not correct\n");
    }
    return head;
}

groupsecret *create_group(groupsecret *head){
    char group[256], flag[8], secret[256];
    int error, numbytes;
    

    printf("Insert the Group Name:\t");
    char c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // Do nothing until input buffer fully flushed.
    }
    fgets(group, sizeof(group), stdin);
    printf("%s\n", group);
    group[strcspn(group, "\n")] = 0;
    gen_random(secret, 255);
    if(search_group(head, group) == NULL){
        head = insertNew_group(head, group, "", error);
        strcpy(flag, "0"); /*Insert new Group-Secret FLAG*/
        if (numbytes = sendto(inet_socket, flag, sizeof(flag), 0,
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
        printf("New group added!\n");
    }     
    else
        printf("Group already exists\n");

    return head;
}

groupsecret *delete_group(groupsecret *head){
    int error;
    char group[256];

    printf("Insert the Group Name:\t");
    char c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // Do nothing until input buffer fully flushed.
    }
    fgets(group, sizeof(group), stdin);
    printf("%s\n", group);
    group[strcspn(group, "\n")] = 0;  

    groupsecret *aux = search_group(head, group);
    if( aux == NULL){
        printf("Group doesn't exist!\n");
        sleep(1);

    }     
    else{
        head = deleteGroup(&head, group);
        printf("Group deleted\n");
        sleep(1);
    }
    
    return head;
}

void show_group(groupsecret *head){
    int keys;
    char group[256], secret[256];
    groupsecret *aux;

    printf("Insert the Group Name:\t");
    char c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // Do nothing until input buffer fully flushed.
    }
    fgets(group, sizeof(group), stdin);
    printf("%s\n", group);
    group[strcspn(group, "\n")] = 0;  

    if(search_group(head, group) == NULL){
        printf("Group doesn't exist!\n");
        sleep(2);
    }     
    else{
        aux = search_group(head, group);
        strcpy(secret, aux->secret);
        printf("Secret: %s\n", secret);
        keys = countKeys_perGroup(aux);
        printf("Number of key-value pairs: %d\n", keys);
        sleep(2);
    }
    
}

void show_status(groupsecret *head){
    int i;
    printf("List of Client PIDs : \n");
    for(i = 0; i < 100; i++){
        if(strcmp(client_PID[i], "") == 0){
            printf(".\n");
            break;
        }
        printf("%d - %s\n", i, client_PID[i]);
    }
}

void exit_server(groupsecret *head){
    printf("Server Closed.\n");
    exit(1);
}

void gen_random(char *s, int len) {
    char alphanum[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";

    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
}