#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define LM1   2  //D4-in3
#define LM2   0  //D3-in4
#define RM1   15  //D8-in2
#define  RM2   13  //D7-in1
#define rightMotorENB  14  /* GPIO14(D5) -> Motor-B Enable */
#define leftMotorENB  12  // D6-enA

#define speaker 1 //TX


#define MS 4 //D2 middle sensor
#define RS 3 //RX  right sensor
#define LS 5 //D1  left sensor

#define ledPin 16  //D0 LED connected to digital pin 16
int runMode = LOW;

char auth[] = "8d619b5375564d708f7b878e6cc9935f"; //Token cua Blynk
char ssid[] = "Welcome"; //Ten wifi
char pass[] = "autoconnect"; //Pass wifi

void forward() {
  digitalWrite(leftMotorENB,HIGH);
  digitalWrite(rightMotorENB,HIGH);
  
  digitalWrite(LM1, LOW);
  digitalWrite(LM2, HIGH);
  digitalWrite(RM1, LOW);
  digitalWrite(RM2, HIGH);
}

void backward() {
  digitalWrite(leftMotorENB,HIGH);
  digitalWrite(rightMotorENB,HIGH);
  
  digitalWrite(LM1, HIGH);
  digitalWrite(LM2, LOW);
  digitalWrite(RM1, HIGH);
  digitalWrite(RM2, LOW);
}

void right() {
  digitalWrite(leftMotorENB,HIGH);
  digitalWrite(rightMotorENB,HIGH);
  
  digitalWrite(LM1, LOW);
  digitalWrite(LM2, HIGH);
  digitalWrite(RM1, LOW);
  digitalWrite(RM2, LOW);
}

void left() {
  digitalWrite(leftMotorENB,HIGH);
  digitalWrite(rightMotorENB,HIGH);
  
  digitalWrite(LM1, LOW);
  digitalWrite(LM2, LOW);
  digitalWrite(RM1, LOW);
  digitalWrite(RM2, HIGH);
}

void Stop() {
  digitalWrite(leftMotorENB,LOW);
  digitalWrite(rightMotorENB,LOW);
  
  digitalWrite(LM1, LOW);
  digitalWrite(LM2, LOW);
  digitalWrite(RM1, LOW);
  digitalWrite(RM2, LOW);
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);

  pinMode(LM1, OUTPUT);
  pinMode(LM2, OUTPUT);
  pinMode(RM1, OUTPUT);
  pinMode(RM2, OUTPUT);

  pinMode(leftMotorENB, OUTPUT); 
  pinMode(rightMotorENB, OUTPUT);

  pinMode(speaker, OUTPUT);
  
  pinMode(LS, INPUT);
  pinMode(RS, INPUT);
  pinMode(MS, INPUT);

  pinMode(ledPin, OUTPUT);      // sets the digital pin as output

  digitalWrite(ledPin,LOW); 
}

//auto mode
void autoRun(){
    Serial.println(digitalRead(LS));
    Serial.println(digitalRead(MS));
    Serial.println(digitalRead(RS));
    Serial.println("->");
    if(digitalRead(MS)) // Move Forward on line
    {
      forward();
      Serial.println("TIEN");
    }
    else if(digitalRead(LS)) // turn left by rotationg left motors in forward and right ones in backward direction
    {
      left();
      Serial.println("TRAI");
    }
    else if(digitalRead(RS)) // Turn right by rotating right motors in forward and left ones in backward direction
    {
      right();
      Serial.println("PHAI");
    }  
    else if(!(digitalRead(LS)) && !(digitalRead(MS)) && !(digitalRead(RS)))
    {
      Stop();
      Serial.println("MAT LINE!! DUNG -> delay 0.1s -> lui...");
      delay(100);
      while(!(digitalRead(LS)) && !(digitalRead(MS)) && !(digitalRead(RS))){
        backward();
        delayMicroseconds(1);   
      }
    }
    else{
      forward();
      Serial.println("len");
    }       
}

//change robot run mode 
BLYNK_WRITE(V3){
  int pinV3 = param.asInt();
  Serial.println(pinV3);
  if(pinV3==1){
    digitalWrite(ledPin,HIGH);
    runMode = HIGH;
  }
  else{
    digitalWrite(ledPin,LOW);
    runMode = LOW;
    Stop();
  }
}

//control by joystick
BLYNK_WRITE(V1) {
  int x = param[0].asInt();
  int y = param[1].asInt();
  Serial.println(x);
  Serial.println(y);
  Serial.println("->");
  if ( (x>90) && (x<270) && (y>270) ){
    forward();
    Serial.println("tien");
  }   
  else if ((x>90) && (x<270) && (y<90)){
    backward();
    Serial.println("lui");
  }  
  else if ((y>=90) && (y<=270) && (x>=270)){
    right();
    Serial.println("Phai");
  } 
  else if ((y>=90) && (y<=270) && (x<=90)){
    left();
    Serial.println("Trai");
  } 
  else if((x==180) && (y==180)){
    Stop();
    Serial.println("Dung");
  }
  else
    Serial.println("???");
}

//speaker
BLYNK_WRITE(V5){
  int pinValue = param.asInt(); // assigning incoming value from pin V5 to a variable
  Serial.println(pinValue);
  if(pinValue==1){
    digitalWrite(speaker,HIGH);
    Serial.println("bim bim....");
    delay(250);
  }
    
  else{
    digitalWrite(speaker,LOW);
    Serial.println("nothing....");
    delay(150);
  }
}

//co den la dk, ko den la auto
void loop() {
  Blynk.run();
  Serial.println(String(runMode));
  if(runMode==HIGH){
    autoRun();
    delayMicroseconds(1);
    Serial.println("auto run");
  }
}
