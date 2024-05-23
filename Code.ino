#include <IRremote.h>

// Motor pins
#define ENA 9
#define IN1 2
#define IN2 3
#define IN3 4
#define IN4 5

// Ultrasonic sensor pins
#define TRIG1 6
#define ECHO1 7
#define TRIG2 8
#define ECHO2 9
#define TRIG3 10
#define ECHO3 11

// IR receiver pin
#define RECV_PIN 12

IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  // Motor pins as output
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  // Ultrasonic sensor pins
  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);
  pinMode(TRIG3, OUTPUT);
  pinMode(ECHO3, INPUT);
  
  // IR receiver
  irrecv.enableIRIn();
}

void loop() {
  // Obstacle avoidance logic
  long duration1, distance1;
  long duration2, distance2;
  long duration3, distance3;
  
  digitalWrite(TRIG1, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG1, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG1, LOW);
  duration1 = pulseIn(ECHO1, HIGH);
  distance1 = duration1 * 0.034 / 2;
  
  digitalWrite(TRIG2, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG2, LOW);
  duration2 = pulseIn(ECHO2, HIGH);
  distance2 = duration2 * 0.034 / 2;
  
  digitalWrite(TRIG3, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG3, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG3, LOW);
  duration3 = pulseIn(ECHO3, HIGH);
  distance3 = duration3 * 0.034 / 2;
  
  if (distance1 < 20 || distance2 < 20 || distance3 < 20) {
    // Obstacle detected
    stop();
    delay(500);
    // Implement avoidance strategy here
    backward();
    delay(500);
    right();
    delay(500);
  } else {
    forward();
  }

  // IR remote control logic
  if (irrecv.decode(&results)) {
    switch(results.value) {
      case 0xFFA25D: forward(); break; // "Forward" button
      case 0xFFE21D: backward(); break; // "Backward" button
      case 0xFF22DD: left(); break; // "Left" button
      case 0xFFC23D: right(); break; // "Right" button
      case 0xFF02FD: stop(); break; // "Stop" button
    }
    irrecv.resume(); // Receive the next value
  }
}

void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 255);
}

void backward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 255);
}

void left() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 255);
}

void right() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 255);
}

void stop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
}