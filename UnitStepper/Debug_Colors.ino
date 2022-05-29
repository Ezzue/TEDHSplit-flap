void Red()                // Tænder det røde lys
{
  digitalWrite(R, HIGH);
  digitalWrite(G, LOW);
  digitalWrite(B, LOW);
}

void Green()              // Tænder det grønne lys
{
  digitalWrite(R, LOW);
  digitalWrite(G, HIGH);
  digitalWrite(B, LOW);
}

void Blue()               // Tænder det blå lys
{
  digitalWrite(R, LOW);
  digitalWrite(G, LOW);
  digitalWrite(B, HIGH);
}

void Purple()             // Tænder det røde og blå lys, som danner et lilla lys
{
  digitalWrite(R, HIGH);
  digitalWrite(G, LOW);
  digitalWrite(B, HIGH);
}
