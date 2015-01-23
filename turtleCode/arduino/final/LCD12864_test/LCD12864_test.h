void longboot();
void setup();
void openMouth();
void waitMouth();
void semiclose();
void closeMouth();
void fullcloseMouth();
const unsigned char* getMatch(int input);
void displayfuse(int firstnumber, int operation , int secondnumber );
int getAnswer(int firstnumber, int operation , int secondnumber);
void checkInput(int input, int answer);
void displaySingle(char a,char b , char c);
void readrfid();
void dump_byte_array(byte *buffer, byte bufferSize);
void writeserial(char* input);
void loop();
void readloop(int length);

