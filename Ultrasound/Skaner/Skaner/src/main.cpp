#include <Arduino.h>
#include <Stepper.h>

#define STEPS_NUMBER 32 // for 28BYJ-48 Stepper motor
#define MAX_STEPS_NUM 2048 //32 STEPS * 64 PULSE/STEP = 2048
#define NUM_STEPS_ON_1_DEG 5.68 // 2048/360 = 5.69
#define BUTTON 12
#define LED 13
#define TRIG 7
#define ECHO 6

Stepper motor(STEPS_NUMBER, 8, 10, 9, 11); //I/O ports on Arduino
bool work = false;

unsigned short r[361];
unsigned short theta[361];


//Funkcje prototypy
void meassureProces(int);
int rMeasurement();
void printToSerial(int);



void setup()
{
Serial.begin (9600);
motor.setSpeed(500); //[rpm]
pinMode(BUTTON, INPUT);
pinMode(TRIG, OUTPUT); //Pin, do którego podłączymy trig jako wyjście
pinMode(ECHO, INPUT); //a echo, jako wejście

}

void loop ()
{
meassureProces(200);
delay(5);
}



// FUNKCJE

/*
Funkcja, która konwertuję podaną przez użytkownika wartość kąta w stopniach na liczbę kroków na stopień
*/
int deg2num(int deg)
{
  return round(NUM_STEPS_ON_1_DEG*deg);
}
/*
Proces pomiaru, wyzwalany przyciskiem, zmienna work kontroluję pracę skanera, dioda sygnalizuję obrót
Skaner wykonuje pomiar co krok, dane są zapisywane do tabel r i theta (układ biegunowy)
Wyświetlane są w Serial Monitorze, później karta SD
Po procedurze pomiaru następuje odkręcenie skanera
*/
void meassureProces(int maxDeg)
{
  unsigned short meassureTime = 5;
  int minStep = 1;
  if(digitalRead(BUTTON) == HIGH)
    work = true;
  if(work){
    digitalWrite(LED,HIGH);
    for(int i=0;i<=maxDeg;i=i+minStep){
          r[i] = rMeasurement();
          theta[i] = i;
          motor.step(deg2num(minStep));
          printToSerial(i);
          delay(meassureTime);
    }
    work = false;
    delay(400);
    motor.step(deg2num(-1*maxDeg));
  }
  digitalWrite(LED,LOW);
}
/*
Pomiar odległosci w cm metodą echa
*/
int rMeasurement() {
  long time, radius;
 
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
 
  time = pulseIn(ECHO, HIGH);
  radius = time / 58;
 
  return radius;
}
/*
Wyświetlanie w Monitorze Serialu układu (theta,r)
*/
void printToSerial(int i){
          Serial.print(theta[i]);
          Serial.print("  ");
          Serial.print(r[i]);
          Serial.println();
}