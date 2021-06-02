#include "KVS-lib.h"



int main(){
    char group_id[256], secret[256];
    char value[256], key[256];
    char ** msg;

    printf("Application 1 initiating ...\n");

    strcpy(group_id, "something1234");
    strcpy(secret, "password321");
    strcpy(key, "abcd");
    strcpy(value, "eletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftw"); 

    establish_connection(group_id, secret);
    put_value(key, value);
    strcpy(key, "1234eletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftw");
    strcpy(value, "tsboateletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftw");
    put_value(key, value);
    strcpy(key, "12345eletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftw");
    strcpy(value, "tsboattsboat");
    put_value(key, value);
    strcpy(key, "123456eletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftw");
    strcpy(value, "tsboateletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftweletroftw");
    put_value(key, value);
    //get_value(key, msg);
    //delete_value(key);
    get_value(key, msg);

    close_connection();

    return(0);
}
