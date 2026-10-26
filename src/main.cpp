#include "pins_arduino.h"
#include <Arduino.h>
#include <avr/iotn85.h>
#include <avr/interrupt.h>

int led=PIN_B1;
uint8_t lvl = 1;

void setupTimer0() {
  cli(); // 1. Disable global interrupts during setup

  TCCR0A = 0x00;                  // normal mode counter
  TCCR0B = 0x00;
  TCCR0B |= (1<<CS00)|(1<<CS02);  // prescaler set to 1024
  TCNT0 = 0;                      // reset counter

  sei(); // 7. Re-enable global interrupts
}

// timer0 ovf ISR
ISR(TIMER0_OVF_vect)
{
  // this is your event
  lvl ^= 1;
  digitalWrite(led, lvl);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  setupTimer0();
}

void loop() {
  // put your main code here, to run repeatedly:
  // digitalWrite(led, HIGH);
  // delay(200);
  // digitalWrite(led, LOW);
  // delay(200);

}

