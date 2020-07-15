// Pins
const uint8_t LED_PIN = 13;
const uint8_t MOTION_PIN = 2;
const uint8_t TILT_PIN = 3;

// Globals
int motion_value = 0;
int tilt_value = 0;

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  pinMode(MOTION_PIN, INPUT);
  pinMode(TILT_PIN, INPUT);
  
  attachInterrupt(digitalPinToInterrupt(MOTION_PIN), detect_motion, CHANGE);
  attachInterrupt(digitalPinToInterrupt(TILT_PIN), check_tilt, CHANGE);
  Serial.begin(9600);
}

void loop()
{

  delay(1000); 
}

void detect_motion()
{
  motion_value = digitalRead(MOTION_PIN);
  Serial.print("Motion Pin: ");
  Serial.println(motion_value);
  if(motion_value == HIGH) // Motion dectected
  {
    digitalWrite(LED_PIN, HIGH); // Turn on LED
    Serial.println("Motion Detected");
  } else 
  {
  	digitalWrite(LED_PIN, LOW); // Turn off LED
    Serial.println("Motion Ended");
  }
}

void check_tilt()
{
  tilt_value = digitalRead(TILT_PIN);
  Serial.print("Tilt Pin: ");
  Serial.println(tilt_value);
  if(tilt_value == HIGH) // Measure tilt
  {
    digitalWrite(LED_PIN, HIGH); // Turn on LED
    Serial.println("Object is tilted");
  } else 
  {
  	digitalWrite(LED_PIN, LOW); // Turn off LED
    Serial.println("Object is balance");
  }
}