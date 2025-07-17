// Monty Hall - Arduino
// Botões: pinos 8,9,10
// LEDs: pinos 11,12,13
// Buzzer: pino 7

const int botoes[3] = {8, 9, 10};
const int leds[3] = {11, 12, 13};
const int buzzer = 7;

void setup() {
  Serial.begin(9600);
  // Configura botões e LEDs
  for (int i = 0; i < 3; i++) {
    pinMode(botoes[i], INPUT_PULLUP);
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
}

void buzzerOn(int ms) {
  digitalWrite(buzzer, HIGH);
  delay(ms);
  digitalWrite(buzzer, LOW);
}

void buzzerPisca(int vezes, int duracao) {
  for (int i = 0; i < vezes; i++) {
    digitalWrite(buzzer, HIGH);
    delay(duracao);
    digitalWrite(buzzer, LOW);
    delay(duracao);
  }
}

void piscarLeds(const int ledsArray[], int tamanho, int msPisca, int vezes) {
  for (int i = 0; i < vezes; i++) {
    for (int j = 0; j < tamanho; j++) {
      digitalWrite(leds[ledsArray[j]], HIGH);
    }
    delay(msPisca);
    for (int j = 0; j < tamanho; j++) {
      digitalWrite(leds[ledsArray[j]], LOW);
    }
    delay(msPisca);
  }
}

void loop() {
  // Verifica botões
  for (int i = 0; i < 3; i++) {
    if (digitalRead(botoes[i]) == LOW) {
      Serial.print("botao ");
      Serial.println(i + 1);
      delay(500);  // debounce simples
      while (digitalRead(botoes[i]) == LOW) {
        // espera soltar o botão
      }
    }
  }

  // Processa comandos serial
  if (Serial.available()) {
    String comando = Serial.readStringUntil('\n');
    comando.trim();
    comando.toLowerCase();

    if (comando.startsWith("led")) {
      int ledNum = comando.charAt(3) - '1';  // led1_on -> índice 0
      if (ledNum >= 0 && ledNum < 3) {
        if (comando.endsWith("_on")) digitalWrite(leds[ledNum], HIGH);
        else if (comando.endsWith("_off")) digitalWrite(leds[ledNum], LOW);
      }
    }
    else if (comando.startsWith("escolha ")) {
      int porta = comando.substring(8).toInt() - 1;
      for (int i = 0; i < 3; i++) digitalWrite(leds[i], LOW);
      if (porta >= 0 && porta < 3) digitalWrite(leds[porta], HIGH);
    }
    else if (comando.startsWith("buzzer_on")) {
      int tempo = 1000;  // padrão 1s
      int espaco = comando.indexOf(' ');
      if (espaco > 0) tempo = comando.substring(espaco + 1).toInt();

      buzzerOn(tempo);
    }
    else if (comando.startsWith("duas_ligadas ")) {
      for (int i = 0; i < 3; i++) digitalWrite(leds[i], LOW);
      int separator = comando.indexOf(',');
      if (separator != -1) {
        int p1 = comando.substring(12, separator).toInt();
        int p2 = comando.substring(separator + 1).toInt();
        if (p1 >= 0 && p1 < 3) digitalWrite(leds[p1], HIGH);
        if (p2 >= 0 && p2 < 3) digitalWrite(leds[p2], HIGH);
      }
    }
    else if (comando.startsWith("piscar ")) {
      int pos1 = comando.indexOf(' ') + 1;
      int pos2 = comando.indexOf(' ', pos1);
      int pos3 = comando.indexOf(' ', pos2 + 1);

      String leds_str = comando.substring(pos1, pos2);
      int msPisca = comando.substring(pos2 + 1, pos3).toInt();
      int vezes = comando.substring(pos3 + 1).toInt();

      int ledsArray[3];
      int count = 0;
      int start = 0;
      for (int i = 0; i <= leds_str.length(); i++) {
        if (i == leds_str.length() || leds_str.charAt(i) == ',') {
          ledsArray[count++] = leds_str.substring(start, i).toInt();
          start = i + 1;
        }
      }
      piscarLeds(ledsArray, count, msPisca, vezes);
    }
    else if (comando == "acertou") {
      buzzerOn(1500);
    }
    else if (comando == "errou") {
      for (int j = 0; j < 5; j++) {
        for (int i = 0; i < 3; i++) digitalWrite(leds[i], HIGH);
        delay(200);
        for (int i = 0; i < 3; i++) digitalWrite(leds[i], LOW);
        delay(200);
      }
      buzzerPisca(2, 500);
    }
  }
}
