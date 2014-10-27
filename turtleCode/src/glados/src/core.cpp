#include <ros/ros.h>
#include <queue> 

class State{

public:
	bool persist;
	int pos;
	void run();
	void setPos(int position){
		pos=position;
	}



};
class EatState:public State{
	void run(){
		std::cout<<"to be implemented"<<std::endl;
	}
public:	EatState(){
		persist=true;
	}
};
class DanceState:public State{
public:	DanceState(){
		persist=false;
	}
	void run(){
		std::cout<<"to be implemented"<<std::endl;
	}
};
class IdleState:public State{
public:	IdleState(){	
		persist=false;
	}
	void run(){
		std::cout<<"to be implemented"<<std::endl;
	}
};
class Core{

private:
	ros::Subscriber sub;
	ros::NodeHandle nh_;
  	std::priority_queue<int> StatesToDo;

	const static int numberOfStates=3;
   	State* states[numberOfStates] = {new EatState,new DanceState,new IdleState};

public:
	Core(int argc,char** argv):nh_("~"){
		ros::init(argc,argv, "Core");
		for(int i=0;i<numberOfStates.size();i++){
			(*states[i]).pos=i;
		}		
	};
	void action(){
		states[numberOfStates.first].run();
	}
	void removeNonPersistant(){
		for(int i=0;i<StatesToDo.size();i++){
			if(states[numberOfStates.get(i)].persist==false){
				StatesToDo.remove(numberOfStates.get(i));
			}
		}
	}
	
};


/**
*launch the node and create the core 
*/
int main(int argc,char** argv){
	Core c(argc,argv);	
}
