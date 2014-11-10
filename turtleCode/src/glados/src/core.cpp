/*core listens to the topic*/
#include <ros/ros.h>
#include <queue> 

class State{

public:
	bool persist;
	int pos;
	int priority; 
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


  	std::map<int, State*> StatePriorities; 
  	std::map<int, State*> StateToDo; 


	
	

public:
	Core(int argc,char** argv):nh_("~"){

		
		StatePriorities[0]= new EatState;
		StatePriorities[1]= new DanceState;
		StatePriorities[2]= new IdleState;	
		
		for(int i=0;i<StatePriorities.size();i++){
			StatePriorities[i]->priority=i;
		}		
	};

	void action(){
		StateToDo.begin()->second->run();
	}
	void removeNonPersistant(){
		for (std::map<int, State*>::iterator it=StateToDo.begin(); it!=StateToDo.end(); ++it){
			
			std::cout << it->first << " => " << it->second << '\n';
		}
	}	
};


/**
*launch the node and create the core 
*/
int main(int argc,char** argv){
	ros::init(argc,argv, "Core");
	Core c(argc,argv);	

}

