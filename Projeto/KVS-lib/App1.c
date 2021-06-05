#include "KVS-lib.h"



int main(){
    char group_id[256], secret[256];
    char value[256], key[256];
    char ** msg;

    printf("Application 1 initiating ...\n");

    strcpy(group_id, "ola");
    strcpy(secret, "pkDHTxmMR29N3l0k99EmLgN8cCCTt0rWksb2fEBw408vi6Ug2YHC4UAVUAoB0VYjCxYhzZSrWPc6IuZAHeCAyATJA8KAQghSS7yHTEyqRPja87aCbaANbTUbOdZf08vP2hVIlHw4UVRSQrSrFT5gmP72qTUnOD4FlOMKV9DoUS7i8OPDfHjIUd8AxPoBShF4tRCCPFqhYkFVxSWSa5YsXTtIiVI21NsjcujpyONKwQdhh116uMnFgUOCpW4fhkC");
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
