void showNewData(String message)  // Funktionen der kontrollerer om den nye besked er den samme som den forrige. Hvis ikke, går den ind i statement
{

  if (writtenLast != message) {
    showDrawing(message);         // Kalder funktionen showDrawing med kaldet som den nye besked
  }
  writtenLast = message;          // Opdatere/gemmer den nye besked som den nuværende

}

void showDrawing(String message)  // Funktionen som sammensætter beskeden så den kan sendes enkeltvis til de forskellige moduler
{

  message = leftAlign(message);   // Kalder funktionen til at orientere beskeden om til venstre i den totale sammenhæng
  while (unitRunning())           // Kalder funktionen som ser på om modulerne kører på nuværende tidspunkt. Hvis de gør, venter den 500ms til at kontrollere igen
  {
    delay(500);
  }


  for (int i = 0; i < unitTot; i++)         // Looper det antal svarende til den totale mængde moduler(slaver)
  {
    char curLetter = message[i];            // curLetter går ind og bliver til den position svarende til hvad i er på det pågældende tidspunkt
    int curPos = strchr(letter, curLetter); // curPos bliver til en variabel samligner hvad curLetter er i det nuværende loop og går igennem letter[] array og finder den tilsvarende. Hvis den leder efter bogstavet T, vil den returnere med int 20, som er T's position i arrayet
    sendLetter(i + 1, curPos - 7, 12);      // Den sender den nye integer til addressen svarende til i's tal +1, og den ønskede hastigheden
  }

}

String leftAlign(String message)        // Funktionen der sørger for en den venstre position og som udfylder de tomme rum med mellemrum " ". Skulle der stå THEO, er der kun 4 moduler brugt, mens de andre 4 har en udefineret position, men som får et mellemrum tildelt " "
{

  message = upperCaseMessage(message);  // Kalder funktionen som forvandler kaldet fra lille bogstaver til store bogstaver
  char alignString[unitTot + 1];        // Skaber et char array med antal moduler i det hele, + 1 for slut string "\n"
  for (int i = 0; i < unitTot + 1; i++) // Looper antal svarende til antal moduler, 8
  {
    if (i < message.length())           // Hvis i er mindre end kaldets totale længde, gå ind i statement, ellers gå videre forbi
    {
      alignString[i] = message[i];      // Placer kaldets nuværende i og flyt over i det nye array
    }
    else if (i == unitTot)              // Hvis de er ens, gå ind og afslut med en string-afslutning
    {
      alignString[i] = '\0';
    }
    else                                // Ellers udfyld med et mellemrum " "
    {
      alignString[i] = ' ';
    }
  }
  return alignString;                   // Returnér så det nye array

}

String upperCaseMessage(String message) // Funktionen som forvandler kaldet fra lille bogstaver til store bogstaver
{

  message.toUpperCase();                // Bruger arduino funktionen til at udføre arbejdet
  return message;                       // Returnér de store bogstaver

}

void sendLetter(byte address, byte letter, byte Speed)              // Denne funktion sørger for at pakke de to bytes ned i et array, som kan sendes over I2C-bussen
{

  int sendArray[] = {letter, Speed};                                // Skaber et integer array som huser de to bytes (tegn og hastighed)
  Wire.beginTransmission(address);                                  // Påbegynder transmissionen, start bit
  for (int i = 0; i < sizeof sendArray / sizeof sendArray[0]; i++)  // Looper antal gange som arrayets totale størrelse
  {
    Wire.write(sendArray[i]);                                       // Sender arrayet ud på bussen til den kaldede adresse
  }
  Wire.endTransmission();                                           // Afslutter transmissionen, slut bit

}

boolean unitRunning()                         // Funktionen som kontrollere om modulernes motor kører. true = det kører, false = de står stille og er klar til deres I2C-pakke
{

  for (int i = 0; i < unitTot; i++)           // Looper antal gange svarende til antal moduler
  {
    unitState[i] = checkUnit(i);              // Kalder funktionen som spørger ude på bussen, adressen er svarende til talet i
    if (unitState[i] == 1)                    // Hvis der bare er én som returnere 1, returnere den true
    {
      return true;
    }
  }
  return false;                               // Ellers returnere den false

}

int checkUnit(int address)                    // Funktionen som aktivt skriver til hvert modul om deres tilstand
{

  int active;                                 // Skaber den returnerebare variabel
  Wire.requestFrom(address, i2cSize, true);   // Spørger den adressen svarende til kaldet
  active = Wire.read();                       // Den returnerebare variabel bliver det den modtager
  if (active == -1)                           // Hvis bussen ikke er aktiv, går den ind i statement
  {
    Wire.beginTransmission(address);          // Påbegynder transmissionen, start bit
    Wire.endTransmission();                   // Afslutter transmissionen, slut bit
  }
  return active;                              // Returnér active

}
