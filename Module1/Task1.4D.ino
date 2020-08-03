// Pins
const int LED_PIN = PB5;
const uint8_t MOTION_PIN = 2;
const uint8_t TILT_PIN = 3;
const uint8_t LIGHT_PIN = 4;

// Counter and compare match values
const uint16_t t1_load = 0;
const uint16_t t1_comp = 31250;

// Globals
int motion_value = 0;
int tilt_value = 0;
int light_value = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(MOTION_PIN, INPUT);
  estalish_group(TILT_PIN);
  estalish_group(LIGHT_PIN);
  estalish_group(MOTION_PIN);
  
  // Set LED pin to be output
  DDRB |= (1 << LED_PIN);
  
  // Reset Timer1 Control Reg A
  TCCR1A = 0;
  
  // Set CTC mode
  TCCR1B &= ~(1 << WGM13);
  TCCR1B |= (1 << WGM12);
  
  // Set to presacler of 256
  TCCR1B |= (1 << CS12);
  TCCR1B &= ~(1 << CS11);
  TCCR1B &= ~(1 << CS10);
  
  // Reset Timer1 and set compare value
  TCNT1 = t1_load;
  OCR1A = t1_comp;
  
  // Enable Timer1 and set compare value
  TIMSK1 = (1 << OCIE1A);
  
  // Enable global interrupts
  sei();
}

void loop()
{
}

void detect_motion()
{
  motion_value = digitalRead(MOTION_PIN);
  Serial.print("Motion Pin: ");
  Serial.println(motion_value);
  if(motion_value == HIGH) // Motion dectected
    Serial.println("Motion Detected");
  else 
    Serial.println("Motion Ended");
}

void check_tilt()
{
  tilt_value = digitalRead(TILT_PIN);
  Serial.print("Tilt Pin: ");
  Serial.println(tilt_value);
  if(tilt_value == HIGH) // Measure tilt
    Serial.println("Object is tilted");
  else 
    Serial.println("Object is balance");
}
void check_light()
{
  light_value = digitalRead(LIGHT_PIN);
  Serial.print("Light Pin: ");
  Serial.println(light_value);
  if(light_value == HIGH) // Measure light
    Serial.println("Light appears");
  else 
    Serial.println("Light disappears");
}

void estalish_group(byte pin)
{
    *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
    PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
    PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
}
ISR (PCINT2_vect) // handle pin change interrupt for D0 to D7 here
{
  if(digitalRead(LIGHT_PIN) != light_value)
    light_value = digitalRead(LIGHT_PIN);
  if(digitalRead(TILT_PIN) != tilt_value)
    tilt_value = digitalRead(TILT_PIN);
  if(digitalRead(MOTION_PIN) != motion_value)
    motion_value = digitalRead(MOTION_PIN);
} 
ISR(TIMER1_COMPA_vect)
{
  PORTB ^= (1 << LED_PIN);
  check_tilt();
  check_light();
  detect_motion();
}