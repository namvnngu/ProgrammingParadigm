#include "arduino.hpp"
#include "board.h"
#ifdef STDIO_UART_BAUDRATE 
#define SERIAL_BAUDRATE STDIO_UART_BAUDRATE
#else
#define SERIAL_BAUDRATE 9600
#endif

const int LED_PIN = 13;
const int MOTION_PIN = 2;
int motion_value = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(MOTION_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  motion_value = digitalRead(MOTION_PIN);
  if(motion_value == HIGH){
    digitalWrite(LED_PIN, HIGH); // Turn on LED
    Serial.println("Motion Dectected");
    delay(2000);
  } else {
    digitalWrite(LED_PIN, LOW); // Turn off LED
    Serial.println("Motion Ended");
    delay(1000);
  }
}


int main(void)
{
    /* run the Arduino setup */
    setup();
    /* and the event loop */
    while (1) {
        loop();
    }
    /* never reached */
    return 0;
}
