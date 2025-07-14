int portas[] = {2, 3, 4};

void setup() {
  for (int i = 0; i < 3; i++) {
    pinMode(portas[i], OUTPUT);
    digitalWrite(portas[i], LOW);
  }
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    char comando = Serial.read();

    // Apaga todos
    digitalWrite(7, LOW);
    digitalWrite(6, LOW);
    digitalWrite(5, LOW);

    if (comando == '1') digitalWrite(7, HIGH);
    else if (comando == '2') digitalWrite(6, HIGH);
    else if (comando == '3') digitalWrite(5, HIGH);
    else if (comando == 'p') {  // Piscar portas 1 e 2
      for (int i = 0; i < 10; i++) {
        digitalWrite(7, HIGH);
        digitalWrite(6, HIGH);
        digitalWrite(5, HIGH);
        delay(250);
        digitalWrite(7, LOW);
        digitalWrite(6, LOW);
        digitalWrite(5, LOW);
        delay(250);
      }
    }
  }
}
