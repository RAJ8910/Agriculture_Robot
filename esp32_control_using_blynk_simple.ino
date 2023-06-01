   #define BLYNK_TEMPLATE_ID "TMPL3tsTk9rjK"
#define BLYNK_TEMPLATE_NAME "Agriculture Robot"
#define BLYNK_PRINT Serial
#define MOISTURE_SENSOR_PIN 39
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>

Servo servo_m; 
Servo servo_p;  
Servo servo_s;


char auth[] = "bvH2fSjpHSXKtc5Cc0SOgk2Nk8GT5FAc";
char ssid[] = "JARVIS";
char pass[] = "12345678";
int flag=0;
float x = 128;
float y = 128;

int seed_flag=0;
int LED_BUILTIN=2;
int m_flag=0;
int p_flag=0;
int GAUGE_VIRTUAL_PIN = V3;
int relayPin = 13;
int   flag_seed=0;
// Motor A connections
const int motorAPin1 = 15;
const int motorAPin2 = 2;

// Motor B connections
const int motorBPin1 = 4;
const int motorBPin2 = 5;

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT); 
  Blynk.begin(auth, ssid, pass,"blynk.cloud", 80); 
  Blynk.virtualWrite(GAUGE_VIRTUAL_PIN, 0);
  servo_m.attach(21);
  servo_p.attach(22);
  servo_s.attach(23);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);
  pinMode(motorAPin1, OUTPUT);
  pinMode(motorAPin2, OUTPUT);
  pinMode(motorBPin1, OUTPUT);
  pinMode(motorBPin2, OUTPUT);
}

void get_moisture(){
 
    for(int a = 0; a<= 90; a += 20){
      servo_m.write(a);
      }
    
    delay(500);
  int i=0;
    while(i<3){
    int moistureValue = analogRead(MOISTURE_SENSOR_PIN);
    int moisturePercentage = map(moistureValue, 4095, 0, 0, 100);
    Blynk.virtualWrite(GAUGE_VIRTUAL_PIN, moisturePercentage);
    Serial.print("Moisture sensing:");
    Serial.println(moisturePercentage);
    delay(1000);
    i++; 
  }
  for(int a = 90; a <= 0; a -= 20){
      servo_m.write(a);
      }
}





void moveServo() {
  
  Serial.println("Seeding on");
  
  
  for (int angle = 30; angle >= 0; angle -= 15) {
    servo_s.write(angle);
    delay(100);  // Adjust the delay according to your needs
  }
for (int angle = 0; angle <= 30; angle += 15) {
    servo_s.write(angle);
    delay(100);  // Adjust the delay according to your needs
  }
  
  

}



// Get the joystick values
BLYNK_WRITE(V0) {
  x = param[0].asInt();
}
BLYNK_WRITE(V1) {
  y = param[0].asInt();
}


BLYNK_WRITE(V2) {
  int state= param.asInt();
   if(state==1 && flag==0){
    get_moisture(); 
   }
     
}


BLYNK_WRITE(V4) {
 int state= param.asInt();
 if (state==1 && flag==0){
  digitalWrite(relayPin, LOW);
  Serial.println("Sprinkle on");
  }
  else if(flag==1)
  {
    Blynk.virtualWrite(V4, LOW);
    }
  else{
    Serial.println("Sprinkle off");
    digitalWrite(relayPin, HIGH);
    }
}
BLYNK_WRITE(V5) {
int state= param.asInt(); 
 
 if (state==1 && flag==0) 
  {
    seed_flag=1;
  }
  else if(flag==1)
  {
    Blynk.virtualWrite(V6, LOW);
    }
  else if( state==0 ){ 
    seed_flag=0;
     
  }
  
}
  

BLYNK_WRITE(V6) {
 int state= param.asInt(); 
 
 if (state==1 && flag==0) 
  {
    Serial.println("ploughing on");
    for (int angle = 90; angle >= 0; angle -= 30) {
    servo_p.write(angle);
    delay(100);  // Adjust the delay according to your needs
  }
  }
  else if(flag==1)
  {
    Blynk.virtualWrite(V6, LOW);
    }
  else{ 
    for (int angle = 90; angle >= 0; angle -= 30) {
    servo_p.write(angle);
    delay(100);  // Adjust the delay according to your needs
  }
    Serial.println("ploughing off"); 
  }
  }

BLYNK_WRITE(V7) {
 int state= param.asInt();
 if(state==1 && flag==0){
  flag =1;
  Blynk.logEvent("automatic_mode", " Automatic is turned on!");
  Serial.println("Automatic on");
  }
  else{
    flag=0;
    Serial.println("Automatic off");
    }
}




// Check these values using the IF condition
void smartcar() {
  if (y > 200) {
    carForward();
    
  } else if (y < 45) {
    carBackward();
    
  } else if (x < 110) {
    carLeft();
    
  } else if (x > 135) {
    carRight();
   
  } else  {
    carStop();
    
  }
}

void loop() {
  
  Blynk.run();// Run the blynk function
  servo_s.write(30);
  if (flag==0) smartcar();// Call the main function
  if (flag==0 && seed_flag==1){
    moveServo();
    }
}




/**************Motor movement functions*****************/
void carForward() {
  Serial.println("carForward");
  digitalWrite(motorAPin1, HIGH);
  digitalWrite(motorAPin2, LOW);
  digitalWrite(motorBPin1, HIGH);
  digitalWrite(motorBPin2, LOW);
}
void carBackward() {
  Serial.println("carBackward");
  digitalWrite(motorAPin1, LOW);
  digitalWrite(motorAPin2, HIGH);
  digitalWrite(motorBPin1, LOW);
  digitalWrite(motorBPin2, HIGH);
}
void carLeft() {
  Serial.println("carLeft");
  digitalWrite(motorAPin1, HIGH);
  digitalWrite(motorAPin2, LOW);
  digitalWrite(motorBPin1, LOW);
  digitalWrite(motorBPin2, HIGH);
}
void carRight() {
  Serial.println("carRight");
  digitalWrite(motorAPin1, LOW);
  digitalWrite(motorAPin2, HIGH);
  digitalWrite(motorBPin1, HIGH);
  digitalWrite(motorBPin2, LOW);
}
void carStop() {
  //Serial.println("carstop");
  digitalWrite(motorAPin1, LOW);
  digitalWrite(motorAPin2, LOW);
  digitalWrite(motorBPin1, LOW);
  digitalWrite(motorBPin2, LOW);

}
