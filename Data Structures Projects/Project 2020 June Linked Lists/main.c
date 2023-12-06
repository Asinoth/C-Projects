

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Gaugamela.h"

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
 * @brief Optional function to initialize data structures that
 *        need initialization
 *
 * @return 1 on success
 *         0 on failure
 */
int initialize (void)
{

	GA_army[0]=(struct Army*)malloc(sizeof(struct Army));
	GA_army[0]->next=NULL;

	GA_army[1]=(struct Army*)malloc(sizeof(struct Army));
	GA_army[1]->next=NULL;


	GA_battle_list=(struct GA_Battle*)malloc(sizeof(struct GA_Battle));
	GA_battle_list->next=NULL;

	satrapies_list=(struct Satrapy*)malloc(sizeof(struct Satrapy));
	satrapies_list->s_army=(struct Army*)malloc(sizeof(struct Army));
	satrapies_list->sentinel=(struct Army*)malloc(sizeof(struct Army));
	satrapies_list->next=satrapies_list;
	satrapies_list->prev=satrapies_list;
	satrapies_list->s_army->next=NULL;
	satrapies_list->sentinel->next=NULL;

	S_battle_list=(struct S_Battle*)malloc(sizeof(struct S_Battle));
	S_battle_list->next=NULL;

	GA_Generals[0]=(struct GA_Battle*)malloc(sizeof(struct GA_Battle));
	GA_Generals[0]->next=NULL;
	GA_Generals[1]=(struct GA_Battle*)malloc(sizeof(struct GA_Battle));
	GA_Generals[1]->next=NULL;
	GA_Generals[2]=(struct GA_Battle*)malloc(sizeof(struct GA_Battle));
	GA_Generals[2]->next=NULL;
	GA_Generals[3]=(struct GA_Battle*)malloc(sizeof(struct GA_Battle));
	GA_Generals[3]->next=NULL;
	GA_Generals[4]=(struct GA_Battle*)malloc(sizeof(struct GA_Battle));
	GA_Generals[4]->next=NULL;



	return 1;
}

/**
 * @brief Register Alexander the Great soldier
 *
 * @param sid     The soldier's id
 * @param type    The soldier's type
 * @param general The general the soldier obeys
 *
 * @return 1 on success
 *         0 on failure
 */
int register_GA_soldier (int sid, int type, int general)
{

	struct Army* temp= GA_army[0];
	int found=0;

	while(temp->next!=NULL && found==0)
	{
		if(temp->next->id > sid)
		{
			found=1;
			break;
		}
		temp=temp->next;
	}
	if(found==1){
	struct Army *temp2=(struct Army*)malloc(sizeof(struct Army));
	temp2->next=temp->next;
	temp->next=temp2;
	temp2->id=sid;
	temp2->type=type;
	temp2->general=general;
}else{
	struct Army *temp2=(struct Army*)malloc(sizeof(struct Army));
	temp2->next=NULL;
	temp->next=temp2;
	temp2->id=sid;
	temp2->type=type;
	temp2->general=general;
}

struct Army* temp3=GA_army[0];

printf(" R <sid> <type> <general> \n  GA Soldiers=");
while(temp3->next!=NULL)
{
	temp3=temp3->next;
	printf("<%d,%d,%d>,",temp3->id,temp3->type,temp3->general);


}
printf("\nDONE");




	return 1;
}

/**
 * @brief Register Alexander the Great horse
 *
 * @param sid     The horse's id
 * @param type    Always the value of 0
 * @param general The general the soldier obeys
 *
 * @return 1 on success
 *         0 on failure
 */
int register_GA_horse (int hid, int type, int general)
{

	struct Army* temp= GA_army[1];
	int found=0;

	while(temp->next!=NULL && found==0)
	{
		if(temp->next->id > hid)
		{
			found=1;
			break;
		}
		temp=temp->next;
	}
	if(found==1){
	struct Army *temp2=(struct Army*)malloc(sizeof(struct Army));
	temp2->next=temp->next;
	temp->next=temp2;
	temp2->id=hid;
	temp2->type=type;
	temp2->general=general;
}else{
	struct Army *temp2=(struct Army*)malloc(sizeof(struct Army));
	temp2->next=NULL;
	temp->next=temp2;
	temp2->id=hid;
	temp2->type=type;
	temp2->general=general;
}

struct Army* temp3=GA_army[1];

printf(" H <hid> <type> <general> \n  GA Horses=");
while(temp3->next!=NULL)
{
	temp3=temp3->next;
	printf("<%d,%d,%d>,",temp3->id,temp3->type,temp3->general);


}
printf("\nDONE");


	return 1;
}


/**
 * @brief Register Satrapy
 *
 * @param sid The Satrapy's id
 *
 * @return 1 on success
 *         0 on failure
 */
int register_satrapy (int sid)
{

	struct Satrapy* head=satrapies_list;
	struct Satrapy* temphead=satrapies_list;
	while(temphead->next!=head)
	{
		temphead=temphead->next;
	}

	struct Satrapy*temp= (struct Satrapy*)malloc(sizeof(struct Satrapy));
	struct Army *tmparmy=(struct Army*)malloc(sizeof(struct Army));
	struct Army *tmpsent=(struct Army*)malloc(sizeof(struct Army));
	temphead->next=temp;
	temp->next=head;
	temp->prev=temphead;
	head->prev=temp;
	temp->id=sid;
	temp->s_army=tmparmy;
	temp->sentinel=tmpsent;

struct Satrapy*temp2=satrapies_list;

printf(" S <sid> \n  Satrapies=");
while(temp2->next!=head)
{
	temp2=temp2->next;
	printf("<%d>,",temp2->id);


}
printf("\nDONE\n");



	return 1;
}

/**
 * @brief Register Satrapy soldier
 *
 * @param sid     The id of the Satrapy the soldier belongs to
 * @param aid     The soldier's id
 * @param type    The type of soldier
 * @param general The general the soldier obeys
 *
 * @return 1 on success
 *         0 on failure
 */
int register_S_soldier (int sid, int aid, int type, int general)
{

struct Satrapy* head=satrapies_list;
struct Satrapy* temphead=satrapies_list;
int found=0;

while(temphead->next!=head && found==0)
{
	temphead=temphead->next;
	if(temphead->id==sid)
	{
		found=1;
		break;
	}

}


struct Army* Atmp1=temphead->s_army;

while(Atmp1->next!=NULL)
{
	Atmp1=Atmp1->next;
}


struct Army *newAtmp=(struct Army*)malloc(sizeof(struct Army));
Atmp1->next=newAtmp;
newAtmp->id=aid;
newAtmp->type=type;
newAtmp->general=general;
newAtmp->next=NULL;
temphead->sentinel=newAtmp;

struct Satrapy* temp2=satrapies_list;
struct Army* Atmp=temp2->s_army;

printf("A <sid> <aid> <type> <general> \n ");
	while(temp2->next!=head)
	{
		temp2=temp2->next;
		printf("\nSatrapy %d =",temp2->id);
		Atmp=temp2->s_army;

		while(Atmp->next!=NULL){
			Atmp=Atmp->next;
			printf("<%d,%d,%d,>,",Atmp->id,Atmp->type,Atmp->general);

	}

	}
printf("\nDONE\n");


	if(found==1)return 1;
	else return 0;
}

/**
 * @brief Prepare Alexander's soldiers for battle
 *
 * @return 1 on success
 *         0 on failure
 */
int prepare_for_battle_GA (void)
{

	struct GA_Battle *newGA =GA_battle_list;

for(int i=1;i<=4;i++)
{
	struct Army *tmpS=GA_army[0];
	struct Army *tmpH=GA_army[1];

		while(tmpS->next!=NULL)
		{
			tmpS=tmpS->next;
			if(tmpS->type==i)
			{
				newGA->next=(struct GA_Battle*)malloc(sizeof(struct GA_Battle));

				if(tmpS->type==1)
				{

					tmpH=tmpH->next;
					newGA->next->soldier_id=tmpS->id;
					newGA->next->horse_id=tmpH->id;
					newGA->next->general=tmpS->general;
					newGA->next->next=NULL;
				}
				else{

					newGA->next->soldier_id=tmpS->id;
					newGA->next->horse_id=-1;
					newGA->next->general=tmpS->general;
					newGA->next->next=NULL;
				}
				newGA=newGA->next;
		}


	}


}

struct GA_Battle *tmp=GA_battle_list;

printf("P\n  GA Battle=");

while(tmp->next!=NULL)
{
	tmp=tmp->next;
	printf("<%d,%d,%d>,",tmp->soldier_id,tmp->horse_id,tmp->general);
}
printf("\nDONE\n");


	return 1;
}

/**
 * @brief Prepare Satrapies for battle
 *
 * @param sid The satrapy's id
 *
 * @return 1 on success
 *         0 on failure
 */
int prepare_for_battle_S (int sid)
{


	struct S_Battle *newSB=S_battle_list;
	struct Satrapy *left=satrapies_list;
	struct Satrapy *right=satrapies_list;
	struct Satrapy *findSid=satrapies_list;
	struct Army *tmp1;
	struct Army *tmp2;




	while (findSid->next!=NULL)
	{
		findSid=findSid->next;

		if(findSid->id==sid)
		{
			left=findSid;
			right=findSid;

			tmp1=left->s_army;
		while(tmp1->next!=NULL)
		{
			tmp1=tmp1->next;
			newSB->next = (struct S_Battle*)malloc(sizeof(struct S_Battle));
			newSB->next->soldier_id=tmp1->id;
			newSB->next->satrapy_id=left->id;
			newSB->next->type=tmp1->type;
			newSB = newSB->next;
			newSB->next=NULL;
		}


			break;
		}
	}


while(right->next!=left->prev)
{


	left=left->prev;


	tmp1=left->s_army;
	while(tmp1->next!=NULL)
	{
		tmp1=tmp1->next;
		newSB->next=(struct S_Battle*)malloc(sizeof(struct S_Battle));

		newSB->next->soldier_id=tmp1->id;
		newSB->next->satrapy_id=left->id;
		newSB->next->type=tmp1->type;
		newSB=newSB->next;
		newSB->next=NULL;
	}

if(right->next!=left->prev){
	right=right->next;


	tmp2=right->s_army;

	while(tmp2->next!=NULL)
	{
		tmp2=tmp2->next;
		newSB->next=(struct S_Battle*)malloc(sizeof(struct S_Battle));

		newSB->next->soldier_id=tmp2->id;
		newSB->next->satrapy_id=right->id;
		newSB->next->type=tmp2->type;
		newSB=newSB->next;
		newSB->next=NULL;
	}
}


}


struct S_Battle *tmp=S_battle_list;

printf("B<sid>\n  Satrapy Battle=");

while(tmp->next!=NULL)
{
	tmp=tmp->next;
	printf("<%d,%d,%d>,",tmp->soldier_id,tmp->satrapy_id,tmp->type);
}
printf("\nDONE\n");

	return 1;
}

/**
 * @brief The death of a soldier
 *
 * @param sid  The soldier's id
 * @param flag If 0, the soldier belongs to a Satrapy
 *             If 1, the soldier belongs to Alexander the Great
 *
 * @return 1 on success
 *         0 on failure
 */
int soldiers_death (int sid, int flag)
{

	struct S_Battle *SBc=S_battle_list;
	struct S_Battle *SBp=S_battle_list;
	struct GA_Battle *GAc=GA_battle_list;
	struct GA_Battle *GAp=GA_battle_list;
	struct Satrapy *S=satrapies_list;
	struct Army *tempc= GA_army[0];
	struct Army *tempn= GA_army[0];

	int found=0;
	int satr;

	if(flag==0)
	{
		while(SBc->next!=NULL&& found==0)
		{
			SBc=SBc->next;
			if(SBc->soldier_id==sid)
				{

							found=1;
							SBc=SBp->next;
							SBp->next=SBc->next;
							satr=SBp->satrapy_id;
							free(SBc);


				}
				else
				{
					SBp=SBc;

				}
		}

		found =0;
 //diagrafi apo satrapia

	while(S->next!=satrapies_list && found==0)
	{
		S=S->next;
		if(S->next->id==satr)
		{

			struct Army *tmp1;
			struct Army *tmp2;
			tmp1=S->s_army;
			while(tmp1->next!=NULL && found==0)
				{
					tmp1=tmp1->next;
					if(tmp1->id==sid)
						{

									found=1;
									tmp1=tmp2->next;
									tmp2->next=tmp1->next;
									free(tmp1);


						}
						else
						{
							tmp2=tmp1;

						}
				}
				break;
		}
	}
}

	if(flag==1)
	{
		while(GAc->next!=NULL&& found==0)
		{
			GAc=GAc->next;



			if(GAc->soldier_id==sid)
				{

							found=1;
							GAc=GAp->next;
							GAp->next=GAc->next;
							free(GAc);
				}

				else
				{
					GAp=GAc;

				}

		}

		found = 0;
		while(tempc->next!=NULL&& found==0)
		{
			tempc=tempc->next;
			if(tempc->id==sid)
				{

							found=1;
							tempc=tempn->next;
							tempn->next=tempc->next;
							free(tempc);


				}else

				{
					tempn=tempc;

				}



	}
}

struct GA_Battle *tmp=GA_battle_list;

printf("K <sid> <flag>\n  GA Battle=");

while(tmp->next!=NULL)
{
	tmp=tmp->next;
	printf("<%d,%d,%d>,",tmp->soldier_id,tmp->horse_id,tmp->general);
}

struct S_Battle *tmp1=S_battle_list;

printf("\n  Satrapy Battle=");

while(tmp1->next!=NULL)
{
	tmp1=tmp1->next;
	printf("<%d,%d,%d>,",tmp1->soldier_id,tmp1->satrapy_id,tmp1->type);
}
printf("\nDONE\n");





return 1;
}
/**
 * @brief Victory of Alexander the Great's horsemen
 *
 * @return 1 on success
 *         0 on failure
 */
int GA_horsemen_victory (void)
{
	struct S_Battle *SBc=S_battle_list;
	struct S_Battle *SBp=S_battle_list;
	struct GA_Battle *GAc=GA_battle_list;
	struct GA_Battle *GAp=GA_battle_list;

	int i=0;
	while(GAc->next!=NULL)
	{
		GAc=GAc->next;

		if(GAc->horse_id!=-1)
			{
				i++;
				if(i==3)
						{
							GAc=GAp->next;
							GAp->next=GAc->next;
							free(GAc);
							i=0;
						}else
						{
							GAp=GAc;
						}
		}
			else
			{
				GAp=GAc;

			}

	}


	while(SBc->next!=NULL)
	{
		SBc=SBc->next;
		if(SBc->type==1)
			{


						SBc=SBp->next;
						SBp->next=SBc->next;
						free(SBc);


			}
			else
			{
				SBp=SBc;

			}
	}

	struct GA_Battle *tmp=GA_battle_list;

	printf("V \n  GA Battle=");

	while(tmp->next!=NULL)
	{
		tmp=tmp->next;
		printf("<%d,%d,%d>,",tmp->soldier_id,tmp->horse_id,tmp->general);
	}

	struct S_Battle *tmp1=S_battle_list;

	printf("\n  Satrapy Battle=");

	while(tmp1->next!=NULL)
	{
		tmp1=tmp1->next;
		printf("<%d,%d,%d>,",tmp1->soldier_id,tmp1->satrapy_id,tmp1->type);
	}
	printf("\nDONE\n");

	return 1;
}

/**
 * @brief The death of Alexander the Great
 *
 * @return 1 on success
 *         0 on failure
 */
int Alexanders_death (void)
{

struct GA_Battle *G1=GA_Generals[0];
struct GA_Battle *G2=GA_Generals[1];
struct GA_Battle *G3=GA_Generals[2];
struct GA_Battle *G4=GA_Generals[3];
struct GA_Battle *G5=GA_Generals[4];


struct GA_Battle *lista=GA_battle_list;

while(lista->next!=NULL)
{
	lista=lista->next;


	if(lista->general==0)
	{
		struct GA_Battle *end=G1;

		while(end->next!=NULL)
		{
			end=end->next;
		}
		struct GA_Battle *temp1=(struct GA_Battle*)malloc(sizeof(struct GA_Battle));
		temp1->next=end->next;
		end->next=temp1;
		temp1->soldier_id=lista->soldier_id;
		temp1->horse_id=lista->horse_id;
		temp1->general=lista->general;
	}

	if(lista->general==1)
	{
		struct GA_Battle *end=G2;

		while(end->next!=NULL)
		{
			end=end->next;
		}
		struct GA_Battle *temp2=(struct GA_Battle*)malloc(sizeof(struct GA_Battle));
		temp2->next=end->next;
		end->next=temp2;
		temp2->soldier_id=lista->soldier_id;
		temp2->horse_id=lista->horse_id;
		temp2->general=lista->general;
	}

	if(lista->general==2)
	{
		struct GA_Battle *end=G3;

		while(end->next!=NULL)
		{
			end=end->next;
		}
		struct GA_Battle *temp3=(struct GA_Battle*)malloc(sizeof(struct GA_Battle));
		temp3->next=end->next;
		end->next=temp3;
		temp3->soldier_id=lista->soldier_id;
		temp3->horse_id=lista->horse_id;
		temp3->general=lista->general;
	}

	if(lista->general==3)
	{
		struct GA_Battle *end=G4;

		while(end->next!=NULL)
		{
			end=end->next;
		}
		struct GA_Battle *temp4=(struct GA_Battle*)malloc(sizeof(struct GA_Battle));
		temp4->next=end->next;
		end->next=temp4;
		temp4->soldier_id=lista->soldier_id;
		temp4->horse_id=lista->horse_id;
		temp4->general=lista->general;

}


if(lista->general==4)
{
	struct GA_Battle *end=G5;

	while(end->next!=NULL)
	{
		end=end->next;
	}
	struct GA_Battle *temp5=(struct GA_Battle*)malloc(sizeof(struct GA_Battle));
	temp5->next=end->next;
	end->next=temp5;
	temp5->soldier_id=lista->soldier_id;
	temp5->horse_id=lista->horse_id;
	temp5->general=lista->general;
}

}

struct GA_Battle* temp1=GA_Generals[0];

printf("D\n General1=");

while(temp1->next!=NULL)
{
	temp1=temp1->next;
	printf("<%d,%d,%d>",temp1->soldier_id,temp1->horse_id,temp1->general);
}

struct GA_Battle* temp2=GA_Generals[1];
printf("\n General2=");
while(temp2->next!=NULL)
{
	temp2=temp2->next;
	printf("<%d,%d,%d>",temp2->soldier_id,temp2->horse_id,temp2->general);
}


struct GA_Battle* temp3=GA_Generals[2];
printf("\n General3=");
while(temp3->next!=NULL)
{
	temp3=temp3->next;
	printf("<%d,%d,%d>",temp3->soldier_id,temp3->horse_id,temp3->general);
}

struct GA_Battle* temp4=GA_Generals[3];
printf("\n General4=");
while(temp4->next!=NULL)
{
	temp4=temp4->next;
	printf("<%d,%d,%d>",temp4->soldier_id,temp4->horse_id,temp4->general);
}

struct GA_Battle* temp5=GA_Generals[4];
printf("\n General5=");
while(temp5->next!=NULL)
{
	temp5=temp5->next;
	printf("<%d,%d,%d>",temp5->soldier_id,temp5->horse_id,temp5->general);
}

printf("\nDONE\n");

	return 1;
}

/**
 * @brief Print all soldiers of Alexander the Great
 *
 * @return 1 on success
 *         0 on failure
 */
int print_GA_soldiers (void)
{
	struct Army* temp3=GA_army[0];

	printf("X \n  GA Soldiers=");
	while(temp3->next!=NULL)
	{
		temp3=temp3->next;
		printf("<%d,%d,%d>,",temp3->id,temp3->type,temp3->general);

	}
		struct Army* temp4=GA_army[1];

		printf("\n  GA Horses=");

		while(temp4->next!=NULL)
		{
			temp4=temp4->next;
			printf("<%d,%d,%d>,",temp4->id,temp4->type,temp4->general);


		}
		printf("\nDONE\n");




	return 1;
}

/**
 * @brief Print all Satrapy soldiers
 *
 * @return 1 on success
 *         0 on failure
 */
int print_S_soldiers (void)
{

	struct Satrapy* temp2=satrapies_list;
	struct Army* Atmp=temp2->s_army;

	printf("Y \n ");
		while(temp2->next!=satrapies_list)
		{
			temp2=temp2->next;
			printf("\nSatrapy=");
			Atmp=temp2->s_army;

			while(Atmp->next!=NULL){
				Atmp=Atmp->next;
				printf("<%d,%d,%d>,",Atmp->id,Atmp->type,Atmp->general);

		}


		}
	printf("\nDONE\n");
	return 1;
}

/**
 * @brief Free resources
 *
 * @return 1 on success
 *         0 on failure
 */
int free_all(void)
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

	/* Initializations */
	initialize();

	/* Read input file buff-by-buff and handle the events */
	while (fgets(buff, BUFFER_SIZE, fin)) {

		DPRINT("\n>>> Event: %s", buff);

		switch(buff[0]) {

			/* Comment */
			case '#':
				break;

				/* Register GA soldier
				 * R <sid> <type> <general> */
			case 'R':
				{
					int sid, type, general;

					sscanf(buff, "%c %d %d %d", &event, &sid, &type, &general);
					DPRINT("%c %d %d %d\n", event, sid, type, general);

					if (register_GA_soldier(sid, type, general)) {
						DPRINT("R %d %d %d succeeded\n", sid, type, general);
					} else {
						fprintf(stderr, "R %d %d %d failed\n", sid, type, general);
					}

					break;
				}

				/* Register GA horse
				 * H <hid> <type> <general> */
			case 'H':
				{
					int hid, type, general;

					sscanf(buff, "%c %d %d %d", &event, &hid, &type, &general);
					DPRINT("%c %d %d %d\n", event, hid, type, general);

					if (register_GA_horse(hid, type, general)) {
						DPRINT("H %d %d %d succeeded\n", hid, type, general);
					} else {
						fprintf(stderr, "H %d %d %d failed\n", hid, type, general);
					}

					break;
				}

				/* Register Satrapy
				 * S <sid> */
			case 'S':
				{
					int sid;

					sscanf(buff, "%c %d", &event, &sid);
					DPRINT("%c %d\n", event, sid);

					if (register_satrapy(sid)) {
						DPRINT("S %d succeeded\n", sid);
					} else {
						fprintf(stderr, "S %d failed\n", sid);
					}

					break;
				}

				/* Register Satrapy soldier
				 * A <sid> <aid> <type> <general> */
			case 'A':
				{
					int sid, aid, type, general;

					sscanf(buff, "%c %d %d %d %d", &event, &sid, &aid, &type, &general);
					DPRINT("%c %d %d %d %d\n", event, sid, aid, type, general);

					if (register_S_soldier(sid, aid, type, general)) {
						DPRINT("A %c %d %d %d %d succeeded\n", event, sid, aid, type, general);
					} else {
						fprintf(stderr, "A %c %d %d %d %d failed\n", event, sid, aid, type, general);
					}

					break;
				}

				/* Prepare Alexander's soldiers for battle
				 * P */
			case 'P':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (prepare_for_battle_GA()) {
						DPRINT("P %c succeeded\n", event);
					} else {
						fprintf(stderr, "P %c failed\n", event);
					}

					break;
				}

				/* Prepare Satrapies for battle
				 * B <sid> */
			case 'B':
				{
					int sid;

					sscanf(buff, "%c %d", &event, &sid);
					DPRINT("%c %d\n", event, sid);

					if (prepare_for_battle_S(sid)) {
						DPRINT("B %c %d succeeded\n", event, sid);
					} else {
						fprintf(stderr, "B %c %d failed\n", event, sid);
					}

					break;
				}

				/* The death of a soldier
				 * K <sid> <flag> */
			case 'K':
				{
					int sid, flag;

					sscanf(buff, "%c %d %d", &event, &sid, &flag);
					DPRINT("%c %d %d\n", event, sid, flag);

					if (soldiers_death(sid, flag)) {
						DPRINT("K %c %d %d succeeded\n", event, sid, flag);
					} else {
						fprintf(stderr, "K %c %d %d failed\n", event, sid, flag);
					}

					break;
				}

				/* Victory of Alexander the Great's horsemen
				 * V */
			case 'V':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (GA_horsemen_victory()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}

				/* The death of Alexander the Great
				 * D */
			case 'D':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (Alexanders_death()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}

				/* Print all soldiers of Alexander the Great
				 * X */
			case 'X':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (print_GA_soldiers()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}

				/* Print all Satrapy soldiers
				 * Y */
			case 'Y':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (print_S_soldiers()) {
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

	free_all();
	return (EXIT_SUCCESS);
}
