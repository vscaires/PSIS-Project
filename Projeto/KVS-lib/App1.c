#include "KVS-lib.h"



int main(){
    char *group_id, *secret;
    char value[256], key[256];
    char ** msg;

    strcpy(key, "abcd");
    strcpy(value, "10238407123469127346\0"); 
    int sock; 

    establish_connection(group_id, secret);
    put_value(key, value);
    put_value(key, value);
    put_value(key, value);
    strcpy(key, "abc"); 
    put_value(key, value);
    strcpy(key, "ab");
    put_value(key, value);


    close_connection();

    return(0);
}
