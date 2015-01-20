char* lastmsg;
void writeserial(char* input){
    lastmsg=input;
    while(1){
    Serial.println(input);
    delay(200);
    if(Serial.available()>=4){
      byte b1,b2,b3,b4; 
      b1=Serial.read();
      if(b1='c'){
      b2=Serial.read();
      b3=Serial.read();
      b4=Serial.read();
        if(b2=='o'&&b3=='n'&&b4=='f'){
          return;
        }
      }
     
    }
  }
}

void setup() {                
  Serial.begin(9600); 
}

void action(){
  delay(2000);
  writeserial("c");
  delay(3000);
  writeserial("w");  
}

void loop(){
    if(Serial.available()>=4){
    byte b1,b2,b3,b4; 
    b1=Serial.read();
    if(b1=='s'){ //indicates a math problem is coming
        b2=Serial.read();
        b3=Serial.read();
        b4=Serial.read();    
        action();

    }else{
          if(b1=='f'){
                b2=Serial.read();
                b3=Serial.read();
                b4=Serial.read();   
                action();
 
          }else{
            
          }
    }
    }
}
