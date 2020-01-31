#include <LiquidCrystal.h>
#define TEMP_0_PIN         13   // Analog Input
#define TEMP_1_PIN         15   // Analog Input
#define TEMP_BED_PIN       14   // Analog Input
#define a  5.30377404285076e-07
#define b  0.000913895174164941
#define c 0.619287460375456
#define d  250.544163050966
#include <stdlib.h>
#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#define X_CS_PIN           53
#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#define Y_CS_PIN           49
#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
#define Z_CS_PIN           40
#define X_MIN_PIN           3
#define X_MAX_PIN         2
#define Y_MIN_PIN          14
#define Y_MAX_PIN          15
#define Z_MIN_PIN          18
#define Z_MAX_PIN          19
#define HEATER_0_PIN     10

LiquidCrystal lcd(16, 17, 23, 25, 27, 29);
String czynnosc;
String ilekrokow;
int wybtemp=200;
int temp;
int licznik1 = 0;
int zmiennatemp=1;
double x,y,aa,bb,cc,i;
void setup()
{
  lcd.begin(20, 4);
  pinMode(HEATER_0_PIN, OUTPUT);
  Serial.begin(115200);
  pinMode(X_ENABLE_PIN, OUTPUT);
  digitalWrite(X_ENABLE_PIN, LOW);
  pinMode(X_DIR_PIN, OUTPUT);
  digitalWrite(X_DIR_PIN, LOW);
  pinMode(X_STEP_PIN, OUTPUT);
  digitalWrite(X_STEP_PIN, HIGH);

  pinMode(Y_ENABLE_PIN, OUTPUT);
  digitalWrite(Y_ENABLE_PIN, LOW);
  pinMode(Y_DIR_PIN, OUTPUT);
  digitalWrite(Y_DIR_PIN, LOW);
  pinMode(Y_STEP_PIN, OUTPUT);
  digitalWrite(Y_STEP_PIN, HIGH);

  pinMode(Z_ENABLE_PIN, OUTPUT);
  digitalWrite(Z_ENABLE_PIN, LOW);
  pinMode(Z_DIR_PIN, OUTPUT);
  digitalWrite(Z_DIR_PIN, LOW);
  pinMode(Z_STEP_PIN, OUTPUT);
  digitalWrite(Z_STEP_PIN, HIGH);

  pinMode(X_MAX_PIN, INPUT_PULLUP);
  pinMode(Y_MAX_PIN, INPUT_PULLUP);
  pinMode(Z_MAX_PIN, INPUT_PULLUP);
  
 

}


void loop()
{
  Serial.println("Wybierz co chcesz zrobic?(dostepne: grzej, zero, temp)");
  while (Serial.available()){
      czynnosc = Serial.readStringUntil('\n');
  }
  
  
  
  if(czynnosc="grzej"){
  
  
    Serial.println("Do jakiej temperatury?");
    
      wybtemp = Serial.readStringUntil('\n').toInt();
    
    digitalWrite(HEATER_0_PIN, HIGH);
    
    Serial.println("Rozpoczynam grzanie");
    while(wybtemp>y){
    temp = analogRead(TEMP_0_PIN);
     lcd.setCursor(0,0);
      x = double(temp);
     y = aa*x*x*x+bb*x*x+cc*x+d;
     lcd.print(y);
    digitalWrite(HEATER_0_PIN, LOW);
    delay(0.1);
      }
   Serial.println("grzanie zakonczone");
  
  }
  
  while(czynnosc="zero")
  {
  
  
  aa = digitalRead(X_MAX_PIN);
  bb = digitalRead(Y_MAX_PIN);
  cc = digitalRead(Z_MAX_PIN);
  
    if (aa == LOW)
    {
      digitalWrite(X_STEP_PIN, HIGH);
    }
  
    if (bb == LOW)
    {
      digitalWrite(Y_STEP_PIN, HIGH);
    }
  
    if (cc == LOW)
    {
      digitalWrite(Z_STEP_PIN, HIGH);
    }
   delay(50);
  if (aa == HIGH && bb == HIGH && cc == HIGH){
    Serial.println("Ustawiono pozycje zerowa");
    digitalWrite(X_STEP_PIN, LOW);
    digitalWrite(Y_STEP_PIN, LOW);
    digitalWrite(Z_STEP_PIN, LOW);
    
  }
  
  
  }
  
  
  if(czynnosc="temp"){
  
    temp = analogRead(TEMP_0_PIN);
     lcd.setCursor(0,0);
     x = double(temp);
     y = a*x*x*x+b*x*x+c*x+d;
     lcd.print(y);
  
    delay(400);
  }


}
