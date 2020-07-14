// Pins
const uint8_t LED_PIN = 13;
const uint8_t MOTION_PIN = 2;

// Globals
int motion_value = 0;

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  pinMode(MOTION_PIN, INPUT);
  
  attachInterrupt(digitalPinToInterrupt(MOTION_PIN), detect_motion, CHANGE);
  Serial.begin(9600);
}

void loop()
{
  delay(250); 
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