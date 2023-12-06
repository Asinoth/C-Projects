#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXN 10
#include <time.h>



int main(int argc, const char* argv[]) {
	
	int N,i,j,ROW,COLUMN,k,l,f=1,number;
	float sum=0.0;
	float R,max,max1,max2;
	float Mark[MAXN][MAXN];
	float sumc[MAXN],sumr[MAXN];
	if(argc>3){
		printf("dwse ti thes na kanw!");
	}
	if(strcmp("-check",argv[1]) == 0) {
		float Mark[MAXN][MAXN];
	scanf("%d", &N);
	for(ROW=0;ROW<N;ROW++) {
		for(COLUMN=0;COLUMN<N;COLUMN++){
			scanf("%f", &Mark[ROW][COLUMN]);
		}
	}
		for(ROW=0;ROW<N;ROW++){ 
		sum=0.0;
			for(COLUMN=0;COLUMN<N;COLUMN++){
				sum = sum + Mark[ROW][COLUMN];
			} 
			if(sum==1.0){
				printf("ROW %d YES\n",ROW + 1);
				}
			else{
				printf("ROW %d NO\n",ROW + 1);
				f=0;
			}
		}
		for(COLUMN=0;COLUMN<N;COLUMN++){ 
		sum = 0;
			for(ROW=0;ROW<N;ROW++){
				sum = sum + Mark[ROW][COLUMN];
			} 
			if(sum==1.0){
				printf("COLUMN %d YES\n",COLUMN + 1);
				}
			else{
				printf("COLUMN %d NO\n",COLUMN + 1);
				f=0;
			}
		}
		if(f) {
			printf("MARKOV YES\n");
		}
		else{
			printf("MARKOV NO\n");
		}
		return 0; 
	}
	else if(strcmp("-create",argv[1]) == 0){
		/* ITS WRONG */



		/* !!!! */
		
		N = atoi(argv[2]);
		printf("%d\n",N);
				
		for(i=1;i<=N;i++) {
			sumc[i]=0.0;
			sumr[i]=0.0;
			for(j=1;j<=N;j++) {
				Mark[i][j]=0.0;}
			}
		
		for(i=1;i<=N;i++) {
			for(j=1;j<=N;j++) {
			max1=Mark[i][j];
			max2=Mark[i][j];
			for(k=1;k<=N;k++){
				if(Mark[k][j]>max1){
					max1=Mark[k][j];
				}
			
			for(l=1;l<=N;l++){
				if(Mark[i][l]>max2){
					max2=Mark[i][l];
				}
			
			if(max1>max2){
				max = max1;
			}
			else{
				max=max2;
			}
			
			if(max==0.0){
				number = rand() % 10;
				R = (float)number/10;
			}
			else if(i==N){
				R = 1 - sumc[j];
			}
			else if(j==N){
				R = 1 - sumr[i];
			}
			else{
					number = rand() % 100;
					R = (float)number / 10;
						while (R >(1-max)){
						
					number = rand() % 100;
					R = (float)number / 10; }
		
					}
			}	Mark[i][j]= R;
		sumc[j] += R;
		sumr[i] += R;
			
			}
			}
			}
		for(i=0;i<N;i++) {
			for(j=0;j<N;j++) {
				printf("%.1f ",Mark[i][j]);}
				printf("\n");
				}
	}
	else{
		printf("Dwse parametrous!!");
		return 0;
	}
		return 0;
}

	




	
	


