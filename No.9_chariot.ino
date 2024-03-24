#include <AFMotor.h>
#include <Ultrasonic.h>

#define motorLpin 3
#define motorRpin 4
#define lightLpin A4
#define lightRpin A5chariotchariotchariot

void R_Spin();
void L_Spin();
void search_enemy();

AF_DCMotor motorR(motorRpin);
AF_DCMotor motorL(motorLpin);
 
Ultrasonic ultrasonic(9, 10);
 
bool lightL = 1;
bool lightR = 1;
bool run_ok;
int distance;
int temp;
int count = 0;
bool random_side=1;

void setup() {
  Serial.begin(9600); // set up Serial library at 9600 bps
  motorR.setSpeed(210);
  motorL.setSpeed(210);
  motorR.run(RELEASE);
  motorL.run(RELEASE);
  while (lightL && lightR) {
    lightL = digitalRead(lightLpin);
    lightR = digitalRead(lightRpin);
  }
}

void loop() {
  distance = ultrasonic.read();
  if (count == 0){
    temp = distance;
  }
  lightR = digitalRead(lightRpin);
  lightL = digitalRead(lightLpin);
  run_ok = !lightL && !lightR; // 都是才可以跑
  if (run_ok && distance <= 85 ) {
    motorR.run(FORWARD);
    motorL.run(FORWARD);
    delay(50);
  } else if (run_ok && distance > 85 ) {
    search_enemy();
    motorR.run(FORWARD);
    motorL.run(FORWARD);
    delay(50);
  } else {
    avoid();
  }
  //speed up when encounter
  
  if(count == 2){
    int y = ultrasonic.read();
    if((y - temp) <= 1 && (y - temp) >= -1){
      //speed up
      motorR.setSpeed(250);
      motorL.setSpeed(250);
    }
  }
  
  
  if(count == 90){
    int y = ultrasonic.read();
    if((y - temp) <= 1 && (y - temp) >= -1){
      //speed down
      motorR.setSpeed(180);
      motorL.setSpeed(180);
      motorR.run(BACKWARD);
      motorL.run(BACKWARD);
      delay(300);
    }
    count = -1;
  }
  
   count++;
}

void avoid() {
  if (lightR && !lightL) {
    motorR.run(BACKWARD);
    motorL.run(RELEASE);
  } else if (!lightR && lightL) {
    motorR.run(RELEASE);
    motorL.run(BACKWARD);
  } else if (lightR && lightL) {
    motorR.run(BACKWARD);
    motorL.run(BACKWARD);
  }
  delay(200);
  srand(millis());
  bool a = rand() % 2;
  if (a == 1) {
    R_Spin();
    random_side=1;
  } else {
    L_Spin();
    random_side=0;
  }
}

void R_Spin() {
  motorR.run(BACKWARD);
  motorL.run(FORWARD);
  delay(400);
}

void L_Spin() {
  motorR.run(FORWARD);
  motorL.run(BACKWARD);
  delay(400);
}

void search_enemy() {
  int x;
  motorR.setSpeed(90);
  motorL.setSpeed(90);
  if(random_side==1){
    motorR.run(BACKWARD);
    motorL.run(FORWARD);
  }
  else if (random_side==0){
    motorR.run(FORWARD);
    motorL.run(BACKWARD);
  }
  for (int i = 0; i < 120; i++) {
    x = ultrasonic.read();
    delay(5);
    if (x <= 90) {
      break;
    }
  }
  motorR.setSpeed(230);
  motorL.setSpeed(230);
}
