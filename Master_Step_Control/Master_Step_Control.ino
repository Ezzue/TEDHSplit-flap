// Biblioteker der bliver brugt i denne kode
#include <Wire.h>
#include <RTClib.h>

// Initiere DS3231 Real-Time Clock modulet
RTC_DS3231 rtc;

// Definere variabler
#define i2cSize 1   // Den totale størrelse af antal bytes som kan/skal sendes
#define unitTot 8   // Totale antal moduler/slaver, den skal skrive til
#define flapTot 45  // Antal totale flaps, som hver modul har

#define buttonInput1 5  // Definere pin til den første debug-knap 
#define buttonInput2 6  // Definere pin til den anden debug-knap
int unitState[unitTot]; // Skaber et integer array med modulernes motorstatus

int buttonState1 = 0;     // Nulstiller variablen som 1. knappens position
int buttonState2 = 0;     // Nulstiller variablen som 2. knappens position
int lastButtonState1 = 0; // Nulstiller variablen som 1. knappens forrige position
int lastButtonState2 = 0; // Nulstiller variablen som 2. knappens forrige position

const char letter[] = {" ABCDEFGHIJKLMNOPQRSTUVWXYZ$&#0123456789:.?!-"}; // char array over flappernes rækkefølge

boolean flagTime = true; // Flag brugt til tids-funktionen
boolean flagDate = true; // Flag brugt til dato-funktionen

// StringControl variabler
String writtenLast; // Variabel der gemmer på forrige besked
char time[10]; // char array til timer og minutter
char date[10]; // char array til dage, måneder og år
byte val = 1;  // switchcase variable

// Tids variabler
unsigned long prvMilTime    = 0;      // Gemmer på forrige antal millis() for tids-funktionen
unsigned long prvMil        = 0;      // Gemmer på forrige antal millis()
unsigned long intervalTime  = 1000;   // Faste interval mellem skift for tids-funktionen
unsigned long interval      = 30000;  // Faste interval mellem skift

// Setup process
void setup()
{

  // Sætter I2C op som værende i master-tilstand
  Wire.begin();
  // Begynder serial (til debugging)
  Serial.begin(9600);
  // Begynder RTC-modulet
  rtc.begin();

  // Initerer knapperne til at være inputs
  pinMode(buttonInput1, INPUT);
  pinMode(buttonInput2, INPUT);

}

// Loop processen
void loop()
{
  
  // Funktionen bliver kaldt for at hente tiden fra RTC-modulet
  getTime();

}
