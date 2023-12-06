#include "symtable.h"

void print(const char *pcKey, void *pvValue,void *pvExtra){
    printf("Entry %s -> %s\n",pcKey,(char *)pvValue);
}

int main(){
    SymTable_T table = SymTable_new();
    int i;
    char result[50];
    
    if(SymTable_put(table,"portokali","orange"))
        printf("PASS: Added 1 element\n");
    else 
        printf("FAIL: SymTable_put\n");

    if(!SymTable_put(table,"portokali","orange"))
        printf("PASS: Did not add second instance\n");
    else 
        printf("FAIL: SymTable_put dublicate entry\n");

    if(SymTable_put(table,"kokkino","red"))
        printf("PASS: Added 2 element\n");
    else 
        printf("FAIL: SymTable_put");

    if(SymTable_put(table,"kitrino","yellow"))
        printf("PASS: Added 3 element\n");
    else 
        printf("FAIL: SymTable_put");
    
    if(SymTable_remove(table,"kitrino"))
        printf("PASS: Removed 3rd element\n");
    else 
        printf("FAIL: SymTable_remove failed\n");
        
 
    for(i=0;i<67000;i++){
        sprintf(result, "entry%d", i);
        SymTable_put(table, result ,"valueee");
    }

    if(SymTable_remove(table,"entry850"))
        printf("PASS: Removed entry850 element\n");
    else 
        printf("FAIL: SymTable_remove failed\n");
    
    if(!SymTable_get(table,"entry850"))
        printf("PASS: removed successfully element\n");
    else 
        printf("FAIL: SymTable_remove failed\n");
    
    if(SymTable_get(table, "entry502"))
        printf("PASS: Got entry502: %s element\n",(char*)SymTable_get(table, "entry502"));
    else 
        printf("FAIL: SymTable_get failed\n");

    
    /*SymTable_map(table,print,"");*/
    
    SymTable_free(table);

     return 0;

}