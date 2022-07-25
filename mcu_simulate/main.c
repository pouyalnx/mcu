#include <stdio.h>


void app(unsigned int *val){
	printf("in func val %d\n",val);
	printf("in func ptr %d\n",*val);
	printf("in func addr %d\n",&val);	
}


void app1(unsigned int *val[]){
	printf("vs in func val %d\n",val);
	printf("vs in func ptr %d\n",*val);
	printf("vs in func addr %d\n",&val);	
}


int main(){
	printf("val=%x not=%x",1,~1); 
	printf("val=%x not=%x",1,!1); 
	//unsigned int val=3;
	//unsigned int vs[2];
	//vs[0]=1;
	//vs[2]=2;
	//printf("in main val %d\n",val);
	//printf("in main ptr %d\n",*(unsigned int *)val);
	//printf("in main addr %d\n",&val);	
	//app(&val);
	
	
	//printf("vs in main val %d\n",vs);
	//printf("vs in main ptr %d\n",*(unsigned int *)vs);
	//printf("vs in main addr %d\n",&vs);	
	//app1(vs);	
	
	return 0;
}
