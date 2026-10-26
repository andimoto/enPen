#include "pins_arduino.h"
#include <Arduino.h>
#include <avr/iotn85.h>
#include <avr/interrupt.h>
// #include <cstdint>

// int led=PIN_B1;
// uint8_t lvl = 1;

const int SERVO_PIN = 0;  // PB1 (Physical Pin 6 on DIP-8) - Servo Control Output

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
  // lvl ^= 1;
  // digitalWrite(led, lvl);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(SERVO_PIN, OUTPUT);
  // setupTimer0();

 
}

void loop() {
  static int cnt = 0;
  static uint32_t lowWidth = 0;
  static uint32_t cntDir = 1;

  if(cnt >= 25)
  {
    cnt = 0;
    if(cntDir == 1)
      lowWidth += 100;
    else
      lowWidth -= 100;

    
    if(lowWidth >= 1600 || lowWidth == 0)
    {
      cntDir ^= 1; 
    }
  }

  

  digitalWrite(SERVO_PIN, HIGH);
  delayMicroseconds(700+lowWidth);
  digitalWrite(SERVO_PIN, LOW);
  delayMicroseconds(1600-lowWidth);

  cnt++;
}

