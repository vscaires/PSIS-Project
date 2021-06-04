#include "KVS-lib.h"



int main(){
    char group_id[256], secret[256];
    char value[256], key[256];
    char ** msg;

    printf("Application 1 initiating ...\n");

    strcpy(group_id, "ola");
    strcpy(secret, "terces");
    strcpy(key, "abcd");
    strcpy(value, "As rosas sao verdes!"); 

    establish_connection(group_id, secret);
    put_value(key, value);
    strcpy(key, "1234");
    strcpy(value, "TSB GO GO GO!");
    put_value(key, value);
    strcpy(key, "4321");
    strcpy(value, "9 * 9 = 81");
    put_value(key, value);
    strcpy(key, "dcba");
    strcpy(value, "PSIS");
    put_value(key, value);
    //get_value(key, msg);
    //delete_value(key);
    get_value(key, msg);

    close_connection();

    return(0);
}
