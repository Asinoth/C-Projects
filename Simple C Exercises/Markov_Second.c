#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define max 100

int check();
int create();


int main(int argc, char * argv[])
{
   int n;
        if(strcmp(argv[1],"-check")==0)
          {
          scanf("%d",&n);
          check(n);
          }
        if(strcmp(argv[1],"-create")==0)
          {
            int n=atoi(argv[2]);
               create(n);
        }

return 0;
}

int check(int n){

   int array[max][max];

   int i,j,sum,a,count;
   int must=(n*(n*n+1)/2);
   int flag,flag2;
   flag=0;
   flag2=0;

   /*filling up*/
   for(i=0;i<n;i++){
      for(j=0;j<n; j++){
         scanf("%d",&array[i][j]);
      }
   }
   /*checking row*/
   for(i=0;i<n;i++){
      sum=0;
      for(j=0;j<n;j++){
          sum=sum+array[i][j];
      }
      if(sum==must){printf("ROW %d YES\n",i+1);}else{printf("ROW %d NO\n",i+1); flag=1;}
    }

    /*checking column*/
    for(j=0;j<n;j++){
      sum=0;
      for(i=0;i<n;i++){
          sum=sum+array[i][j];
      }
      if(sum==must){printf("COLUMN %d YES\n",j+1);}else{printf("COLUMN %d NO\n",j+1); flag=1;}
    }

    /*checking diagonal*/
    /*1*/

     sum=0;
     for(i=0;i<n;i++){
       for(j=0;j<n;j++){
         if(i==j){
           sum=sum+array[i][j];
           }
       }
     }
      if(sum==must){ printf("DIAG1 YES\n");}else{ printf("DIAG1 NO\n"); flag=1; }


    /*2*/

     sum=0;
     for(i=0;i<n;i++){
       for(j=0;j<n;j++){
         if(j+i==n-1){
           sum=sum+array[i][j];
           }
       }
     }
     if(sum==must){ printf("DIAG2 YES\n"); }else{ printf("DIAG2 NO\n"); flag=1; }

    /*Unique*/

   for(i=0;i<n;i++){
     for(j=0;j<n;j++){
        if(array[i][j]< 1) { flag2=1; }
        if(array[i][j]>(n*n)){ flag2=1; }
     }
   }

        /*2nd*/
    for(a=1;a<=(n*n);a++){
        count=0;
      for(i=0;i<n;i++){
        for(j=0;j<n;j++){
                if(array[i][j]==a){ count++; }
                if(count>1){ flag2=1; break; }
         }
      }
     }

     if(flag2==0){printf("UNIQUE YES\n");}else if(flag2==1){printf("UNIQUE NO\n"); flag=1;}

    /*Magic*/
    if(flag==0){ printf("MAGIC YES\n"); } else if(flag==1){ printf("MAGIC NO\n"); }

return 0;
}
int create(int n){
int k,i,j;
int i1=n-1;
int j1=n/2;
int array[max][max];

   for(i=0;i<n;i++){
      for(j=0;j<n;j++){
         array[i][j]=0;
      }
   }

   for(k=1;k<=(n*n);k++){

     array[i1][j1]=k;
     j=j1+1;
     i=i1+1;
     if(i==n){i=0;}
     if(j==n){j=0;}

     if(array[i][j]>0){ i=i1-1; j=j1; }

     j1=j;
     i1=i;
   }

printf("%d\n",n);

 for(i=0;i<n;i++){
    for(j=0;j<n;j++){
        printf("%d ",array[i][j]);
    }
    printf("\n");
 }
return 0;
}


