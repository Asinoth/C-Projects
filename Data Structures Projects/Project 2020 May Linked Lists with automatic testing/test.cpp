// dhmiourgia komvou ths listas DirNode
struct DirNode *InsertDirNode( struct DirNode *filenode, char *fname, int fsize, long K, long M, struct FBlock *FreeBlockPtr)
{
        struct DirNode *tmpfilenode, *newnode, *previous;

        newnode = (struct DirNode*) malloc (sizeof (struct DirNode) );
        newnode->name = strdup(fname);

        newnode = createIDnode (newnode, fsize, K, M, FreeBlockPtr);//vazw to new node na deixnei sto komvo delta

        tmpfilenode = filenode;
        previous = NULL;

        if ( (filenode == NULL) || ( strcmp(newnode->name, tmpfilenode->name) <= 0 ) )
        {//gia insert sth prwth 8esh
            newnode->next = filenode;
            printf("To file kataxwrh8hke epityxws.\n");
//          printf("To name tou komvou:%s \n\n", newnode->name);
//          PrintDirNode(newnode);

            return newnode;
        }
        while ( ( tmpfilenode != NULL) && ( strcmp(newnode->name, tmpfilenode->name) > 0 ) )
        {//gia insert se mia apo tis epomenes 8eseis
             previous = tmpfilenode;
             tmpfilenode = tmpfilenode->next;
        }

        previous->next = newnode;
        newnode->next = tmpfilenode;

        printf("To file kataxwrh8hke epityxws.\n");
//      printf("To name tou komvou:%s \n\n", newnode->name);
//      PrintDirNode( filenode);

        return filenode;

}
