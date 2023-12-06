#include<stdio.h>
#include<strings.h>
#include<ctype.h>
#include<stdlib.h>
#define size 200

int main(int argc,char *argv[]){
        int encrypt();
        int decrypt();

                int n;
        scanf("%d",&n);

     if(strcmp(argv[1] ,"-encrypt") == 0 ){
          encrypt(n, argv[2]);
     } else
     if (strcmp(argv[1] ,"-decrypt") == 0 ){
          decrypt(n,argv[2]);
     }
return 0;
}

int encrypt(int n,char *cipher){
        char *alph="ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    char *inputstring[n];
    int a,i,j,flag;

        for(i=0;i<=n;i++){
                inputstring[i]=malloc(200*sizeof(char)+1);
                fgets(inputstring[i], 200*sizeof(char) , stdin);
    }

  for(a=0;a<=n;a++){
     //using toupper
        for(i=0;inputstring[a][i]!='\0';i++){
               inputstring[a][i]= toupper(inputstring[a][i]);
        }
     //replace characters that are not included in cipher with ' '
            for(i=0;inputstring[a][i]!='\0';i++){
                if(inputstring[a][i]==' ') continue;
                if(inputstring[a][i]==10||inputstring[a][i]==13) {inputstring[a][i]=' '; continue;}
                flag=0;
                if(inputstring[a][i]=='?') { inputstring[a][i]=' '; continue; }
                for(j=0;alph[j]!='\0';j++){
                        if(inputstring[a][i]==alph[j]){ flag=1; break; }
                }
                if(flag==1){ continue; }else{inputstring[a][i]='?'; }
        }
         //encrypting
    for(i=0;inputstring[a][i]!='\0';i++){
                if(inputstring[a][i]==' '){ i++; }
                for(j=0;alph[j]!='\0';j++){
                        if(inputstring[a][i]==alph[j]){inputstring[a][i]=cipher[j];  break; }
                 }
         }
  }

  for(a=1;a<=n;a++){
        printf("%s\n",inputstring[a]);
  }
return 0;
}
int decrypt(int n,char *cipher){
        char *alph="ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    char *inputstring[n];
    int i,j,a;

        for(i=0;i<=n;i++){
                inputstring[i]=malloc(200*sizeof(char)+1);
                fgets(inputstring[i], 200*sizeof(char) , stdin);
    }
        for(a=0;a<=n;a++){
          //turn ? into spaces
        for(i=0;inputstring[a][i]!='\0';i++){
                if(inputstring[a][i]==' ') continue;
                if(inputstring[a][i]==10||inputstring[a][i]==13) {inputstring[a][i]=' '; continue; }
                        if(inputstring[a][i]=='?') inputstring[a][i]=' ';
            }
        //decrypting
            for(i=0;inputstring[a][i]!='\0';i++){
                for(j=0;cipher[j]!='\0';j++){
                        if(inputstring[a][i]==cipher[j]) { inputstring[a][i]=alph[j]; break; }
                }
            }
        }

        for(a=1;a<=n;a++){
        printf("%s\n",inputstring[a]);
    }

return 0;
}


