#include <Servo.h>

// Pin Definitions
const int SERVO_RADAR_PIN = 3;  
const int SERVO_LASER_PIN = 11; 
const int TRIG_PIN = 9;         
const int ECHO_PIN = 10;        
const int SOUND_PIN = 2;        
const int LED_GREEN = 6;        
const int LED_YELLOW = 7;       
const int LED_RED = 8;          
const int BUZZER_PIN = 4;       
const int LASER_PIN = 5;        

Servo radarServo;
Servo laserServo;

int radarAngle = 0;
int sweepDirection = 1; 

// Time tracking for Sound Sensor (Case 2)
unsigned long soundStartTime = 0;
unsigned long lastSoundDetectedTime = 0;

void setup() {
  Serial.begin(9600);
  
  radarServo.attach(SERVO_RADAR_PIN);
  laserServo.attach(SERVO_LASER_PIN);
  radarServo.write(0);
  laserServo.write(90); // Idle pos

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(SOUND_PIN, INPUT); // Sound sensor pin

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LASER_PIN, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();

  // ---------------------------------------------------------
  // 1. SENSOR READINGS
  // ---------------------------------------------------------
  long distance = measureDistance();

  // Sound Sensor Logic (Checking for continuous sound)
  int soundState = digitalRead(SOUND_PIN); // Assuming HIGH = Sound Detected
  
  if (soundState == HIGH) {
    if (soundStartTime == 0) {
      soundStartTime = currentMillis; // Start counting time
    }
    lastSoundDetectedTime = currentMillis;
  } else {
    // Agar 500ms tak koi awaz nahi ati, toh timer reset kar do
    if (currentMillis - lastSoundDetectedTime > 500) {
      soundStartTime = 0; 
    }
  }

  // Calculate Sound Duration
  unsigned long soundDuration = 0;
  if (soundStartTime > 0) {
    soundDuration = currentMillis - soundStartTime;
  }

  // ---------------------------------------------------------
  // 2. THREAT CALCULATION (Cases 1, 2, 3)
  // ---------------------------------------------------------
  int u_threat = 0; // Ultrasonic Threat
  if (distance > 0 && distance <= 20) u_threat = 2;       // Red Zone (0 to 20 cm)
  else if (distance > 20 && distance <= 40) u_threat = 1; // Yellow Zone (20 to 40 cm)

  int s_threat = 0; // Sound Threat
  if (soundDuration > 3000) s_threat = 2;       // More than 3 sec -> Attack
  else if (soundDuration > 0) s_threat = 1;     // 0 to 3 sec -> Alert Indication

  // Overall threat is the maximum of both sensors (Case 3 combined)
  int overallThreat = max(u_threat, s_threat);

  // ---------------------------------------------------------
  // 3. ACTION PHASE (Lights, Buzzer & Laser)
  // ---------------------------------------------------------
  if (overallThreat == 0) {
    // SAFE: Green light continuously blinks every 500ms
    bool blinkState = (currentMillis / 500) % 2; 
    digitalWrite(LED_GREEN, blinkState);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    
    digitalWrite(LASER_PIN, LOW);
    laserServo.write(90); // Relax mode
  } 
  else if (overallThreat == 1) {
    // WARNING / ALERT: Yellow light & Buzzer slow blink (250ms)
    bool blinkState = (currentMillis / 250) % 2; 
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, blinkState);
    digitalWrite(LED_RED, LOW);
    digitalWrite(BUZZER_PIN, blinkState);
    
    // YAHAN CHANGE KIYA HAI: Laser servo target ko follow karegi, lekin laser OFF rahegi
    laserServo.write(radarAngle); 
    digitalWrite(LASER_PIN, LOW);
  } 
  else if (overallThreat == 2) {
    // DANGER / COUNTER ATTACK: Red light & Laser fast blink (100ms)
    bool blinkState = (currentMillis / 100) % 2; 
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, blinkState);
    
    // YAHAN CHANGE KIYA HAI: Buzzer ab continuously ON rahega (Strong Buzzer)
    digitalWrite(BUZZER_PIN, HIGH);

    // BHOOOOM! Target Destroyed 
    laserServo.write(radarAngle); // Lock target direction
    digitalWrite(LASER_PIN, blinkState); // Laser fires (blinks)
  }

  // ---------------------------------------------------------
  // 4. SEND DATA TO PROCESSING 
  // Format: Angle,Distance,U_Threat,S_Threat,OverallThreat
  // ---------------------------------------------------------
  Serial.print(radarAngle); Serial.print(",");
  Serial.print(distance); Serial.print(",");
  Serial.print(u_threat); Serial.print(",");
  Serial.print(s_threat); Serial.print(",");
  Serial.println(overallThreat);

  // ---------------------------------------------------------
  // 5. RADAR MOVEMENT
  // ---------------------------------------------------------
  radarServo.write(radarAngle);
  delay(30); // Speed of radar sweep
  radarAngle += sweepDirection;
  if (radarAngle >= 180 || radarAngle <= 0) {
    sweepDirection = -sweepDirection; 
  }
}

long measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000); 
  long dist = duration * 0.034 / 2;
  if (dist == 0 || dist > 45) dist = 200; // Ignore far objects
  return dist;
}