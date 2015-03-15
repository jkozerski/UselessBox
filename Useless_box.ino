#include <Servo.h>

#define LED 13

#define SWITCH 2

#define RAMIE   10  // pin do sterowania serwem ramienia
#define WIECZKO 11  // pin do sterowania serwem wieczka
#define POWER_T  8  // pin do sterowania tranzystorem (zasilanie serwa)
 
Servo ramie;    // create servo object to control a servo
Servo wieczko;  // create servo object to control a servo 
 
// RAMIĘ
int r_start = 179;  // Pozycja startowa ramienia
int r_close = 114;   // Pozycja tuż przed przełącznikiem
int r_final = 92;   // Pozycja koncowa ramienia

// WIECZKO
int w_start = 90;  // Pozycja startowa wieczka
int w_final = 90;  // Pozycja koncowa wieczka

// Metoda wyłączania
int times_regular = 0;

int tmp = 1;
 
void setup() 
{ 
  tmp = 1;
  pinMode(LED, OUTPUT);
  digitalWrite(LED, false);
  
  ramie.attach(RAMIE);  // attaches the servo on RAMIE to the servo object
  wieczko.attach(WIECZKO);  // attaches the servo on RAMIE to the servo object
  pinMode(POWER_T, OUTPUT);
  pinMode(SWITCH, INPUT_PULLUP);
  
  digitalWrite(POWER_T, true);
  delay(200);
  ramie.write(r_start);
  wieczko.write(w_start);
  delay(1000);
  //digitalWrite(POWER_T, false);
}

boolean is_on()
{
  if (digitalRead(SWITCH)==LOW) {
    delay(20);
    if (digitalRead(SWITCH)==LOW);
      return true;
  }
  return false;  
}

void wait_until_is_on()
{
  while(true) {
    if (digitalRead(SWITCH)==LOW)
      delay(20);
    else {
      delay(20);
      if (digitalRead(SWITCH)==HIGH)
        return;
    }
  }
}

void regular_turn_off()
{
  digitalWrite(POWER_T, true);
  //delay(100);
  ramie.write(r_final);
  wait_until_is_on();
  ramie.write(r_start);
  delay(1000);
  //digitalWrite(POWER_T, false);
}

void stop_turn_off()
{
  digitalWrite(POWER_T, true);
  //delay(100);
  ramie.write(r_close);
  delay(2000 + random(0,4000));
  ramie.write(r_final);
  wait_until_is_on();
  ramie.write(r_start);
  delay(1000);
  //digitalWrite(POWER_T, false);
}

void slow_turn_off()
{
  digitalWrite(POWER_T, true);
  //delay(100);
  
  int slowness = random(10,50); 
  for(int i=r_start; i>r_close; i--) {
    ramie.write(i);
    delay(slowness);
  }
  ramie.write(r_final);
  wait_until_is_on();
  ramie.write(r_start);
  delay(1000);
  digitalWrite(POWER_T, false);
}

void ready_to_next_turn_off()
{
  digitalWrite(POWER_T, true);
  //delay(100);
  ramie.write(r_final);
  wait_until_is_on();
  ramie.write(r_start);
  delay(random(1000,3000));
  ramie.write(r_close);
  delay(500);
  while (true) {
    if (is_on()) {
      regular_turn_off();
      digitalWrite(POWER_T, false);
      return;
    }
  delay(200);
  }
}

void turn_off()
{
  delay(random(1, 5000));
  
//  if (times_regular<3) {
//    times_regular++;
//    regular_turn_off();
//    return;
//  }
  
  if (tmp == 1) {
    regular_turn_off();
    tmp++;
  } else if (tmp == 2) {
    stop_turn_off();
    tmp++;
  } else if (tmp == 3) {
    slow_turn_off();
    tmp++;
  } else {
    ready_to_next_turn_off();
    tmp = 1;
  }
    
  
//  switch (random(1, 4)) {
//    case 1: regular_turn_off(); break;
//    case 2: stop_turn_off(); break;
//    case 3: slow_turn_off(); break;
//    default: regular_turn_off(); break;
//  }
}
 
void loop() 
{ 
  if (is_on())
    turn_off();
  delay(1000);
} 
