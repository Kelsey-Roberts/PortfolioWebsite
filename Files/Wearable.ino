int potpin = A0;  // elbow
int pot2pin = A1; // wrist

int val1;
int val2;

void setup() {
  Serial.begin(38400); 
}

void loop() {

  // Read raw pots
  val1 = analogRead(potpin);
  val2 = analogRead(pot2pin);

  // Clamp to your measured ranges (Option A)
  if(val1 < 24)  val1 = 24;
  if(val1 > 110) val1 = 110;

  if(val2 < 350) val2 = 350;
  if(val2 > 720) val2 = 720;

  // Build fixed 3-digit fields
  char msg[12];

  int checksum = (val1 + val2) % 10;

  // Format S### ###C
  sprintf(msg, "S%03d%03d%d", val1, val2, checksum);

  Serial.println(msg);

  delay(10); // ~100 Hz update rate
}
