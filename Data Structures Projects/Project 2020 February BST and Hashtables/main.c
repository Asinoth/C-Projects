

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <ctype.h>


#define BUFFER_SIZE 1024 /* Maximum length of a line in input file */

/* Uncomment the following line to enable debugging prints
 * or comment to disable it */
#define DEBUG
#ifdef DEBUG
#define DPRINT(...) fprintf(stderr, __VA_ARGS__);
#else /* DEBUG */
#define DPRINT(...)
#endif /* DEBUG */

/**
 * @brief Parsing event arguments from buffer
 *
 * @param buff String of current line in the test file
 * @param int_arr_size Pointer to integer that represents the number of arguments in the test after the Event char
 *
 * @return 0 on success
 *         1 on failure
 * 
 *
 */

#define MG 64

struct Group* InfoGroups[MG];
struct SubInfo* Hashtable[10];
int Hashtablesize;
int Hashprimenumber;



struct Info {
	int iId;
	int itm;
	int igp[MG];
	struct Info* ilc;
	struct Info* irc;
	struct Info* ip;
};
struct Subscription {
	int sId;
	struct Subscription* snext;
};
struct Group {
	int gId;
	struct Subscription* gsub;
	struct Info* gr;
};
struct SubInfo {
	int sId;
	int stm;
	struct TreeInfo* tgp[MG];
	struct TreeInfo* sgp[MG];
	struct SubInfo* snext;
};
struct TreeInfo {
	int tId;
	int ttm;
	struct TreeInfo* tlc;
	struct TreeInfo* trc;
	struct TreeInfo* tp;
	struct TreeInfo* next;
};




void deleteSubscription(struct Subscription** head_ref, int key)
{
	// Store head node
	struct Subscription* temp = *head_ref, * prev;

	// If head node itself holds the key to be deleted
	if (temp != NULL && temp->sId == key) {
		*head_ref = temp->snext; // Changed head
		free(temp); // free old head
		return;
	}

	// Search for the key to be deleted, keep track of the
	// previous node as we need to change 'prev->next'
	while (temp != NULL && temp->sId != key) {
		prev = temp;
		temp = temp->snext;
	}

	// If key was not present in linked list
	if (temp == NULL)
		return;

	// Unlink the node from linked list
	prev->snext = temp->snext;

	free(temp); // Free memory
}


void deleteSubscriberFromHash(struct SubInfo** head_ref, int key)
{
	// Store head node
	struct SubInfo* temp = *head_ref, * prev;
	
	// If head node itself holds the key to be deleted
	if (temp != NULL && temp->sId == key) {
		*head_ref = temp->snext; // Changed head
		free(temp); // free old head

		return;
	}

	// Search for the key to be deleted, keep track of the
	// previous node as we need to change 'prev->next'
	while (temp != NULL && temp->sId != key) {
		prev = temp;
		temp = temp->snext;
	}

	// If key was not present in linked list
	if (temp == NULL)
		return;

	// Unlink the node from linked list
	prev->snext = temp->snext;

	free(temp); // Free memory
}




// A utility function to do inorder traversal of BST
void inorder(struct Info* root)
{
	if (root == NULL)
		return;
	inorder(root->ilc);
	printf("iId%d,", root->iId);
	inorder(root->irc);
}


void printSubscriptions(struct Subscription* head) {
	struct Subscription* current_node = head;
	while (current_node != NULL) {
		printf("sId%d,", current_node->sId);
		current_node = current_node->snext;
	}
}


void printSubscribers() {

	for (int i = 0; i < Hashtablesize; i++)
	{


		if (Hashtable[i] != NULL)
		{
			printf("HashtableList[%d]=<", i);
			struct SubInfo* parser = Hashtable[i];
			while (parser != NULL)
			{
				printf("sId%d>,", parser->sId);
				parser = parser->snext;
			}
			printf("\n");
		}

		else
			printf("HashtableList[%d] = EMPTY\n", i);
	}

	}


struct Info* NodeToFix;
void fixtable(struct Info* info, int* arr, int size_of_arr)
{

	for (int i = 0; i < size_of_arr - 1; i++)
		info->igp[arr[i]] = 1;

}





struct Group* newGNode(int GroupID)
{
	struct Group* temp
		= (struct Group*)malloc(sizeof(struct Group));
	temp->gId = GroupID;
	temp->gr = NULL;
	temp->gsub = NULL;
	return temp;
}


struct Subscription* newSubscription(int SubID)
{
	struct Subscription* temp
		= (struct Subscription*)malloc(sizeof(struct Subscription));
	temp->sId = SubID;
	temp->snext = NULL;
	return temp;
}



// A utility function to create a new BST node
struct Info* newInfo(int ID, int timpestamp)
{
	struct Info* temp
		= (struct Info*)malloc(sizeof(struct Info));
	temp->iId = ID;
	temp->itm = timpestamp;
	for (int i = 0; i < MG; i++)
		temp->igp[i] = 0;
	temp->ilc = temp->irc = NULL;
	temp->ip = NULL;
	return temp;
}



struct SubInfo* newSubscriber(int ID, int timestamp)
{
	struct SubInfo* temp
		= (struct SubInfo*)malloc(sizeof(struct SubInfo));
	temp->sId = ID;
	temp->stm = timestamp;
	temp->snext = NULL;
	for (int i = 0; i < MG; i++)
		temp->sgp[i] = NULL;

	return temp;
}





int Hash_Func()
{

	int lower = 1, upper = Hashprimenumber - 1;

	int Hash_a, Hash_b;

	Hash_a = (rand() % (upper - lower + 1)) + lower;

	lower = 0;
	int key = (rand() % (upper - lower + 1)) + lower;

	Hash_b = (rand() % (upper - lower + 1)) + lower;

	int ChainInTable = ((Hash_a * key + Hash_b) % Hashprimenumber) % Hashtablesize;

	return ChainInTable;


}



/* A utility function to insert
   a new node with given key in
 * BST */
struct Info* insertInfoTree(struct Info* node, int ID, int timestamp)
{
	/* If the tree is empty, return a new Node */
	if (node == NULL)
	{
		
		node =  newInfo(ID, timestamp);
		NodeToFix = node;
		return node;
	}

	/* Otherwise, recur down the tree */
	if (ID < node->iId)
	{
		struct Info* lchild = insertInfoTree(node->ilc, ID, timestamp);
		node->ilc = lchild;

		// Set parent of root of left subtree
		lchild->ip = node;
	}
	else if (ID > node->iId)
	{
		struct Info* rchild = insertInfoTree(node->irc, ID, timestamp);
		node->irc = rchild;

		// Set parent of root of right subtree
		rchild->ip = node;
	}

	/* return the (unchanged) Node pointer */
	return node;
}


struct Subscription insertSubscription(struct Subscription** head, struct Subscription* Sub)//function to insert data in sorted position
{
	//If linked list is empty
	if (*head == NULL || (*head)->sId >= Sub->sId)
	{
		Sub->snext = *head;
		*head = Sub;
		return;
	}

	//Locate the node before insertion
	struct Subscription* current = *head;
	while (current->snext != NULL && current->snext->sId < Sub->sId)
		current = current->snext;

	Sub->snext = current->snext;
	current->snext = Sub;

}


struct SubInfo* insertSubscriber(struct SubInfo* Subscriber, int ChainInTable)//function to insert data in sorted position
{
	//If linked list is empty
	if (Hashtable[ChainInTable] == NULL)
	{
		Subscriber->snext = Hashtable[ChainInTable];
		Hashtable[ChainInTable] = Subscriber;
		return;
	}

	//Locate the node before insertion
	struct SubInfo* current = Hashtable[ChainInTable];
	while (current->snext != NULL && current->snext->sId < Subscriber->sId)
		current = current->snext;

		Subscriber->snext = current->snext;
		current->snext = Subscriber;


}




struct SubInfo* newHashEntry(int ChainInTable, struct SubInfo* subscriber)
{
	insertSubscriber(subscriber, ChainInTable);

}


/**
 * @brief Optional function to initialize data structures that
 *        need initialization
 *
 * @param m Size of hash table
 * @param p Prime number for the universal hash function
 *
 * @return 0 on success
 *         1 on failure
 */
struct SubInfo* initialize(int m, int p)
{

	for (int i = 0; i < m; i++)
	{
		Hashtable[i] = NULL;

	}
		struct SubInfo* HashStart = Hashtable[0];
		return HashStart;

}


/**
 * @brief Free resources
 *
 * @return 0 on success
 *         1 on failure
 */
int free_all(void)
{

	return 1;

}


/**
 * @brief Insert info
 *
 * @param iTM Timestamp of arrival
 * @param iId Identifier of information
 * @param gids_arr Pointer to array containing the gids of the Event.
 * @param size_of_gids_arr Size of gids_arr including -1
 * @return 0 on success
 *          1 on failure
 */
int Insert_Info(int iTM, int iId, int* gids_arr, int size_of_gids_arr)
{
	

	for (int i = 0; i < size_of_gids_arr-1; i++)
	{
		
		int tablenum = gids_arr[i];
		struct Info* info;
		struct Group* Table = InfoGroups[tablenum];
		if(Table->gr == NULL)
		info = Table->gr = insertInfoTree(Table->gr, iId, iTM);
		else
			info = insertInfoTree(Table->gr, iId, iTM);

		fixtable(NodeToFix, gids_arr, size_of_gids_arr);

	}

	for (int i = 0; i < size_of_gids_arr-1; i++)
	{
		int tablenum = gids_arr[i];
		struct Group* Table = InfoGroups[tablenum];
		printf("\nGroupID = <%d>, ", Table->gId);
		printf("INFOLIST = <");
		inorder(Table->gr);
		printf(">\n");
	}



	return 0;

}


/**
 * @brief Subsriber Registration
 *
 * @param sTM Timestamp of arrival
 * @param sId Identifier of subscriber
 * @param gids_arr Pointer to array containing the gids of the Event.
 * @param size_of_gids_arr Size of gids_arr including -1
 * @return 0 on success
 *          1 on failure
 */
int Subscriber_Registration(int sTM, int sId, int* gids_arr, int size_of_gids_arr)
{
	struct SubInfo* Subscriber = newSubscriber(sId, sTM);
	int ChainTable = Hash_Func();
	newHashEntry(ChainTable, Subscriber);

	for (int i = 0; i < size_of_gids_arr - 1; i++)
	{
		
		int tablenum = gids_arr[i];
		struct Group* Table = InfoGroups[tablenum];
		struct Subscription* info;
		struct Subscription* SUB = newSubscription(sId);
		insertSubscription(&Table->gsub, SUB);
	}


	for (int i = 0; i < size_of_gids_arr - 1; i++)
	{
		int tablenum = gids_arr[i];
		struct Group* Table = InfoGroups[tablenum];
		printf("\nGroupID = <%d>, ", Table->gId);
		printf("SUBLIST = <");
		printSubscriptions(Table->gsub);
		printf(">\n");
	}

		printf("\n");
		printSubscribers();
	

	return 0;
}


/**
 * @brief Prune Information from server and forward it to client
 *
 * @param tm Information timestamp of arrival
 * @return 0 on success
 *          1 on failure
 */
int Prune(int tm)
{

	return 1;

}


/**
 * @brief Consume Information for subscriber
 *
 * @param sId Subscriber identifier
 * @return 0 on success
 *          1 on failure
 */
int Consume(int sId)
{

	return 1;

}


/**
 * @brief Delete subscriber
 *
 * @param sId Subscriber identifier
 * @return 0 on success
 *          1 on failure
 */
int Delete_Subscriber(int sId)
{

	struct Group* TableFinder;
	for (int i = 0; i < 63; i++)
	{
		TableFinder = InfoGroups[i];
		deleteSubscription(&TableFinder->gsub, sId);
	}


	struct SubInfo* HashFinder;

	for (int i = 0; i < Hashtablesize; i++)
	{
		HashFinder = Hashtable[i];
		deleteSubscriberFromHash(&HashFinder, sId);

	}

	struct Group* printer;

	for (int i = 0; i < MG; i++)
	{
		printer = InfoGroups[i];
		printf("\nGroupID = <gId%d>, ", printer->gId);
		printf("SUBLIST = <");
		printSubscriptions(printer->gsub);
		printf(">\n");

	}

	printf("\n");
	printSubscribers();


	return 0;

}

/**
 * @brief Print Data Structures of the system
 *
 * @return 0 on success
 *          1 on failure
 */
int Print_all(void)
{

	struct Group* printer;

	for (int i = 0; i < MG; i++)
	{
		printer = InfoGroups[i];
		printf("\nGroupID = <gId%d>, ", printer->gId);
		printf("INFOLIST = <");
		inorder(printer->gr);
		printf(">, ");
		printf("SUBLIST = <");
		printSubscriptions(printer->gsub);
		printf(">\n");
		
	}
	printf("\n");
	printSubscribers();
	


	return 0;

}






static int * event_args(char *buff,unsigned int * int_arr_size){
    int * int_arr;
    char event='\0';
    int value;
    char *p =buff+1;
    unsigned int len = 0;
    unsigned int count_args=1;
    unsigned int i =0;
    sscanf(buff, "%c", &event);
    while (sscanf(p, "%d%n", &value, &len)==1 && value!=-1 ) {
        count_args++;
        p += len;
    }
    p=buff+1;
    (*int_arr_size)=count_args;
    int_arr= malloc(count_args*sizeof(int));
    while (sscanf(p, "%d%n", &value, &len)==1) {
        int_arr[i++]=value;
        p += len;
        if(value ==-1){
            break;
        }
    }

    return int_arr;
}

/**
 * @brief The main function
 *
 * @param argc Number of arguments
 * @param argv Argument vector
 *
 * @return 0 on success
 *         1 on failure
 */
int main(int argc, char **argv)
{
	FILE *fin = NULL;
	char buff[BUFFER_SIZE], event='\0';

	for (int i = 0; i < argc; ++i)
	{
		printf("argv[%d]: %s\n", i, argv[i]);
	}

	/* Check command buff arguments */
	if (argc != 4)
	{
		fprintf(stderr, "Usage: %s <m> <p> <input_file>\n", argv[0]);
		return EXIT_FAILURE;
	}

	/* Parse command buff arguments */
	int hashTableSize = atoi(argv[1]);
	int universalHashingNumber = atoi(argv[2]);

	if (hashTableSize <= 0)
	{
		fprintf(stderr, "\n Invalid hash table size: %d\n", hashTableSize);
		perror("Parsing command line argument\n");
		return EXIT_FAILURE;
	}

	if (universalHashingNumber <= 0)
	{
		fprintf(stderr, "\n Invalid universal hashing number: %d\n", universalHashingNumber);
		perror("Parsing command line argument\n");
		return EXIT_FAILURE;
	}

	/* Open input file */
	if ((fin = fopen(argv[3], "r")) == NULL)
	{
		fprintf(stderr, "\n Could not open file: %s\n", argv[3]);
		perror("Opening test file\n");
		return EXIT_FAILURE;
	}


	/* Initializations */
	Hashtablesize = hashTableSize;
	Hashprimenumber = universalHashingNumber;
	initialize(hashTableSize, universalHashingNumber);

	for (int i = 0; i < MG; i++)
	{
		InfoGroups[i] = newGNode(i);
		printf("\nGroup%d is initialized\n", InfoGroups[i]->gId);

	}


	/* Read input file buff-by-buff and handle the events */
	while (fgets(buff, BUFFER_SIZE, fin))
	{
		DPRINT("\n>>> Event: %s\n", buff);
		switch (buff[0])
		{
			

		/* Comment */
		case '#':
			break;

		/* Insert_Info
		 * I <itm> <iId> <gId1> <gId2> ... <gIdk> -1 */
		case 'I':
		{
			int itm;
			int iId;
            unsigned int num_of_gids;
            int * event_args_arr;
            int * gids_arr;
            event_args_arr=event_args(buff, &num_of_gids);
            itm=event_args_arr[0];
            iId=event_args_arr[1];
            gids_arr= event_args_arr+2;
            num_of_gids-=2;
			if (Insert_Info(itm,iId,gids_arr,num_of_gids)==0)
			{
				DPRINT("%c <%d> <%d> DONE\n",buff[0], itm,iId);
			}
			else
			{
				fprintf(stderr, "%s failed\n", buff);
			}
            num_of_gids=0;
            free(event_args_arr);
			break;
		}

		/* Subscriber_Registration
		 * S <s??> <sId> <gId1> <gId2> ... <gIdm> -1  */
		case 'S':
		{
            int sTM, sId;
            unsigned int num_of_gids;
            int * event_args_arr;
            int * gids_arr;
            event_args_arr=event_args(buff, &num_of_gids);
            sTM=event_args_arr[0];
            sId=event_args_arr[1];
            gids_arr= event_args_arr+2;
            num_of_gids-=2;
            if (Subscriber_Registration(sTM,sId,gids_arr,num_of_gids)==0)
            {
                DPRINT("%c <%d> <%d> DONE\n", buff[0],sTM,sId);
            }
            else
            {
                fprintf(stderr, "%s failed\n", buff);
            }
            num_of_gids=0;
            free(event_args_arr);
			break;
		}

		/* Prune
		 * R <tm> */
		case 'R':
		{
            int tm;
			sscanf(buff, "%c %d", &event, &tm);
			if (Prune(tm)==0)
			{
				DPRINT("%c <%d> DONE\n", event, tm);
			}
			else
			{
				fprintf(stderr, "%c %d failed\n", event, tm);
			}
			break;
		}

		/* Consume
		 * C <sId> */
		case 'C':
		{
            int sId;
			sscanf(buff, "%c %d", &event, &sId);
			if (Consume(sId)==0)
			{
				DPRINT("%c <%d> DONE\n", event,sId);
			}
			else
			{
				fprintf(stderr, "%c %d failed\n", event,sId);
			}
			break;
		}

		/* Delete_Subscriber
		 * D <sId>: */
		case 'D':
		{
			int sId;
			sscanf(buff, "%c %d", &event, &sId);
            if (Delete_Subscriber(sId)==0)
			{
				DPRINT("%c <%d> DONE\n", event, sId);
			}
			else
			{
				fprintf(stderr, "%c %d failed\n", event, sId);
			}
			break;
		}

		/* Print
		 * P */
		case 'P':
		{
			if (Print_all()==0)
			{
				DPRINT("%c DONE\n", buff[0]);
			}
			else
			{
				fprintf(stderr, "%c failed\n", buff[0]);
			}

			break;
		}
		/* Empty line */
		case '\n':
			break;
		/* Ignore everything else */
		default:
			DPRINT("Ignoring line: %s \n", buff);
			break;
		}
	}

	free_all();
	return (EXIT_SUCCESS);
}
