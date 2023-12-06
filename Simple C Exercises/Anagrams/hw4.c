#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#define MAX 1000 /*orio grammis */

void is_it_anagram(char array1[],char array2[],FILE *f1, FILE *f2);

int main(int argc, char *argv[]){
  int i=0,c1,c2;
  int sum1=0,sum2=2;
  FILE *file1, *file2 ;
  char str1[MAX],str2[MAX];
  if (argc!=3){  /*an dinei parapanw-ligotera files */
    printf("error\n");
    return 1;
  }
  file1=fopen(argv[1],"r");
  file2=fopen(argv[2],"r");

  if(file1==NULL||file2==NULL){ /*an einai adeia i den mporoun na anoiksoun*/
    printf("kapoio arxeio den mporei na anoiksei\n");
    exit(1);
  }

    while ((fgets(str1,1000,file1)!=NULL)&&(fgets(str2,1000,file2))!=NULL){ /*bazei tin mia seira se pinaka*/
        for(i=0;i<strlen(str1);i++){
      str1[i]=toupper(str1[i]);}
      for(i=0;i<strlen(str2);i++){
      str2[i]=toupper(str2[i]);}
      is_it_anagram(str1,str2,file1,file2);
    }
   c1=fgetc(file1);
   c2=fgetc(file2);

  if (c1==EOF && c2!=EOF){ /*an kapoio teleiwnei nwritera*/
    printf("TRAILING LINES IN tests/test4/file2.in\n");
  }else if (c1!=EOF && c2==EOF) {
    printf("TRAILING LINES IN tests/test4/file1.in\n");
  }

  return 0;
}

void is_it_anagram(char array1[],char array2[],FILE *f1,FILE *f2){
  int first[256] = {0}, second[256] = {0}, c = 0, n=0;
    while (array1[c] != '\0'){
      if(array1[c]!=' '){
      first[array1[c]]++;
    }
    c++;
      }
     c = 0;
    while (array2[c] != '\0') {
      if(array2[c]!=' '){
        second[array2[c]]++;
  }
        c++;
}
     for (c = 0; c <256; c++)
     {
        if (first[c] != second[c])
           n=1;
     }
     if (n==1) {
       printf("FALSE\n");
     }else{
       printf("TRUE\n");
     }

}
