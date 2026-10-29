#include "pins_arduino.h"
#include <Arduino.h>
#include <avr/iotn85.h>
#include <avr/interrupt.h>

// #include <cstdint>

// #define TESTPWM

const uint8_t DUTY_CYCLE_PEN_DOWN = 205;
const uint8_t DUTY_CYCLE_PEN_UP = 170;

int led=PIN_B1;
int readPWM = PIN_B1;

uint8_t lvl = 1;
uint32_t cnt = 0;

// const int SERVO_PIN = 0;  // PB1 (Physical Pin 6 on DIP-8) - Servo Control Output


void setupTimer1() {
  cli(); // 1. Disable global interrupts during setup
  TCCR1 = 0x00;                  // normal mode counter

  TCCR1 |= (1<<CS13)|(1<<CS11)|(1<<CS11);  // prescaler set to 64 -> 1/8MHz * 64 = 40us
  TCNT1 = 0;                      // reset counter

  TIMSK |= (0 << TOIE0);
  TIMSK |= (1 << TOIE1);
  sei(); // 7. Re-enable global interrupts
}

// timer1 ovf ISR
// ISR(TIMER1_OVF_vect)
// {
//   // this is your event

// }

void setupINT()
{
  cli(); // 1. Disable global interrupts during setup
  MCUCR |= (1<<ISC00) | (0<<ISC01); //set to irq on logical change
  GIMSK = 0x00;
  GIMSK |= 1<<PCIE;   // irq enable on logical change
  GIFR = 0x00;
  GIFR |= (1<<PCIF);  // set irq flag on any logical change

  PCMSK = 0x00;
  PCMSK = 1<<PCINT1;  // irq on pin b1

  sei(); // 7. Re-enable global interrupts
}

uint8_t lowTime = 0;
uint8_t highTime = 0;
ISR(INT0_vect)
{
  uint8_t timerVal = TCNT1;
  if(digitalRead(readPWM) == HIGH)
  {
    lowTime = timerVal;
  }else{
    highTime = timerVal; 
  }


  TCNT1 = 0;
}

void setDutyCycle(const uint8_t dutyCycle)
{
  // allow only up and down. no other values
  if(dutyCycle == DUTY_CYCLE_PEN_DOWN || dutyCycle == DUTY_CYCLE_PEN_UP)
  {
    OCR0A = dutyCycle;
  }
}

void pwm_setup()
{
    cli(); // 1. Disable global interrupts during setup

    DDRB |= (1 << PB0);                    // Set PB1 as output
    TCCR0A = 0x00;                         // Normal mode, no PWM
    TCCR0B = 0x00;                         // Timer stopped for now
    
    // 2. Configure TCCR0A for Fast PWM mode & Non-inverting output
    // COM0A1 = 1: Clear OC0A on Compare Match, set OC0A at BOTTOM (non-inverting)
    // WGM01 = 1, WGM00 = 1: Fast PWM mode (Mode 3)
    TCCR0A |= (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);

    TCCR0B |= (1 << CS00) | (1 << CS01);   // Prescaler = 64 @ 8MHz

    // TIMSK |= (1 << TOIE0);                 // Enable Timer0 overflow interrupt

    TCNT0 = 0;                             // Start counting from zero

    setDutyCycle(DUTY_CYCLE_PEN_UP);

    sei();                                 // Enable global interrupts
}



void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  // setupINT();
  // setupTimer1();
  pwm_setup();
  



 
}


void testPWM()
{
  static uint8_t cnt = 0;

  cnt++;
  if(cnt <= 2)
  {
    setDutyCycle(DUTY_CYCLE_PEN_DOWN);
    digitalWrite(led, LOW);
  }else {
    setDutyCycle(DUTY_CYCLE_PEN_UP);
    digitalWrite(led, HIGH);
  }

  if(cnt >= 4)
  {
    cnt = 0;
  }
   
  delay(500);
}

uint8_t lowCnt = 0;
uint8_t highCnt = 0;
  uint8_t oldLvl = 0;
  uint8_t cnt = 0;
void loop() {
#ifdef TESTPWM
  testPWM();
#else
  // digitalWrite(led, HIGH);
  // delay(500);
  // // delayMicroseconds(100);
  // digitalWrite(led, LOW);
  // delay(500);
  // //delayMicroseconds(10);




  while(1)
  {
    // read pwm signal of/for laser
    uint8_t lvl = digitalRead(readPWM);

    // rising edge / new phase
    if(lvl == HIGH && oldLvl == LOW)
    {
      
    }

    if(digitalRead(readPWM) == HIGH)
    {
      highCnt++;
    }else {
      lowCnt++;
    }
    oldLvl = lvl;

    cnt++;



    delayMicroseconds(50);
  }
#endif
}

