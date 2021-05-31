#include "KVS-lib.h"



int main(){
    char *group_id, *secret;
    char value[256], key[256];
    char ** msg;

    printf("Application 1 initiating ...\n");

    strcpy(key, "abcd");
    strcpy(value, "10238407123469127346"); 
    
    establish_connection(group_id, secret);
    put_value(key, value);
    get_value(key, msg);
    delete_value(key);
    get_value(key, msg);

    close_connection();

    return(0);
}
