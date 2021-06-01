#include "localserver.h"

void commands(){
    char command;

    printf("\n");
    printf("***************************************************\n");
    printf("*             KVS-LocalServer Commands            *\n");
    printf("*                                                 *\n");
    printf("* 1 - Create Group;                               *\n");
    printf("* 2 - Delete Group;                               *\n");
    printf("* 3 - Show Group Info;                            *\n");
    printf("* 4 - Show Application Status;                    *\n");
    printf("*                                                 *\n");
    printf("***************************************************\n");

    scanf("%c", &command);

    switch(command)
    {
        case '1':
            printf("Create Group\n");
            break;

        case '2':
            printf("Delete Group\n");
            break;

        case '3':
            printf("Show Group Info\n");
            break;

        case '4':
            printf("Show Application Status\n");
            break;

        default:
            printf("Error! Command is not correct\n");
    }
}