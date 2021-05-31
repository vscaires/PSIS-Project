#include "localserver.h"

int main(){

    int client_socket, i = 0;
    char key[256], c[1], *value;
    struct sockaddr_un server_addr;
    Dict kvs = DictCreate();

    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    /* Clear structure */
    server_addr.sun_family = AF_UNIX;
    /* UNIX domain address */
    strcpy(server_addr.sun_path, SOCKNAME);

    unlink(SOCKNAME);
    int server_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if(server_sock == -1){
        perror("socket creation");
        exit(-1);
    }
    if(bind(server_sock, (struct sockaddr *) &server_addr, sizeof(struct sockaddr_un)) == -1){
        perror("bind");
        exit(-1);
    }
    if(listen(server_sock, 5) == -1){
        perror("Listen");
        exit(-1);
    }

    while(1){
        client_socket = accept(server_sock, NULL, NULL);
        printf("accepted one client -> new socket %d\n", client_socket);

        read(client_socket, &i, sizeof(i)); 
        do{
            if(i == 1){ //put_value
                read(client_socket, &i, sizeof(i));
                read(client_socket, key, i);
                
                read(client_socket, &i, sizeof(i));
                value = malloc(i*sizeof(char));
                read(client_socket, value, i);
                
                if(DictSearch(kvs, key) == 0 || strcmp(DictSearch(kvs, key), value) != 0){
                    DictInsert(kvs, key, value);
                    printf("Key-value added\n");
                }else{
                    printf("Key-value already exists\n");   
                }
                memset(key, 0, sizeof(key));
                memset(value, 0, sizeof(value));

            }else if(i == 2){ // get_value
                read(client_socket, &i, sizeof(i));
                read(client_socket, key, i);

                if(DictSearch(kvs, key) == 0){
                    i = -1;
                    write(client_socket, &i, sizeof(i));
                    printf("Key-value not found ...\n");
                }else{
                    printf("Value : %s\n", DictSearch(kvs, key));
                    i = strlen(DictSearch(kvs, key));
                    write(client_socket, &i, sizeof(i));
                    write(client_socket, DictSearch(kvs, key), i);
                    printf("Key-value found!\n");   
                }
                memset(key, 0, sizeof(key));
                memset(value, 0, sizeof(value));

            }else if(i == 3){ // delete_value
                read(client_socket, &i, sizeof(i));
                read(client_socket, key, i);

                if(DictSearch(kvs, key) == NULL){
                    printf("Key-value not found ...\n");
                }else{
                    DictDelete(kvs, key);
                    printf("Key-value found and deleted!\n");   
                }
            }
        }while(read(client_socket, &i, sizeof(i)) > 0);
        
        close(client_socket);
        printf("Socket closed\n");
    }
    DictDestroy(kvs);
    printf("Key-Value data destroyed! \n");

    return 0;
}

struct elt {
    struct elt *next;
    char *key;
    char *value;
};

struct dict {
    int size;           /* size of the pointer table */
    int n;              /* number of elements stored */
    struct elt **table;
};

#define INITIAL_SIZE (1024)
#define GROWTH_FACTOR (2)
#define MAX_LOAD_FACTOR (1)

/* dictionary initialization code used in both DictCreate and grow */
Dict internalDictCreate(int size)
{
    Dict d;
    int i;

    d = malloc(sizeof(*d));

    assert(d != 0);

    d->size = size;
    d->n = 0;
    d->table = malloc(sizeof(struct elt *) * d->size);

    assert(d->table != 0);

    for(i = 0; i < d->size; i++) d->table[i] = 0;

    return d;
}

Dict DictCreate(void)
{
    return internalDictCreate(INITIAL_SIZE);
}

void DictDestroy(Dict d)
{
    int i;
    struct elt *e;
    struct elt *next;

    for(i = 0; i < d->size; i++) {
        for(e = d->table[i]; e != 0; e = next) {
            next = e->next;

            free(e->key);
            free(e->value);
            free(e);
        }
    }

    free(d->table);
    free(d);
}

#define MULTIPLIER (97)

static unsigned long hash_function(const char *s)
{
    unsigned const char *us;
    unsigned long h;

    h = 0;

    for(us = (unsigned const char *) s; *us; us++) {
        h = h * MULTIPLIER + *us;
    }

    return h;
}

static void grow(Dict d)
{
    Dict d2;            /* new dictionary we'll create */
    struct dict swap;   /* temporary structure for brain transplant */
    int i;
    struct elt *e;

    d2 = internalDictCreate(d->size * GROWTH_FACTOR);

    for(i = 0; i < d->size; i++) {
        for(e = d->table[i]; e != 0; e = e->next) {
            /* note: this recopies everything */
            /* a more efficient implementation would
             * patch out the strdups inside DictInsert
             * to avoid this problem */
            DictInsert(d2, e->key, e->value);
        }
    }

    /* the hideous part */
    /* We'll swap the guts of d and d2 */
    /* then call DictDestroy on d2 */
    swap = *d;
    *d = *d2;
    *d2 = swap;

    DictDestroy(d2);
}

/* insert a new key-value pair into an existing dictionary */
void DictInsert(Dict d, const char *key, const char *value)
{
    struct elt *e;
    unsigned long h;

    assert(key);
    assert(value);

    e = malloc(sizeof(*e));

    assert(e);

    e->key = strdup(key);
    e->value = strdup(value);

    h = hash_function(key) % d->size;

    e->next = d->table[h];
    d->table[h] = e;

    d->n++;

    /* grow table if there is not enough room */
    if(d->n >= d->size * MAX_LOAD_FACTOR) {
        grow(d);
    }
}

/* return the most recently inserted value associated with a key */
/* or 0 if no matching key is present */
const char *DictSearch(Dict d, const char *key)
{
    struct elt *e;

    for(e = d->table[hash_function(key) % d->size]; e != 0; e = e->next) {
        if(!strcmp(e->key, key)) {
            /* got it */
            return e->value;
        }
    }

    return 0;
}

/* delete the most recently inserted record with the given key */
/* if there is no such record, has no effect */
void DictDelete(Dict d, const char *key)
{
    struct elt **prev;          /* what to change when elt is deleted */
    struct elt *e;              /* what to delete */

    for(prev = &(d->table[hash_function(key) % d->size]); 
        *prev != 0; 
        prev = &((*prev)->next)) {
        if(!strcmp((*prev)->key, key)) {
            /* got it */
            e = *prev;
            *prev = e->next;

            free(e->key);
            free(e->value);
            free(e);

            return;
        }
    }
}

