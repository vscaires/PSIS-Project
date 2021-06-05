#include "KVS_AuthServer.h"
//# Functioning details in KVS_AuthServer.h

auth* initList(void)
{
    return NULL;
}
//#####################  Free  ###################################
// void free_all_locals(locals* main_head)
// {
//     locals *aux, *newhead;

//     for (aux = main_head; aux!=NULL; aux=newhead){
//         newhead = aux->next_locals;
//         free_auth_lists(aux->head_auth);
//         free(aux);
//     }
//     return;
// }

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

// locals *insert_new_local(locals* main_head,char local_id_new[6], int err)
// {
//     locals *new;

//     new = (locals*) malloc(sizeof(locals));
//     if(new!=NULL)
//     {
//         strcpy(new->local_id, local_id_new);
//         new->head_auth = NULL;
//         new->next_locals = main_head;
//         main_head = new;
//         err = 0;
//     }else
//     {
//         err = 1;
//     }
//         return main_head;
// }

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

// auth* change_secret(auth* p, char* new_secret)
// {
//     strcpy(p->secret,new_secret);
//     return p; 
// }

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

// locals* search_local(locals* main_head, char * local_asked) 
// {
//     locals* aux;
//     aux = main_head; 

//     while(aux!=NULL){
//         if(strcmp(local_asked, aux->local_id) == 0)
//         {
//             return aux;
//         }

//     aux = aux->next_locals;

//     }
//     return NULL; // make warning about not found 
// }

// //#####################  Extras  ###################################
// int countgroups_perLocal(locals* p)
// {
//     auth* aux;
//     int count = 0;
//     aux = p->head_auth;

//     for (aux=p->head_auth; aux!=NULL; aux= aux->next_auth)
//         count++;

//     return count;
// }

