#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/**
* listens to the "/questions" and "/balls" topics and sends messages to the "/makeSound", "/drive" and "/display" topic to make sounds, drive around and display various the question. 
*/ 
int int_balls[] = {0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9};
std::vector<int> avaible_balls (int_balls, int_balls + sizeof(int_balls) / sizeof(int) );

int main(){
	 srand (time(NULL));
	 
}


/* returns array of 3 integers first 2 are the once being summed third is the answer*/
int* Addition2dig(){
	int* result =new int[3];
	//select indexes 
	int a;
	int b;
	do{
		a=avaible_balls[rand()%avaible_balls.size()];
		b=avaible_balls[rand()%avaible_balls.size()];
	}while(a!=b);
		result[2]=a*10+b; 
		result[1]=rand()%result[2];
		result[0]=result[2]-result[1];
	return result; 
}

int* Addition1dig(){
	int* result =new int[3];
	avaible_balls[rand()%avaible_balls.size()];
	result[1]=rand()%result[2];
		result[0]=result[2]-result[1];
	return result; 
}
