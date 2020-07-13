int LED_PIN = 13;
int MOTION_PIN = 2;
int motion_value = 0;

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  pinMode(MOTION_PIN, INPUT);
  Serial.begin(9600);
}

void loop()
{
  motion_value = digitalRead(MOTION_PIN);
  if(motion_value == HIGH) // Motion dectected
  {
    digitalWrite(LED_PIN, HIGH); // Turn on LED
    Serial.println("Motion Dectected");
    delay(2000);
  } else 
  {
  	digitalWrite(LED_PIN, LOW); // Turn off LED
    Serial.println("Motion Ended");
    delay(1000);
  }
}
