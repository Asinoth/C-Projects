
#include "symtable.h"

#define HASH_MULTIPLIER 65599
#define MAX_HASH 65521
/*
    Hashtable with single linked list
*/
struct SymTable_S{
    struct binding** table;
    unsigned int buckets;
    unsigned int size;
};
struct binding {
    char *key;
    void *value;
    struct binding * next;
};

static int getNextSize(int n){
    switch(n){
        case 509: return 1021;
        case 1021: return 2053;
        case 2053: return 4093;
        case 4093: return 8191;
        case 8191: return 16381;
        case 16381: return 32771;
        case 32771: return MAX_HASH;
    }
    return 0;
}
/* Return a hash code for pcKey. */
static unsigned int SymTable_hash(const char *pcKey,unsigned int buckets){
  size_t ui;
  unsigned int uiHash = 0U;
  for (ui = 0U; pcKey[ui] != '\0'; ui++)
    uiHash = uiHash * HASH_MULTIPLIER + pcKey[ui];
  return uiHash % buckets;
} 


static void expand(SymTable_T oSymTable){
    struct binding *contents,*cur,*next;
    int i;
    if(oSymTable->buckets == MAX_HASH || oSymTable->size < oSymTable->buckets) return;

    contents = NULL; /* transfer all bindings in a list*/
    for(i=0;i<oSymTable->buckets;i++){
        cur = oSymTable->table[i];
        while(cur){
            next = cur->next;
            cur->next = contents;
            contents = cur;
            cur = next;
        }
        oSymTable->table[i] = NULL;
    }

    /*define the next size*/
    oSymTable->buckets =  getNextSize(oSymTable->buckets); 
    oSymTable->table = realloc(oSymTable->table, oSymTable->buckets * sizeof(struct binding));
    assert(oSymTable->table);

    /*add again the bindings*/
    cur = contents;
    while(cur){
        next = cur->next;
        i = SymTable_hash(cur->key,oSymTable->buckets);
        cur->next = oSymTable->table[i];
        oSymTable->table[i] = cur;
        cur = next;
    }
}



SymTable_T SymTable_new(void){
    SymTable_T new = malloc(sizeof(SymTable_T*));
    assert(new);
    new->table = malloc(509 * sizeof(struct binding*));
    new->size = 0;
    new->buckets = 509;
    return new;
}

unsigned int SymTable_getLength(SymTable_T oSymTable){
    assert(oSymTable);
    return oSymTable->size;
}

int SymTable_put(SymTable_T oSymTable, const char *pcKey,const void *pvValue){
    char *newkey;
    struct binding *newnode;
    int position;
    assert(oSymTable && pcKey);
    /*if it's already here return */ 
    if(SymTable_contains(oSymTable,pcKey)==1) return 0; 
    newkey = (char *)malloc(strlen(pcKey)+1);
    assert(newkey);
    strcpy(newkey,pcKey);

    newnode = (struct binding *)malloc(sizeof(struct binding));
    assert(newnode);
    newnode->key = newkey;
    newnode->value = (void *)pvValue;

    position = SymTable_hash(pcKey,oSymTable->buckets);
    newnode->next = oSymTable->table[position];
    oSymTable->table[position] = newnode;
    oSymTable->size++;

    expand(oSymTable);
    return 1;
}

void SymTable_free(SymTable_T oSymTable){
    struct binding *tmp,*next;
    int i;
    if(oSymTable) return;
    for(i=0;i<oSymTable->buckets;i++){
        tmp = oSymTable->table[i];
        while(tmp){
            next = tmp->next;
            free(tmp->key);
            free(tmp);
            tmp = next;
        }
    }
    free(oSymTable->table);
}

int SymTable_remove(SymTable_T oSymTable, const char *pcKey){
    struct binding *prev,*curr;
    assert(oSymTable && pcKey);

    curr = oSymTable->table[SymTable_hash(pcKey,oSymTable->buckets)];
    prev = NULL;
    while(curr){
        if(strcmp(curr->key,pcKey) == 0) break;
        prev = curr;
        curr = curr->next;
    }
    if(curr==NULL) return 0; /*was not found*/

    if(prev == NULL) oSymTable->table[SymTable_hash(pcKey,oSymTable->buckets)] = curr->next;
    else prev->next = curr->next;
    free(curr->key);
    free(curr);
    oSymTable->size--;
    return 1;
}

int SymTable_contains(SymTable_T oSymTable, const char *pcKey){
    struct binding *curr;
    assert(oSymTable && pcKey);

    curr = oSymTable->table[SymTable_hash(pcKey,oSymTable->buckets)];
    while(curr){
        if(strcmp(curr->key,pcKey) == 0) return 1;
        curr = curr->next;
    }   
    
    return 0;
}

void *SymTable_get(SymTable_T oSymTable, const char *pcKey){
    struct binding *curr;
    assert(oSymTable && pcKey);

    curr = oSymTable->table[SymTable_hash(pcKey,oSymTable->buckets)];
    while(curr){
        if(strcmp(curr->key,pcKey) == 0) return curr->value;
        curr = curr->next;
    }   
    
    return NULL;
}

void SymTable_map(SymTable_T oSymTable, 
                    void (*pfApply)(const char *pcKey, void *pvValue,void *pvExtra),
                    const void *pvExtra){
    int i;
    struct binding* cur;
    assert(oSymTable);

    for(i=0;i<oSymTable->buckets;i++){
        cur = oSymTable->table[i];
        while(cur){
           pfApply((const char *)(cur->key),cur->value,(void*)pvExtra);
            cur = cur ->next;
        }
    }
}

