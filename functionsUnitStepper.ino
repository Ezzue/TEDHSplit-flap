// Denne funktion stopper motoren. Dvs. at den ikke laver en 'holde' position ved at sende strøm igennem viklingerne, som den ellers ville gøre.
void motorStop(byte waitTime)
{
  delay(waitTime);
  curRotating = 0;

  lastWindState_1 = digitalRead(Winding_4);
  lastWindState_2 = digitalRead(Winding_3);
  lastWindState_3 = digitalRead(Winding_2);
  lastWindState_4 = digitalRead(Winding_1);

  digitalWrite(Winding_1, LOW);
  digitalWrite(Winding_2, LOW);
  digitalWrite(Winding_3, LOW);
  digitalWrite(Winding_4, LOW);

}

// Gør stepmotorene klar til at rotere
void motorStart()
{

  curRotating = 1;

  digitalWrite(Winding_1, lastWindState_1);
  digitalWrite(Winding_2, lastWindState_2);
  digitalWrite(Winding_3, lastWindState_3);
  digitalWrite(Winding_4, lastWindState_4);

}

// Funktionen til at finde hallsensoren. Hallsensorpinden er trukket høj i setup() processen, og når den finder magneten, vil den interne schmitt-trigger trække den lav.
// Denne funktion finder punktet det sker, og stopper motoren på det rigtige tidspunkt.
void Home (boolean initiateHoming)
{
#ifdef debug
  Blue();
#endif
  UnitStepper.setSpeed(homSpeed);             // Sætter hastigheden til homing hastigheden
  curRotating = 1;                            // Sætter variablen op til at den kan fortæller masterprintet at stepmotoren kører
  boolean magnetFound = false;                // Flaget der fortæller om magneten er fundet eller ej
  while (!magnetFound)                        // Hvis magneten ikke er fundet, forbliv i dette loop
  {
    int curSensorVal = digitalRead(HALLPIN);  // Variablen der opsættes til at læse niveauet på HALLPIN
    int i = 0;                                // Variablen der tæller antal steps
    if (curSensorVal == HIGH && i == 0)       // Gør kun in i if() statement i starten af sit forløb, for at rotere væk fra magneten, skulle den være lige ude foran den i starten
    {
      i = 50;                                 // Antal steps bliver 50, da stepmotoren stepper 50 gange
      UnitStepper.step(50);
    }
    if (curSensorVal == HIGH)                 // Hvis den ikke har fundet magneten, gå ind i dette statement
    {
      UnitStepper.step(1);                    // Step kun én gang frem
    }
    else                                      // Hvis den har fundet magneten, går den forbi forige if() statement og rammer dette else() statement
    {
      magnetFound = true;                     // Sætter variblen til at den har fundet magneten, derved kan den hoppe ud af loppet til sidst
      curLetter = 0;                          // Nulstiller curLetter
      UnitStepper.step(cal);                  // Roter til kalibreret hjem

      if (initiateHoming)                     // Hvis kaldet initiateHoming er true, stopper den motoren da det er i starten af sin optænding fra slukket tilstand, ellers skal den bare rotere til
                                              // newNumber
      {
        motorStop(100);                       // Kort hviletilstand
      }
    }
    if (i > 2 * StepsPerRev)                  // Hvis den ikke har fundet magneten efter to omgange, vil den melde fejl
    {
      magnetFound = true;
      shownLetter = 0;
      missedStep = 0;
      motorStop(100);
#ifdef debug
      Red();
#endif
      return -1;
    }
    i++;                                      // i + 1, da stepmotoren rotere ét step frem
  }
}


void goToLetter (int newLetter)               // Funktionen som for motoren til at rotere til de korrekte tegn
{

  int letterPosition = -1;                    // letterPosition og curLetterPosition bliver kaldt -1, da hvis det tal den modtager fra I2C ikke giver det rigtige, kan den ikke køre
                                              // videre i sit kommende forløb. Derved kan man undgå volapük
  letterPosition = newLetter;                 // Gør letterPosition til integer som newLetter bliv defineret i funktionskaldet
  int curLetterPosition = -1;
  curLetterPosition = shownLetter;            // Gemmer nuværende tegn i curLetterPosition til samligningen længere nede
  if (newLetter > -1)
  {
    if (letterPosition >= curLetterPosition)  // Hvis det nye tal har en højere eller ligmed værdi med det nuværende, går den ind i dette loop.
                                              // Hvis den går ind i dette statement, vil den kun rotere til næste tegn i forløbet. Skulle det næverende
                                              // tegn være f.eks. 2 og det nye skulle være 9, skal den kun steppe det antal svarende til at kunne
                                              // rotere til 9. Tegnet 9 er nr. 39 i arrayet og i flap-rækkefølgen, og 2 er 32, 39 - 32 => 7*(2048/45)=318,6 steps.
                                              // Sådan undgår vi den skal rotere en hel omgang igen, for at finde det rette tegn.
    {
      int differencePosition = letterPosition - curLetterPosition;          // Her bliver de to variabler trukket fra hinanden
      curRotating = 1;                                                      // Den kan fortælle masterprintet at den rotere
      UnitStepper.setSpeed(normSpeed);                                      // Normal hastighed
      for (int i = 0; i < differencePosition; i++)                          // Looper det antal gange svarende til differencen mellem variablerne
      {
        float presiceStep = float(StepsPerRev) / float(flapsTot);           // Definere en variable som tager decimaler med i udregningen, som er ligmed divideringen af antal steps per omgang
                                                                            // og antal flaps i tromlen
        int roundedStep = int(presiceStep);                                 // Fordi stepmotoren ikke kan rotere med decimaler, f.eks. 0,1, holder missedStep øje med at hvis den over siger 1
                                                                            // f.eks. 0,5 + 0,6 = 1,1, kompensere den med give roundedSteps det overskydende. Dette løber op i sidste ende!
        missedStep = missedStep + (float(presiceStep) - int(roundedStep));
        if (missedStep > 1)
        {
          roundedStep++;
          missedStep--;
        }
#ifdef debug
        Green();
#endif
        UnitStepper.step(roundedStep);        // Giver stepmotoren besked om at steppe det udregnede
      }

    }
    else                                                                    // Hvis det så ikke kan lade sig give sig at steppe til det næste tegn uden en tur forbi
                                                                            // magneten (Home), bliver dette else() statement brugt
    {
      Home(false);                                                          // Der bliver kaldt på funktionen Home(), som finder magneten og da giver µC besked om, hvor den er
      UnitStepper.setSpeed(normSpeed);                                      // Sætter normal hastighed
      for (int i = 0; i < letterPosition; i++)                              // Udfra hjemmepositionen, looper den antalet svarende til det nye tegn
      {
        float presiceStep = float(StepsPerRev) / float(flapsTot);           // Samme forløb som førnævnt -=-
        int roundedStep = int(presiceStep);
        missedStep = missedStep + (float(presiceStep) - int(roundedStep));
        if (missedStep > 1)
        {
          roundedStep++;
          missedStep--;
        }
#ifdef debug
        Purple();
#endif
        UnitStepper.step(roundedStep);
      }
    }
    Serial.println(letterPosition);
    Serial.println(curLetterPosition);
    shownLetter = newLetter;                                                // Opdatere/gemmer det nye tegn til at være nuværende tegn
    motorStop(100);
  }
}

int getAddress()                                                            // Funktionen der laver highstate eller lowstate om til bin og efterfølgende til et dec
{
  int address = !digitalRead(AddressSW_0) + (!digitalRead(AddressSW_1) * 2) + (!digitalRead(AddressSW_2) * 4) + (!digitalRead(AddressSW_3) * 8); // Ved at tage det læste niveau, hvor HIGH er 1 og LOW er 0
                                                                                                                                                 // kan det ganges med den rækkefølge switchene står i
                                                                                                                                                 // og fordi binær er 2-tal system, ganges hvor plads med x+2
  return address;
}

void incomingLetter(byte bytes)           // Funktionen der definere hvad der skal ske når den læser indkommende bytes og pakker dem ud
{
  int incomingArray[2];                   // Initiere et array, hvor bytes bliver lagt ind                          
  for (int i = 0; i < bytes; i++)         // Looper de antal svarende til de indkommende bytes er i alt
  {
    incomingArray[i] = Wire.read();       // Indskriver bytes ind i arrayet
  }
  incomingNumber = incomingArray[0];      // Lagre det byte der fortæller hvad den nye position skal være
  setSpeedM = incomingArray[1];           // Lagre det byte der fortæller hvor hurtigt de skal køre
}

void statusToMaster ()                    // Funktionen der fortæller masterprintet om den rotere eller ej, når master spørger om det

{
  Wire.write(curRotating);                // Skriver hvad variablen der håndtere om den kører eller ej, er
}
