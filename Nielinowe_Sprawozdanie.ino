#include <LiquidCrystal.h>
#define TEMP_0_PIN         13   // Analog Input
#define TEMP_1_PIN         15   // Analog Input
#define TEMP_BED_PIN       14   // Analog Input
#define a -5.30377404285076e-07
#define b 0.000913895174164941
#define c -0.619287460375456
#define d 250.544163050966


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

#define E0_STEP_PIN        26
#define E0_DIR_PIN         28
#define E0_ENABLE_PIN      24
#define E0_CS_PIN          42

#define E1_STEP_PIN        36
#define E1_DIR_PIN         34
#define E1_ENABLE_PIN      30
#define E1_CS_PIN          44

#define X_MIN_PIN           3
#ifndef X_MAX_PIN
#define X_MAX_PIN         2
#endif
#define Y_MIN_PIN          14
#define Y_MAX_PIN          15
#define Z_MIN_PIN          18
#define Z_MAX_PIN          19

String a1, a2;
int i = 0, krok1 = 0, krok2 = 0, krok3 = 0, kroki = 0;
bool kierunek1 = HIGH, kierunek2 = HIGH, kierunek3 = HIGH;
bool kierunek = HIGH;
bool x1 = 1;

bool z1;
bool start = LOW;
bool okr = 0;
bool srodek = 0;
bool y2;

#ifndef MOSFET_D_PIN
#define MOSFET_D_PIN     -1
#endif
#ifndef RAMPS_D8_PIN
#define RAMPS_D8_PIN      8
#endif
#ifndef RAMPS_D9_PIN
#define RAMPS_D9_PIN      9
#endif
#ifndef RAMPS_D10_PIN
#define RAMPS_D10_PIN    10
#endif

#define HEATER_0_PIN       RAMPS_D10_PIN


LiquidCrystal lcd(16, 17, 23, 25, 27, 29);
int temp;
double pomiar;
double x;
int czas = 0;
bool praca = 0;
bool grzej = 0;
bool stop2 = 0;
bool poz0 = 0;
int sr = 0;
bool startup = 0;

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
  
    // ****************************************************************************
    // ************************* WYBOR TRYBU PRACY  ******************************
    // **************************************************************************
    
  Serial.println("Wybierz co chcesz zrobic?(dostepne: grzej, zero, jazda, stop). Po wybraniu funkcji stop mozliwosc sterowania manualnego");
  while (Serial.available()) {
    a2 = Serial.readStringUntil('\n');
    if (a2 == "grzej") {
      grzej = 1;
      praca = 0;
      stop2 = 0;
      okr = 0;
      poz0 = 0;

    }
    if (a2 == "jazda") {
      praca = 1;
      grzej = 0;
      stop2 = 0;
      okr = 0;
      poz0 = 0;
    }
    if (a2 == "stop") {
      stop2 = 1;
      praca = 0;
      grzej = 0;
      okr = 0;
      poz0 = 0;
    }
 
    if (a2 == "zero") {
      poz0 = 1;
      stop2 = 0;
      praca = 0;
      grzej = 0;
      okr = 0;

    }

    if (stop2== 1) {
      start = HIGH;
      Serial.print("Wpisz liczbe krokow (a xkrokow b ykrokow c zkokow):   ");
      a1 = Serial.readStringUntil('\n');
      if (a1 == "praca") stop2 = 0;
      sscanf(a1.c_str(), "a %d b %d c %d", &krok1, &krok2, &krok3);
      Serial.println(krok1);
      Serial.println(krok2);
      Serial.println(krok3);

      kierunek1 = krok1 < 0;
      digitalWrite(X_DIR_PIN, kierunek1);
      krok1 = krok1 > 0 ? krok1 : -krok1;

      kierunek2 = krok2 < 0;
      digitalWrite(Y_DIR_PIN, kierunek2);
      krok2 = krok2 > 0 ? krok2 : -krok2;

      kierunek3 = krok3 < 0;
      digitalWrite(Z_DIR_PIN, kierunek3);
      krok3 = krok3 > 0 ? krok3 : -krok3;

      if (krok1 == 0 && krok2 == 0 && krok3 == 0 && start == HIGH)
      {
        Serial.println("podaj dal");
        start = LOW;

      }

    }

  }

  // ****************************************************************************
    // ********************* URUCHOMIENIE GRZALKI  ******************************
    // **************************************************************************

  while (grzej == 1) {
    digitalWrite(HEATER_0_PIN, HIGH);

    if (czas < 200) {
      delay(10);
      Serial.println("grzanie ");
      Serial.println(czas);
      czas++;
    }
    else
    {
      digitalWrite(HEATER_0_PIN, LOW);
      Serial.println("grzanie skonczone");


      czas = 0;
      praca = 1;
      grzej = 0;
    }


//*********************** Ustawianie pozycji zerowej **************************

    if (poz0 == 1)
    {

      digitalWrite(X_DIR_PIN, !kierunek);
      digitalWrite(Y_DIR_PIN, !kierunek);
      digitalWrite(Z_DIR_PIN, !kierunek);

      digitalWrite(X_STEP_PIN, LOW);
      digitalWrite(Y_STEP_PIN, LOW);
      digitalWrite(Z_STEP_PIN, LOW);
      delayMicroseconds(100);

      x1 = digitalRead(X_MAX_PIN);
      y2 = digitalRead(Y_MAX_PIN);
      z1 = digitalRead(Z_MAX_PIN);

      if (x1 == LOW && kierunek == HIGH)
      {
        digitalWrite(X_STEP_PIN, HIGH);
      }

      if (y2 == LOW && kierunek == HIGH)
      {
        digitalWrite(Y_STEP_PIN, HIGH);
      }

      if (z1 == LOW && kierunek == HIGH)
      {
        digitalWrite(Z_STEP_PIN, HIGH);
      }
   
    // **************************************************************************
    //******************* Silnik Gora/dol po 500 krokow *************************
    // **************************************************************************
    }

    if (praca == 1) {
      if (kroki < 500)
      {
        digitalWrite(X_STEP_PIN, HIGH);

      }

      if (kroki < 500)
      {
        digitalWrite(Y_STEP_PIN, HIGH);

      }

      if (kroki < 500)
      {
        digitalWrite(Z_STEP_PIN, HIGH);

      }
      kroki++;
      delayMicroseconds(100);
      if (kroki == 500)
      {
        Serial.println("\t\t\tgora/dol");
        digitalWrite(X_DIR_PIN, kierunek);
        digitalWrite(Y_DIR_PIN, kierunek);
        digitalWrite(Z_DIR_PIN, kierunek);
        kierunek = !kierunek;
        kroki = 0;

      }
    }
    
    // **************************************************************************
    // ************ sterowanie manualne poprzez wysylanie ***********************
    // ***** wartosci krokow za pomoca a xkrokow b ykrokow c zkrokow ************
    // **************************************************************************
    
    if (praca == 0 && stop2 == 1) {
      grzej = 0;
      if ((x1 == LOW || kierunek1 == LOW) && krok1 > 0)
      {
        digitalWrite(X_STEP_PIN, HIGH);
        krok1--;
      }

      if ((y2 == LOW || kierunek2 == LOW) && krok2 > 0)
      {
        digitalWrite(Y_STEP_PIN, HIGH);
        krok2--;
      }

      if ((z1 == LOW || kierunek3 == LOW) && krok3 > 0)
      {
        digitalWrite(Z_STEP_PIN, HIGH);
        krok3--;
      }
      delayMicroseconds(100);
      digitalWrite(X_STEP_PIN, LOW);
      digitalWrite(Y_STEP_PIN, LOW);
      digitalWrite(Z_STEP_PIN, LOW);
  

    x1 = digitalRead(X_MAX_PIN);
    y2 = digitalRead(Y_MAX_PIN);
    z1 = digitalRead(Z_MAX_PIN);

      
    }
   // **************************************************************************
    // ************ sprawdzanie i wyswietlanie temperatury *********************
    // *************************************************************************
    temp = analogRead(13);
    x = (double) temp;
    pomiar = a * x * x * x + b * x * x + c * x + d;
    lcd.setCursor(0, 0);
    lcd.print(pomiar);

 

  }
}
