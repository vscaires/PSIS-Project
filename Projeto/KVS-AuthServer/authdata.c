#include "authserver.h"
//# Functioning details in KVS_AuthServer.h

auth* initList(void)
{
    return NULL;
}
//#####################  Free  ###################################


void free_auth_lists(auth* head_auth)
{
    auth *aux, *newhead;

    for (aux = head_auth; aux!=NULL; aux=newhead){
        newhead = aux->next_auth;
        free(aux->group_id);
        free(aux->secret);
        free(aux);
    }
    return;
}

//#####################  Inserts  ###################################
auth *insert_new_auth(auth* head_auth,char* group_new,char* secret_new,int err)
{
    auth *new;

    new = (auth*) malloc(sizeof(auth));
    if(new!=NULL)
    {
        new->group_id = (char*) malloc(sizeof(group_new));
        new->secret = (char*) malloc(sizeof(secret_new));
        new->next_auth = head_auth;
        strcpy(new->group_id,group_new);
        strcpy(new->secret,secret_new);
        head_auth = new;
        err = 0;
    }else
    {
        err = 1;
    }
        return head_auth;
}



//#####################  Read / Change / Remove  ###################################
int authentication(auth* p, char* secret_check)
{
    if(strcmp(p->secret,secret_check)==0){
        return 0;
    }
    else{
        return 1;
    }
}


auth *remove_auth_pair(auth* head_auth, char* group_asked)
{
    auth* temp = head_auth;
    auth* previous = NULL;

    if (temp != NULL && (strcmp(temp->group_id,group_asked)==0))
    {
        head_auth = temp->next_auth;
        free(temp->group_id);
        free(temp->secret);
        free(temp);
        return head_auth;
    }
    else
    {
        while(temp != NULL && (strcmp(temp->group_id,group_asked)==0))
        {
            previous = temp;
            temp = temp->next_auth;

        }
        if (temp == NULL)
        {
            return head_auth;
        }
        previous->next_auth = temp->next_auth;
        free(temp->group_id);
        free(temp->secret);
        free(temp);
    }
    return head_auth;
}


//#####################  Search  ###################################
auth* search_auth(auth* head_auth,char* group_asked)
{
    auth* aux;
    aux = head_auth; 

    while(aux!=NULL){
        if(strcmp(aux->group_id,group_asked)==0){
            return aux;;
        }
        aux = aux->next_auth;
    }
    return NULL; 
}
