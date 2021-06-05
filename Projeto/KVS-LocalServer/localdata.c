# include "localserver.h"
// search, remove, change value , read value

groupsecret *groupsecret_head;

groupsecret *initList(void)
{
    return NULL;
}

void free_All(groupsecret* groupsecret_head)
{
    groupsecret *aux, *newhead;

    for (aux = groupsecret_head; aux!=NULL; aux=newhead){
        newhead = aux->nextgroup;
        free_keyvalue_List(aux->head_2ndlist);
        free(aux->group_id);
        free(aux->secret);
        free(aux);
    }
    return;
}

void free_keyvalue_List(keyvalue* head_2ndlist)
{
    keyvalue *aux, *newhead;

    for (aux = head_2ndlist; aux!=NULL; aux=newhead){
        newhead = aux->next;
        free(aux->value);
        free(aux->key);
        free(aux);
    }
    return;
}

keyvalue *insertNew_keyvalue(keyvalue* head_2ndlist,char* key,char* value,int err)
{
    keyvalue *new;

    new = (keyvalue*) malloc(sizeof(keyvalue));

    if(new!=NULL)
    {
        // new->key = (char*) malloc(sizeof(key));
        new->value = (char*) malloc(sizeof(value));
        new->next = head_2ndlist;
        strcpy(new->key,key);
        strcpy(new->value,value);
        head_2ndlist = new;
        err = 0;
    }else
    {
        err = 1;
    }
        return head_2ndlist;

}

groupsecret *insertNew_group(groupsecret* groupsecret_head,char* group,char* secret,int err)
{
    groupsecret *new;

    new = (groupsecret*) malloc(sizeof(groupsecret));
    if(new!=NULL)
    {
        // new->group_id = (char*) malloc(sizeof(group));
        strcpy(new->group_id,group);
        // new->secret = (char*) malloc(sizeof(secret));
        strcpy(new->secret, secret);
        new->head_2ndlist = NULL;
        new->nextgroup = groupsecret_head;
        groupsecret_head = new;
        err = 0;
    }else
    {
        err = 1;
    }
        return groupsecret_head;
}

//Receives group head and returns number of key-value pairs stored in that group
int countKeys_perGroup(groupsecret* groupsecret_head)
{
    keyvalue* aux;
    int count = 0;
    aux = groupsecret_head->head_2ndlist;

    for (aux=groupsecret_head->head_2ndlist; aux!=NULL; aux= aux->next)
        count++;

    return count;
}

char* read_value(keyvalue* p)
{
 return p->value;
}

keyvalue* change_value(keyvalue* p, char* newvalue)
{
    strcpy(p->value,newvalue);
    return p; 
}

// needs to be diferent from search beacuse we need to store the previous node of the list
void remove_keyvalue_pair(keyvalue* head_2ndList, char* key_asked)
{
    keyvalue* temp = head_2ndList;
    keyvalue* previous = NULL;

    if (temp != NULL && (strcmp(temp->key,key_asked)==0))
    {
        head_2ndList = temp->next;
        free(temp->value);
        free(temp->key);
        free(temp);
        return;
    }
    else
    {
        while(temp != NULL && (strcmp(temp->key,key_asked)==0))
        {
            previous = temp;
            temp = temp->next;

        }
        if (temp == NULL)
        {
            return;
        }
        previous->next = temp->next;
        free(temp->value);
        free(temp->key);
        free(temp);
    }
}

//receives key value list head and a key to look for, returns pointer to group if found or -1 if not found
void* search_keyvalue(keyvalue* head_2ndList,char* key_asked)
{
    keyvalue* aux;
    aux = head_2ndList; 

    while(aux!=NULL){
        if(strcmp(aux->key,key_asked)==0){
            return aux->value;
        }

        aux = aux->next;

    }
    return NULL; 
}

//receives groups head and an ID to look for, returns pointer to group if found or -1 if not found
groupsecret* search_group(groupsecret* groupsecret_head, char* group_ID_asked)
{
   groupsecret* aux;
    aux = groupsecret_head; 

    while(aux!=NULL){
        if(strcmp(aux->group_id,group_ID_asked)==0){
            return aux;
        }

    aux = aux->nextgroup;

    }
    return NULL; // make warning about key not found 
}

groupsecret *deleteGroup(groupsecret **groupsecret_head, char *group)
{
    // Store head node
    groupsecret *temp = *groupsecret_head, *prev;
 
    // If head node itself holds the key to be deleted
    if (temp != NULL && strcmp(temp->group_id,group) == 0) {
        *groupsecret_head = temp->nextgroup; // Changed head
        free(temp); // free old head
        return *groupsecret_head;
    }
 
    // Search for the key to be deleted, keep track of the
    // previous node as we need to change 'prev->next'
    while (temp != NULL && strcmp(temp->group_id,group) != 0) {
        prev = temp;
        temp = temp->nextgroup;
    }
 
    // If key was not present in linked list
    if (temp == NULL)
        return *groupsecret_head;
 
    // Unlink the node from linked list
    prev->nextgroup = temp->nextgroup;
 
    free(temp); // Free memory

    return *groupsecret_head;
}
