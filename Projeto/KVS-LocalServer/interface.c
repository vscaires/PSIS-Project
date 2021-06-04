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
    char group[256];
    int error;

    printf("Insert the Group Name:\t");
    char c = NULL;
    while ((c = getchar()) != '\n' && c != EOF) {
        // Do nothing until input buffer fully flushed.
    }
    fgets(group, sizeof(group), stdin);
    printf("%s\n", group);
    group[strcspn(group, "\n")] = 0;  
    if(search_group(head, group) == NULL){
        head = insertNew_group(head, group, "", error);
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
    char c = NULL;
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
    char c = NULL;
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

}

void exit_server(groupsecret *head){

}