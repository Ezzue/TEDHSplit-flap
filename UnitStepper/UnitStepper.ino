// Biblioteker der bliver brugt i denne kode
#include <Stepper.h>
#include <Wire.h>

// Totale antal steps for at rotere 28BYJ-48 stepmotoren én omgang/revo
int StepsPerRev = 2048;

// Hvis denne er #defined, vil debug processen blive initieret
//#define debug

// Definér stepmotor spole aktiverings ben
#define Winding_1 8
#define Winding_2 9
#define Winding_3 10
#define Winding_4 11

// Definér DIP-switch indgangene
#define AddressSW_0 6
#define AddressSW_1 5
#define AddressSW_2 4
#define AddressSW_3 3

// Definér RGB-LED'en samt hallsensor indgangen
#define R  A2
#define G  A1
#define B  A0
#define HALLPIN 7

// Initierer de fleste variabler brugt i koden
byte homSpeed    =  12;   // Homing hastighed
byte normSpeed   =  12;   // Normal hastighed
byte cal         =  60;  // Unikke og calibrede værdi, alt efter hvad der passer bedst til hvert modul.
byte shownLetter =  0;    // Viste tegn
byte curLetter   =  0;    // Nuværende tegn
byte flapsTot    =  45;   // Totale antal flaps
byte missedStep  =  0;    // Decimal overskudet efter at have afrundet flapsTot delt med StepsPerRev
byte curRotating =  0;    // Om stepmotoren kører på nuværende tidspunkt
byte i2cAddress;          // Variable brugt i funktionen til at definere adressen på modulet
byte incomingNumber;      // Læste bit på I2C-bussen
byte setSpeedM   =  0;    // Variable til at gemme på motorhastigheden fra I2C-bussen

// Array som huser alle tegnene som flapsene repræsenterer
const String letter[] = {" ", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "$", "&", "#", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ":", ".", "?", "!", "-"};

// Gemmer tilstanden stepmotor spole benene har på nuværende tidspunkt til senere brug
boolean lastWindState_1 = false;
boolean lastWindState_2 = false;
boolean lastWindState_3 = false;
boolean lastWindState_4 = false;

// Initierer stepmotor biblioteket
Stepper UnitStepper (StepsPerRev, Winding_4, Winding_2, Winding_3, Winding_1);

// Setup processen
void setup()
{

  // Definér hvad de forskellige ben skal være
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(AddressSW_0, INPUT_PULLUP);
  pinMode(AddressSW_1, INPUT_PULLUP);
  pinMode(AddressSW_2, INPUT_PULLUP);
  pinMode(AddressSW_3, INPUT_PULLUP);
  pinMode(HALLPIN,     INPUT_PULLUP);

  // Initierer i2cAddress til at være hvad funktionen getAddress() har fundet frem til (oversætter fra bin til dec)
  i2cAddress = getAddress();

  // Initierer I2C adressen. I dette tilfælde er den en slave
  Wire.begin(i2cAddress);
  Wire.onReceive(incomingLetter);
  Wire.onRequest(statusToMaster);

  // Kalder funktionen Home(), som egentlig finder hallsensoren og derefter rotere til sit kalibrede hjem. Derefter sætter den hastigheden til normal
  Home(true);
  UnitStepper.setSpeed(normSpeed);

}

// Loop processen
void loop()
{

  // Dette er det eneste den kigger på. Den samligner om det nuværende tegn er det samme som det indkommende tegn. Hvis de er ens, går den ikke ind i if() statement.
  // Men hvis de ikke er ens, går den så ind i if() statement og kalder functionen goToLetter() med det indkommende tegn
  if (shownLetter != incomingNumber)
  {
    goToLetter(incomingNumber);
  }

}
