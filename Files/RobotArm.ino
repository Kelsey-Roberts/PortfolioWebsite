#include <LiquidCrystal.h>

// ---------------- PID STRUCT ----------------
struct PID {
  float Kp, Ki, Kd;
  float integral;
  float lastError;
};

PID pid1 = {0.9, 0.002, 0.25, 0, 0};
PID pid2 = {0.9, 0.002, 0.25, 0, 0};

unsigned long lastTime = 0;

// ---------------- PINS ----------------------
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int in1 = 8;
int in2 = 7;
int in3 = 13;
int in4 = 9;

int speedPin1 = 10; // Motor1 speed PWM
int speedPin2 = 1;  // Motor2 speed PWM, change if conflicts with Serial
int potPin = A0;
int pot2Pin = A1;

// ---------------- STATE ----------------------
int target1 = 0;
int target2 = 0;

char incoming[16];
int idx = 0;

void setup() {
  Serial.begin(38400);
  lcd.begin(16, 2);

  pinMode(speedPin1, OUTPUT);
  pinMode(speedPin2, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  lastTime = micros();
}

// ---------------- MOTOR CONTROL ----------------
void motor1(float power) {
  int pwm = constrain(abs(power), 0, 255);

  if(power > 0) {
    analogWrite(speedPin1, pwm);
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  } else if(power < 0) {
    analogWrite(speedPin1, pwm);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  } else {
    analogWrite(speedPin1, 0);
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }
}

void motor2(float power) {
  int pwm = constrain(abs(power), 0, 255);

  if(power > 0) {
    analogWrite(speedPin2, pwm);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  } else if(power < 0) {
    analogWrite(speedPin2, pwm);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  } else {
    analogWrite(speedPin2, 0);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
  }
}

// ---------------- PID FUNCTION ----------------
float runPID(PID &pid, float target, float current, float dt) {
  float error = target - current;

  pid.integral += error * dt;
  float derivative = (error - pid.lastError) / dt;
  pid.lastError = error;

  return pid.Kp * error + pid.Ki * pid.integral + pid.Kd * derivative;
}

// ---------------- MAIN LOOP -------------------
void loop() {

  // -------- SERIAL PARSING --------
  while (Serial.available()) {
    char c = Serial.read();

    if(c == 'S') { 
      idx = 0;
    }

    incoming[idx++] = c;

    if(c == '\n' || idx >= 10) {
      incoming[idx] = 0;

      if(incoming[0] == 'S') {
        int v1 = atoi(&incoming[1]);
        int v2 = atoi(&incoming[4]);
        int chk = incoming[7] - '0';

        if(((v1+v2) % 10) == chk) {
          target1 = map(v1, 24, 110, 255, 779);
          target2 = map(v2, 350, 720, 298, 673);
        }
      }
      idx = 0;
    }
  }

  // -------- TIME STEP --------
  unsigned long now = micros();
  float dt = (now - lastTime) / 1000000.0; 
  lastTime = now;

  // -------- READ CURRENT POSITIONS --------
  int pos1 = analogRead(potPin);
  int pos2 = analogRead(pot2Pin);

  // -------- PID OUTPUTS --------
  float power1 = runPID(pid1, target1, pos1, dt);
  float power2 = runPID(pid2, target2, pos2, dt);

  // -------- DRIVE MOTORS --------
  motor1(power1);
  motor2(power2);

  // -------- LCD --------
  lcd.setCursor(0,0);
  lcd.print("T1:");
  lcd.print(target1);
  lcd.print(" C:");
  lcd.print(pos1);

  lcd.setCursor(0,1);
  lcd.print("T2:");
  lcd.print(target2);
  lcd.print(" C:");
  lcd.print(pos2);
}
