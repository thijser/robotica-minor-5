#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
/**
* listens to the "/questions" and "/balls" topics and sends messages to the "/makeSound", "/drive" and "/display" topic to make sounds, drive around and display various the question. 
Subscribes to:
	/questions
	/balls
	
Publishes to:
	/makeSound
	/drive
	/display	
*/ 
int int_balls[] = {0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9};
std::vector<int> avaible_balls (int_balls, int_balls + sizeof(int_balls) / sizeof(int) );


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

int* substract1dig(){
	int* ans=Addition1dig();
	int temp=ans[0];
	ans[0]=ans[2];
	ans[2]=temp;
	return ans; 
}
int* substract2dig(){
	int* ans=Addition2dig();
	int temp=ans[0];
	ans[0]=ans[2];
	ans[2]=temp;
	return ans; 		
	}

int main(){
	 srand (time(NULL));
	 int * print = Addition2dig();
	std::cout<<print[0]<<"+"<<print[1]<<"="<<print[2]<<std::endl;
	  print = Addition1dig();
	std::cout<<print[0]<<"+"<<print[1]<<"="<<print[2]<<std::endl;
	  print = substract2dig();
	std::cout<<print[0]<<"-"<<print[1]<<"="<<print[2]<<std::endl;
	print = substract1dig();
	std::cout<<print[0]<<"-"<<print[1]<<"="<<print[2]<<std::endl;
}

