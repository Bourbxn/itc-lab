//Button Variables
#define buttonWalk 3
#define buttonSouth 4
#define buttonWest 5
bool buttonWalkState;
bool buttonSouthState;
bool buttonWestState;

//Red Yellow Green Variables
#define walkR 6
#define walkG 7
#define southR 8
#define southY 9
#define southG 10
#define westR 11
#define westY 12
#define westG 13

//State Variables
#define goS 0
#define waitS 1
#define goW 2
#define waitW 3
#define goWalk 4
#define waitWalk 5

//State 
struct State{
  unsigned long streetOut;
  unsigned long timeDelay;
  unsigned long nextState[6];
};

typedef const struct State sType;
sType FSM[6]={
  {},
  {},
  {},
  {},
  {},
  {}
};

void setup() {
  Serial.begin(9600);
  //all Button in INPUT Mode
  for(int i=3;i<=5;i++) pinMode(i,INPUT); 
  //all LED in OUPTPUT Mode
  for(int i=6;i<=13;i++) pinMode(i,OUTPUT);
}

void loop() {
  digitalWrite(7,80);
  delay(1000);
}
