#include <stdio.h>


struct node {
	char k;
};


struct node s[100];

int main() {
  char c;
  int i;

int node_num = 0;
  while(1){
	  scanf("%c",&c);
	  if(c  != '0'){
		   s[node_num].k = c;
	  printf("EGRAPSA : %c sto s[]node_num to : %c\n",c,s[node_num].k);
	  }else{
		  break;
	  }
	 
	  node_num++;
  }
	
 for(i =0;i<100;i++){
 
	 printf("GRAMMA %c position : %d\n",s[i].k,i);
}
	
  return 0;
}