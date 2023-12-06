/*************************************************************


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Shopping.h"

#define BUFFER_SIZE 1024  /* Maximum length of a line in input file */

/* Uncomment the following line to enable debugging prints
 * or comment to disable it */
#define DEBUG

#ifdef DEBUG
#define DPRINT(...) fprintf(stderr, __VA_ARGS__);
#else  /* DEBUG */
#define DPRINT(...)
#endif /* DEBUG */


/**
 * @brief Print customers
 *
 * @return 1 on success
 *         0 on failure
 */


void inorder1(struct Shop *root)
{
		if (root != NULL)
		{
				inorder1(root->lc);
				printf("<%d>,", root->sid); //thelei kai type kai products klp
				inorder1(root->rc);
		}
}


void inorder2(struct Shop *root)
{
	if (root != NULL)
	{
			inorder2(root->lc);
			printf("\nShop<%d>=", root->sid);
			struct Product *temp=root->products;
			while(temp->next!=NULL)
			{
				temp=temp->next;
				printf("<%d><%d><%d>,",temp->pid,temp->qty,temp->price); //thelei kai type kai products klp
			}
			inorder2(root->rc);
	}
}

struct Shop* insertShop(struct Shop* node,int key,int type)
{
	if(node==NULL)
	{
		struct Shop* temp=(struct Shop*)malloc(sizeof(struct Shop));
		temp->sid=key;
		temp->type=type;
		temp->products=(struct Product*)malloc(sizeof(struct Product));
		temp->products->next=NULL;
		temp->lc=NULL;
		temp->rc=NULL;
		return temp;
	}

	 if(key<node->sid)
	 {
		 node->lc=insertShop(node->lc,key,type);
	 }
	 else if(key>node->sid)
	 {
		 node->rc=insertShop(node->rc,key,type);
	 }
	 return node;

}

struct C_Product* insertPro(struct C_Product* node,int id,int qty)
{
	if(node==NULL)
	{
		struct C_Product* temp=(struct C_Product*)malloc(sizeof(struct C_Product));
		temp->pid=id;
		temp->qty=qty;
		temp->lc=NULL;
		temp->rc=NULL;
		return temp;
	}

	 if(id<node->pid)
	 {
		 node->lc=insertPro(node->lc,id,qty);
	 }
	 else if(id>node->pid)
	 {
		 node->rc=insertPro(node->rc,id,qty);
	 }
	 return node;
}

int parent(int i)
{
	return (i-1)/2;
}



int has1(int id)
{
		return (id%prime);
}

int has2(int id)
{

	return (prime-(id%prime));
}


int hash_function(int id)
{
	int a = has1(id);
	int b=has2(id);


	if(customers_hash[a]!=NULL)
	{

	if(customers_hash[b]!=NULL)
	{

		int i=1,has;
		has=(a+(i*b))%max_customers_g;
		while(i<max_customers_g&& customers_hash[has]!=NULL)
		{
			i++;
			has=(a+(i*b))%max_customers_g;
			if(customers_hash[has]==NULL)
			{

				return has;
			}


		}
		for(i=0;i<max_customers_g;i++)
		{
			if(customers_hash[i]==NULL)
			{
				return i;
			}
		}


	}

		return b;
	}


	return a;

}




int has_find(int id)
{
	int a = has1(id);
	int b=has2(id);
	int i=1;

	if(customers_hash[a]->cid==id)
	{

		return a;

	}
	else if(customers_hash[b]->cid==id)
	{

		return b;
	}
	else
	{

		int i=1,has;
		has=(a+(i*b))%max_customers_g;
		while(i<max_customers_g&& customers_hash[has]->cid!=id )
		{

			has=(a+(i*b))%max_customers_g;
			if(customers_hash[has]->cid==id)
			{

				return has;
			}
			i++;
	}
	for(i=0;i<max_customers_g;i++)
	{
		if(customers_hash[i]->cid==id)
		{
			return i;
		}
	}
}
return 1;
}


void print_inorder_the_shopping_list(struct C_Product *node){

	if (node != NULL){
			print_inorder_the_shopping_list(node->lc);
			printf("<%d,%d> ", node->pid, node->qty);
			print_inorder_the_shopping_list(node->rc);
	}
}

int PrintCustomers (void)
{
    int i;
    for (i = 0; i < max_customers_g; i++){
        if (customers_hash[i] != NULL){
            printf("\nCustomer<%d> = ",customers_hash[i]->cid);
            print_inorder_the_shopping_list(customers_hash[i]->shopping_tree);
        }
    }

	return 1;
}


/**
 * @brief Optional function to initialize data structures that
 *        need initialization
 *
 * @return 1 on success
 *         0 on failure
 */
int Initialize (void)
{
		shopRoot=NULL;

		int i=0;
		int p=0;

		i = 0;
		prime = 0;
		while(i<=367&& p==0){
			if(primes_g[i]>=max_customers_g){
				prime = primes_g[i-1];
				p=1;
			}
			i++;
		}

		 customers_hash = (struct Customer**) malloc (max_customers_g*(sizeof(struct Customer*)));
		for(i=0;i<max_customers_g;i++)
		{
			customers_hash[i]=NULL;
		}

		for(i=0;i<prime;i++){
		if(customers_hash[i] == NULL)
		{
			printf("einai null sto initialize sto i =%d\n",i);
		}
	}



	return 1;
}

/**
 * @brief Register shop
 *
 * @param sid   The shop's id
 * @param type  The shop's type
 *
 * @return 1 on success
 *         0 on failure
 */
int RegisterShop (int sid, int type)
{
	if(shopRoot==NULL)
	{
		shopRoot=insertShop(shopRoot,sid,type);
	}else
	{
		insertShop(shopRoot,sid,type);
	}

	printf("R <%d>\n Shops=",sid);
	inorder1(shopRoot);
	printf("DONE\n");
	return 1;


	return 1;
}

/**
 * @brief Supply shop
 *
 * @param sid       The shop's id
 * @param pid       The product's id
 * @param quantity  The product's quantity
 * @param price     The product's price
 *
 * @return 1 on success
 *         0 on failure
 */


 void insertP(struct Shop* node,int sid,int pid,int quantity,int price)
{
	if(node==NULL)
	{
		return ;
	}

	 if(sid<node->sid)
	 {
		 insertP(node->lc,sid,pid,quantity,price);
	 }
	 else if(sid>node->sid)
	 {
		 insertP(node->rc,sid,pid,quantity,price);
	 }
	 else if(sid==node->sid)
	 {

			struct Product *p=node->products;

			while(p->next!=NULL&&p->next->pid<pid)
			{
				p=p->next;

			}

			struct Product* temp=(struct Product*)malloc(sizeof(struct Product));
		 temp->pid=pid;
		 temp->qty=quantity;
		 temp->price=price;
		 printf("lol\n");
		 temp->next=p->next;
		 p->next=temp;

	 }

	 return ;

}

int SupplyShop (int sid, int pid, int quantity, int price)
{

		insertP(shopRoot,sid,pid,quantity,price);

		printf("S <%d><%d><%d><%d>\n",sid,pid,quantity,price);
		inorder2(shopRoot);
		printf("DONE\n");
	return 1;
}

/**
 * @brief Register customer
 *
 * @param cid The customer's id
 *
 * @return 1 on success
 *         0 on failure
 */
int RegisterCustomer (int cid)
{

int pos=hash_function(cid);


	struct Customer* new=(struct Customer*)malloc(sizeof(struct Customer));
	new->cid=cid;
 	new->shopping_size=0;
	new->shopping_tree=NULL;

	customers_hash[pos]=new;

printf("pos in register=%d\n",pos);
	return 1;
}

/**
 * @brief Add to shopping list
 *
 * @param cid       The customer's id
 * @param pid       The product's id
 * @param quantity  The quantity of the product
 *
 * @return 1 on success
 *         0 on failure
 */
int AddToShoppingList (int cid, int pid, int quantity)
{

	int pos=has_find(cid);

struct Customer *temp= customers_hash[pos];
struct C_Product *head=temp->shopping_tree;
if(head==NULL)
{
	temp->shopping_tree=insertPro(temp->shopping_tree,pid,quantity);
	temp->shopping_size++;
}else
{
	insertPro(temp->shopping_tree,pid,quantity);
	temp->shopping_size++;
}

	PrintCustomers();

	return 1;
}






struct C_Product* deletePro(struct C_Product* head, int id)
{
    if (head == NULL) return head;

    if (id < head->pid)
		{
        head->lc = deletePro(head->lc,id);
 		}
    else if (id > head->pid)
		{
        head->rc = deletePro(head->rc,id);
		}
    else
    {
        if (head->lc == NULL)
        {
            struct C_Product *temp = head->rc;
            free(head);
            return temp;
        }
        else if (head->rc == NULL)
        {
            struct C_Product *temp = head->lc;
            free(head);
            return temp;
        }
struct C_Product*temp=head;
  		temp=temp->rc;
			while(temp && temp->lc!=NULL)
			{
				temp=temp->lc;
			}

        head->pid = temp->pid;
				head->qty=temp->qty;

        head->rc= deletePro(head->rc, temp->pid);
    }
		PrintCustomers();
    return head;
}



/**
 * @brief Delete from shopping list
 *
 * @param cid  The customer's id
 * @param pid  The product's id
 *
 * @return 1 on success
 *         0 on failure
 */
int DeleteFromShoppingList (int cid, int pid)
{
int pos=has_find(cid);
struct Customer *temp= customers_hash[pos];
struct C_Product *head=temp->shopping_tree;

if(head!=NULL)
{

	deletePro(temp->shopping_tree,pid);
	temp->shopping_size--;
}

	return 1;
}




void inorder_put_in_array(struct C_Product *head)
{
	if(head!=NULL)
	{
		inorder_put_in_array(head->lc);
		arrs[help]=head;
		help++;
		inorder_put_in_array(head->rc);
	}

}

void inorder_put_in_first(struct C_Product *head)
{
	if(head!=NULL)
	{
		inorder_put_in_array(head->lc);
		m1[help]=head;
		help++;
		inorder_put_in_array(head->rc);
	}

}


void inorder_put_in_sec(struct C_Product *head)
{
	if(head!=NULL)
	{
		inorder_put_in_array(head->lc);
		m2[help]=head;
		help++;
		inorder_put_in_array(head->rc);
	}

}



void shopping_therapy(struct Shop *head,int size)
{
	if(head!=NULL)
	{
		printf("lol\n");
		shopping_therapy(head->lc,size);
		int i;
		struct Product *temp=head->products;
		while(temp->next!=NULL)
		{
			temp=temp->next;
			for(i=0;i<size;i++)
			{
				if(temp->pid==arrs[i]->pid)
				{
				 if(temp->qty==arrs[i]->qty)
				{

					temp->qty=temp->qty-arrs[i]->qty;
					arrs[i]->qty=0;
				}
				else if(temp->qty<arrs[i]->qty)
				{
					temp->qty=0;
					arrs[i]->qty=arrs[i]->qty-temp->qty;
				}
				else
				{
					arrs[i]->qty=0;
					temp->qty=0;

				}
			}
			}
		}
		shopping_therapy(head->rc,size);
	}
}
/**
 * @brief Go shopping
 *
 * @param cid The customer's id
 *
 * @return 1 on success
 *         0 on failure
 */
int GoShopping (int cid)
{
	int pos=has_find(cid);


	arrs=malloc(customers_hash[pos]->shopping_size * sizeof(struct C_Product));
	help=0;
	inorder_put_in_array(customers_hash[pos]->shopping_tree);

	shopping_therapy(shopRoot,customers_hash[pos]->shopping_size);

	return 1;
}



struct C_Product * makemerged(int start, int end)
{
    /* Base Case */
    if (start > end)
    return NULL;


    int mid = (start + end)/2;
    struct C_Product *new;
		struct C_Product *node =malloc(sizeof(struct C_Product));
		node->pid=merged[mid]->pid;
		node->qty=merged[mid]->qty;
		node->lc=NULL;
		node->rc=NULL;
		new=node;
    new->lc = makemerged(start, mid - 1);

    new->rc = makemerged(mid + 1, end);

    return new;
}
/**
 * @brief Customers shop together
 *
 * @param cid1  The id of the first customer
 * @param cid2  The id of the second customer
 *
 * @return 1 on success
 *         0 on failure
 */
int ShopTogether (int cid1, int cid2)
{
	int pos1=has_find(cid1);
	int pos2=has_find(cid2);
	int i, j, k;
	int size1=customers_hash[pos1]->shopping_size;
	int size2=customers_hash[pos2]->shopping_size;
	int sizemerged=customers_hash[pos1]->shopping_size + customers_hash[pos2]->shopping_size;

	m1=malloc(size1 * sizeof(struct C_Product));
	m2=malloc(customers_hash[pos2]->shopping_size * sizeof(struct C_Product));
	merged=malloc(sizemerged*sizeof (struct C_Product));
	help=0;
	inorder_put_in_first(customers_hash[pos1]->shopping_tree);
	help=0;
	inorder_put_in_sec(customers_hash[pos2]->shopping_tree);

	  j =0;
	 	k = 0;

	  for (i = 0; i < sizemerged;) {
	    if (j < size1 && k < size2) {
	      if (m1[j] < m2[k]) {
	        merged[i] = m1[j];
	        j++;
	      }
	      else {
	        merged[i] = m2[k];
	        k++;
	      }
	      i++;
	    }
	    else if (j == size1) {
	      for (; i < sizemerged;) {
	        merged[i] = m2[k];
	        k++;
	        i++;
	      }
	    }
	    else {
	      for (; i < sizemerged;) {
	        merged[i] = m1[j];
	        j++;
	        i++;
	      }
	    }
	  }



	return 1;
}

 void kids(struct Shop *p, int lol,struct Shop *child){

    if (lol==1) {
        p->lc = child;
    } else {
        p->rc = child;
    }
}



struct Shop *Split_Tree(struct Shop *head, int type)
{
    struct Shop *new_root = NULL, *parent1 = NULL,*parent2 = NULL;
		int ok = (head != NULL && type < head->type);

    while(head != NULL)
		{
        while (head != NULL && (type < head->type) == ok)
				{
            parent1 = head;
            if (ok)
						{
                head = head->lc;
            }
            else
						{
                head = head->rc;
            }
        }

        kids(parent1, ok, NULL);

        if (ok == 1)
				{
            ok = 0;
        }
        else
				{
            ok = 1;
        }
        if(new_root == NULL)
				{
            new_root = head;
        }
        else if (parent2 != NULL)
				{
            kids(parent2, ok, head);
        }
        parent2 = parent1;
        parent1 = NULL;
    }
    return new_root;
}




/**
 * @brief Categorize shops
 *
 * @return 1 on success
 *         0 on failure
 */
int CategorizeShops (void)
{
	int i;
	for(i=0;i<5;i++)
	{
		ShopTypes[i]=malloc(sizeof(struct Shop));
	}

	for(i=0;i<4;i++)
	{
		ShopTypes[i]=Split_Tree(shopRoot,i+1);
	}
	ShopTypes[4]=shopRoot;

for(i=0;i<5;i++)
{
	inorder2(ShopTypes[i]);
	printf("\n");
}


return 1;
}


/**
 * @brief Print shops
 *
 * @return 1 on success
 *         0 on failure
 */
int PrintShops (void)
{
	return 1;
}


/**
 * @brief Free resources
 *
 * @return 1 on success
 *         0 on failure
 */
int FreeAll(void)
{
	return 1;
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
int main(int argc, char** argv)
{
	FILE *fin = NULL;
	char buff[BUFFER_SIZE], event;

	/* Check command buff arguments */
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <input_file> \n", argv[0]);
		return EXIT_FAILURE;
	}

	/* Open input file */
	if (( fin = fopen(argv[1], "r") ) == NULL ) {
		fprintf(stderr, "\n Could not open file: %s\n", argv[1]);
		perror("Opening test file\n");
		return EXIT_FAILURE;
	}

	// /* Initializations */
	// Initialize();

	/* Read input file buff-by-buff and handle the events */
	while (fgets(buff, BUFFER_SIZE, fin)) {

		DPRINT("\n>>> Event: %s", buff);

		switch(buff[0]) {

			/* Comment */
			case '#':
				break;

			/* dummy event '0'. Get the total number of customers */
			case '0':
				{
					sscanf(buff, "%c %u", &event, &max_customers_g);
					Initialize();
					//hashtablesize=max_customers_g/0.75;
					DPRINT("%c MAX CUSTOMERS: %u\n", event, max_customers_g);
					break;
				}

				/* Register shop
				 * R <sid> <type> */
			case 'R':
				{
					int sid, type;

					sscanf(buff, "%c %d %d", &event, &sid, &type);
					DPRINT("%c %d %d\n", event, sid, type);

					if (RegisterShop(sid, type)) {
						DPRINT("%c %d %d succeeded\n", event, sid, type);
					} else {
						fprintf(stderr, "%c %d %d failed\n", event, sid, type);
					}

					break;
				}

				/* Supply shop
				 * S <sid> <pid> <quantity> <price> */
			case 'S':
				{
					int sid, pid, quantity, price;

					sscanf(buff, "%c %d %d %d %d", &event, &sid, &pid, &quantity, &price);
					DPRINT("%c %d %d %d %d\n", event, sid, pid, quantity, price);

					if (SupplyShop(sid, pid, quantity, price)) {
						DPRINT("%c %d %d %d %d succeeded\n", event, sid, pid, quantity, price);
					} else {
						fprintf(stderr, "%c %d %d %d %d failed\n", event, sid, pid, quantity, price);
					}

					break;
				}

				/* Register customer
				 * C <cid> */
			case 'C':
				{
					int cid;

					sscanf(buff, "%c %d", &event, &cid);
					DPRINT("%c %d\n", event, cid);

					if (RegisterCustomer(cid)) {
						DPRINT("%c %d succeeded\n", event, cid);
					} else {
						fprintf(stderr, "%c %d failed\n", event, cid);
					}

					break;
				}

				/* Add to shopping list
				 * L <cid> <pid> <quantity> */
			case 'L':
				{
					int cid, pid, quantity;

					sscanf(buff, "%c %d %d %d", &event, &cid, &pid, &quantity);
					DPRINT("%c %d %d %d\n", event, cid, pid, quantity);

					if (AddToShoppingList(cid, pid, quantity)) {
						DPRINT("%c %d %d %d succeeded\n", event, cid, pid, quantity);
					} else {
						fprintf(stderr, "%c %d %d %d failed\n", event, cid, pid, quantity);
					}

					break;
				}

				/* Delete from shopping list
				 * D <cid> <pid> */
			case 'D':
				{
					int cid, pid;

					sscanf(buff, "%c %d %d", &event, &cid, &pid);
					DPRINT("%c %d %d\n", event, cid, pid);

					if (DeleteFromShoppingList(cid, pid)) {
						DPRINT("%c %d %d succeeded\n", event, cid, pid);
					} else {
						fprintf(stderr, "%c %d %d failed\n", event, cid, pid);
					}

					break;
				}

				/* Go shopping
				 * G <cid> */
			case 'G':
				{
					int cid;

					sscanf(buff, "%c %d", &event, &cid);
					DPRINT("%c %d\n", event, cid);

					if (GoShopping(cid)) {
						DPRINT("%c %d succeeded\n", event, cid);
					} else {
						fprintf(stderr, "%c %d failed\n", event, cid);
					}

					break;
				}

				/* Shop together
				 * M <cid1> <cid2> */
			case 'M':
				{
					int cid1, cid2;

					sscanf(buff, "%c %d %d", &event, &cid1, &cid2);
					DPRINT("%c %d %d\n", event, cid1, cid2);

					if (ShopTogether(cid1, cid2)) {
						DPRINT("%c %d %d succeeded\n", event, cid1, cid2);
					} else {
						fprintf(stderr, "%c %d %d failed\n", event, cid1, cid2);
					}

					break;
				}

				/* Categorize shops
				 * T */
			case 'T':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (CategorizeShops()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}

				/* Print shops
				 * X */
			case 'X':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (PrintShops()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}

				/* Print customers
				 * Y */
			case 'Y':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (PrintCustomers()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}

				/* Empty line */
			case '\n':
				break;

				/* Ignore everything else */
			default:
				DPRINT("Ignoring buff: %s \n", buff);

				break;
		}
	}

	FreeAll();
	return (EXIT_SUCCESS);
}
