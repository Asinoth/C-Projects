#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NO_BLOCK -1
#define LINE_LENGTH 64


struct FBlock 
{
         int *FreeBL; // array of M elements,
                      // where M is defined by the user
         struct FBlock *next;
};
struct FBlock *FreeBlockPtr;


struct INode
{
       int id;
       int size;
       struct BNode *BL;
};
struct INode *ID;

struct DirNode 
{
       char *name;
       struct INode *inp;
       struct DirNode *next;
};
struct DirNode *FileNode;
// DEN TO EXEIS ELEGKSEI ! ! ! ! ! ! ! 
struct DirNode *createIDnode (struct DirNode *filenode, int fsize, int &IDnum)
{
//     struct INode *newnode;
       
       ID = (struct INode*) malloc (sizeof (struct INode) );
       ID->size = fsize;
       ID->id = IDnum;
       IDnum++;
       /*sundew to fileNode me to komvo delta */
       filenode->inp = ID;
       
       return filenode; 
}
// dhmiourgia komvou ths listas DirNode
struct DirNode *InsertDirNode( struct DirNode *filenode, char *fname )
{
       struct DirNode *tmpfilenode, *newnode, *previous;

       newnode = (struct DirNode*) malloc (sizeof (struct DirNode) );
       newnode->name = fname;

       tmpfilenode = filenode;
       previous = NULL;
       
       if ( (filenode == NULL) || ( strcmp(newnode->name, tmpfilenode->name) <= 0 ) )
       {//gia insert sth prwth 8esh
            newnode->next = filenode;
            printf("To file kataxwrh8hke epityxws.\n");
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
       
       return filenode;

}

/*Dhmiourgia/diavasma listas FreeBlock*/
struct FBlock *FreeBL_Read(struct FBlock * FreeBlockPtr, long M, int bId, FILE *fp)
{
     long i=0;
     struct FBlock  *tmp;

     tmp = (struct FBlock *) malloc (sizeof (struct FBlock) );
	 tmp->FreeBL = (int*) malloc(M * sizeof(int));
	 FreeBlockPtr = tmp;
     
	 while(fscanf(fp, "%d", &bId) > 0)
     {
		/* place your code for every new block that
		  is declared as free. */
	    tmp->FreeBL[i] = bId;
//	    printf("%d", TMP_FreeBlockPtr->FreeBL[i]);
        i++;	 
		if (i == M){
              i=0;
              
              tmp->next = (struct FBlock *) malloc (sizeof (struct FBlock) );
              tmp = tmp->next;
              tmp->FreeBL = (int*) malloc(M * sizeof(int));
        } 
  
	 }
	 for (i; i<M; i++)
         tmp->FreeBL[i] = NO_BLOCK;

	 tmp->next = NULL;  
      
     return FreeBlockPtr;
}	

/* Print tou DirNode */
void PrintDirNode( struct DirNode *filenode)
{
       struct DirNode *tmpfilenode;
       tmpfilenode = filenode;
       
       while (tmpfilenode != NULL)
       {
             printf("The name file is: %s\n", tmpfilenode->name);
             printf("The ID is %d and the block size %d .\n", tmpfilenode->inp->id, tmpfilenode->inp->size );
             tmpfilenode = tmpfilenode->next;
       }
}
/* Print tou FreeBlock */ 
void FreeBL_Print(struct FBlock *FreeBlockPtr, long M)
{
     long i=0;
     struct FBlock *tmp; 
     tmp = FreeBlockPtr;
    
     while (tmp != NULL)
     {
    
           for (i=0; i<M; i++)
           {
               printf("%d ", tmp->FreeBL[i] );
           }
           printf("\n");
           tmp = tmp->next;
           
     }
     
}

/*Delete tou stoixeiou bId apo th lista FreeBlock */
struct FBlock *Delete_bId (struct FBlock * FreeBlockPtr, int bId, long M)
{
     long i=0, allempty=0;
     struct FBlock *tmp; 
     tmp = FreeBlockPtr;
    
     while (tmp != NULL)
     {
    
           for (i=0; i<M; i++)
           {
               if (tmp->FreeBL[i] == bId)
               {
                  tmp->FreeBL[i] = NO_BLOCK;
                  allempty++;
               }
               else if(tmp->FreeBL[i] == NO_BLOCK)
                  allempty++;
           }
           
           if (allempty == M)
           {   
              //delete_node(tmp); //8elei ftiaksimo.
              printf("Node Deleted.\n");
           }
          
           tmp = tmp->next;
           
     } 
     return tmp;
}

/*Insert tou stoixeiou bId sth lista FreeBlock */
struct FBlock *add_bId (struct FBlock * FreeBlockPtr, int bId, long M)
{
     long i=0;
     struct FBlock *tmp; 
     tmp = FreeBlockPtr;
    
     while (tmp != NULL)
     {
    
           for (i=0; i<M; i++)
           {
               if (tmp->FreeBL[i] == NO_BLOCK)
               {
                  tmp->FreeBL[i] = bId;
                  return tmp;
               }
           }
           tmp = tmp->next;
           
     }
     tmp = (struct FBlock *) malloc ( sizeof(struct FBlock) );
     tmp->FreeBL = (int*) malloc(M * sizeof(int));
     tmp->next = NULL;
     
     tmp->FreeBL[0] = bId; // if all arrays are full create a new node
     for (i=1; i<M; i++)
         tmp->FreeBL[i] = NO_BLOCK;
     
     return tmp;
}


int main(int argc, char** argv)
{
	FILE *fp = NULL;
	char line[LINE_LENGTH], event, fname1[LINE_LENGTH], fname2[LINE_LENGTH], fname[LINE_LENGTH], * p_end;
    int IDnum =0;
	int fsize;
	int bId;
	long block_size, fblock_M, bnode_K;

	/* Check command line arguments */
 
	if ( argc != 5 )	{
		printf("Usage: %s <BLOCK_SIZE> <M> <K> <input_file> \n", argv[0]);
		//getchar();
		return EXIT_FAILURE;
	}

	/* Open input file */
 	if (( fp = fopen(argv[4], "r") ) == NULL ) {
		printf("\n Could not open file: %s\n", argv[0 ]);
        //getchar();
		return EXIT_FAILURE;
	}

	/* Read arguments from command line*/
	block_size = strtol(argv[1], &p_end, 10);
	fblock_M = strtol(argv[2], &p_end, 10);
	bnode_K = strtol(argv[3], &p_end, 10);

	/* Read free blocks until -1*/
	FreeBlockPtr = FreeBL_Read(FreeBlockPtr, fblock_M, bId, fp);

/*	
	Delete_bId (FreeBlockPtr, 4, fblock_M); 
	Delete_bId (FreeBlockPtr, 2, fblock_M); //test delete. 
   	printf("kai meta to delete:");
	FreeBL_Print(FreeBlockPtr, fblock_M );
*/	
/*
    add_bId (FreeBlockPtr, 10, fblock_M);  //test insert.
    FreeBL_Print(FreeBlockPtr, fblock_M );
*/

	/* Read input file line-by-line and handle the events */
	while( fgets(line, LINE_LENGTH, fp) != NULL )
	{
		/* Un-comment the line below when in debug mode */
		//printf("\n Event: %s ", line);
		switch(line[0])
		{
		/* Create new file */
		case 'C':
			sscanf(line, "%c %s %d", &event, fname, &fsize);
			/* put your code here */
			FileNode = InsertDirNode(FileNode,fname);
			FileNode = createIDnode (FileNode, fsize, IDnum);
			
			break;

		/* Delete a file */
		case 'D':
			sscanf(line, "%c %s", &event, fname);
			/* put your code here */
			break;

		/* Read a file */
		case 'R':
			sscanf(line, "%c %s", &event, fname);
			/* put your code here */
			break;

		/* Merge files */
		case 'M':
			sscanf(line, "%c %s %s %s", &event, fname1, fname2, fname);

			/* put your code here */
			break;

		/* Copy one file to another */
		case 'O':
			sscanf(line, "%c %s %s", &event, fname1, fname);

			/* put your code here */
			break;

		/* Print internal structure */
		case 'P':
			/* put your code here */
            /* Print all free blocks*/
            FreeBL_Print(FreeBlockPtr, fblock_M );
            /* Print all Dir Nodes */
			PrintDirNode(FileNode);
			break;

		/* Ignore everything else */
		default:
		/* Uncomment the line below when in debug mode */
			/* printf("\n Ignoring line: %s", line); */

			break;
		}
	}
	printf("\n");
	
	return (EXIT_SUCCESS);
}

