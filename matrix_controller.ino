#define LED_DATA 2
#define LED_CS 3
#define LED_CLK 4
#define MAX_DEVICES 4

#define OP_NOOP   0
#define OP_ROW0 1
#define OP_ROW1 2
#define OP_ROW2 3
#define OP_ROW3 4
#define OP_ROW4 5
#define OP_ROW5 6
#define OP_ROW6 7
#define OP_ROW7 8
#define OP_DECODEMODE  9
#define OP_INTENSITY   10
#define OP_SCANLIMIT   11
#define OP_SHUTDOWN    12
#define OP_DISPLAYTEST 15

void spiTransfer(int addr, byte opcode, byte data){
    byte spidata[16];
    int offset=addr*2;
    int maxbytes=MAX_DEVICES*2;
    spidata[offset+1]=opcode;
    spidata[offset]=data;

    //enable the line 
    digitalWrite(LED_CS,LOW);
    for(int i=maxbytes-1;i>=0;i--){
      for (int j = 0; j < 8; j++)  {//for each bit
        digitalWrite(LED_DATA, !!(spidata[i] & (1 << (7-j))));
        digitalWrite(LED_CLK, HIGH);
        digitalWrite(LED_CLK, LOW);
      }
    }
    digitalWrite(LED_CS,HIGH);
}

void setup(){
    pinMode(LED_DATA,OUTPUT);
    pinMode(LED_CLK,OUTPUT);
    pinMode(LED_CS,OUTPUT);
    digitalWrite(LED_CS,HIGH);

    for(int i=0;i<MAX_DEVICES;i++) {
        spiTransfer(i,OP_DISPLAYTEST,0);//off
        spiTransfer(i, OP_SCANLIMIT,7);
        spiTransfer(i, OP_INTENSITY, 0);//0-15
        spiTransfer(i,OP_DECODEMODE,0);//off
        spiTransfer(i,OP_SHUTDOWN,1);//off
        for(int j=0; j<8; j++){//each row
          spiTransfer(i, j+1, 0);
        }
    }
}

void loop() {
  for(int i=0;i<MAX_DEVICES;i++) {
    spiTransfer(i,OP_DISPLAYTEST,0);//off
    spiTransfer(i, OP_SCANLIMIT,7);
    spiTransfer(i, OP_INTENSITY, 0);//0-15
    spiTransfer(i,OP_DECODEMODE,0);//off
    spiTransfer(i,OP_SHUTDOWN,1);//off
    for(int j=0; j<8; j++){//each row
      spiTransfer(i, j+1, 0xAA);
    }
  }
  for(int i=0;i<MAX_DEVICES;i++) {
    spiTransfer(i,OP_DISPLAYTEST,0);//off
    spiTransfer(i, OP_SCANLIMIT,7);
    spiTransfer(i, OP_INTENSITY, 0);//0-15
    spiTransfer(i,OP_DECODEMODE,0);//off
    spiTransfer(i,OP_SHUTDOWN,1);//off
    for(int j=0; j<8; j++){//each row
      spiTransfer(i, j+1, 0x55);
    }
  }
}
