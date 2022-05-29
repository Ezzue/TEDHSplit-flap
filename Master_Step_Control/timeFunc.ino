void getTime()                                  // Funktionen som kalder og modtager RTC-modulets tids data
{

  unsigned long curMilTime = millis();          // Gemmer millis i curMilTime
  if (curMilTime - prvMilTime >= intervalTime)  // Hvis curMilTime - prvMilTime er større end eller ligmed intervalTime(1000), gå ind i statement  
  {
    prvMilTime = curMilTime;                    // Opdatere/gemmer curMilTime
    flagTime = true;                            // Sætter tids-flaget til true, så den kan påbegynde næste loop
  }

  while (flagTime)                                             // Mens tids-flaget er true, kør loopet
  {
    DateTime now = rtc.now();                                  // Fryser tiden som den er på nuværende tidspunkt og smid i en buffer
    sprintf(time, " %02hhu:%02hhu", now.hour(), now.minute()); // Oversætter bytes til char
    Serial.println(time);                                      // Debug kald
    showNewData(time);                                         // Skriver den nye besked til showNewData() funktionen
    flagDate = true;                                           // Gør dato-flaget true
    flagTime = false;                                          // Gør tids-flaget false, så der skal gå 1 sekund før den kan gå ind i dette loop igen

  }

}

void getDate()                                                                        // Funktionen som kalder og modtager RTC-modulets dato data
{

  while (flagDate)                                                                    // Kør loopet hvis dato-flaget er true
  {
    DateTime now = rtc.now();                                                         // Fryser tiden som den er på nuværende tidspunkt og smid i en buffer
    sprintf(time, "%02hhu:%02hhu:%02hhu", now.day(), now.month(), now.year() - 2000); // Oversætter bytes til char
    Serial.println(date);                                                             // Debug kald
    showNewData(date);                                                                // Skriver den nye besked til showNewData() funktionen

    flagDate = false;                                                                 // Gør dato-flaget false, så der skal gå 1 sekund før den kan gå ind i dette loop igen
  }

}

void shiftVal()                     // Funktionen som skifter switchcase variablen med et hvis tidsinterval
{

  unsigned long curMil = millis();  // Gemmer millis i curMilTime
  if (curMil - prvMil >= interval)  // Hvis curMil - prvMil er større end eller ligmed interval(30000), gå ind i statement
  {
    prvMil = curMil;                // Opdatere/gemmer curMil
    Serial.println("val++");        // Debug kald
    val++;                          // Pluser variablen med 1
  }

}
