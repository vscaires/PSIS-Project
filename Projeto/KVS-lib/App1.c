#include "KVS-lib.h"


int main(){
    char *group_id, *secret;
    char *value, *key;
    char ** msg;

    value = "1234";
    key = "asdf";
    int sock; 

    establish_connection(group_id, secret);
    put_value(key, value);
    close_connection();

    return(0);
}
