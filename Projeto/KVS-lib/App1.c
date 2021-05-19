#include <stdlib.h>
#include "KVS-lib.h"


int main(){
    char *group_id, *secret;
    char *value, *key;

    value = "1234";
    key = "asdf";

    if(establish_connection("group_id", "secret") < 0)
        exit(-1);
    
    put_value(key, value);


    return(0);
}