const int botoes[3] = {8, 9, 10};
const int leds[3] = {11, 12, 13};
const int buzzer = 7;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 3; i++) {
    pinMode(botoes[i], INPUT_PULLUP);
    pinMode(leds[i], OUTPUT);
  }
  pinMode(buzzer, OUTPUT);
  for (int i = 0; i < 3; i++) digitalWrite(leds[i], LOW);
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

void piscarLeds(int ledsArray[], int tamanho, int msPisca, int vezes) {
  for (int i = 0; i < vezes; i++) {
    for (int j = 0; j < tamanho; j++) digitalWrite(leds[ledsArray[j]], HIGH);
    delay(msPisca);
    for (int j = 0; j < tamanho; j++) digitalWrite(leds[ledsArray[j]], LOW);
    delay(msPisca);
  }
}

void loop() {
  for (int i = 0; i < 3; i++) {
    if (digitalRead(botoes[i]) == LOW) {
      Serial.print("botao ");
      Serial.println(i + 1);
      delay(500); // debounce
      while (digitalRead(botoes[i]) == LOW);
    }
  }

  if (Serial.available()) {
    String comando = Serial.readStringUntil('\n');
    comando.trim();

    if (comando.startsWith("escolha ")) {
      int porta = comando.substring(8).toInt() - 1;
      for (int i = 0; i < 3; i++) digitalWrite(leds[i], LOW);
      if (porta >= 0 && porta < 3)
        digitalWrite(leds[porta], HIGH);
    }

    else if (comando.startsWith("buzzer_on ")) {
      int tempo = comando.substring(9).toInt();
      buzzerOn(tempo);
    }

    else if (comando.startsWith("abrir ")) {
      int porta = comando.substring(6).toInt() - 1;
      if (porta >= 0 && porta < 3)
        digitalWrite(leds[porta], HIGH);
      buzzerOn(500); // som ao abrir a porta
    }

    else if (comando.startsWith("duas_ligadas ")) {
      for (int i = 0; i < 3; i++) digitalWrite(leds[i], LOW);
      int separator = comando.indexOf(',');
      if (separator != -1) {
        int p1 = comando.substring(12, separator).toInt();
        int p2 = comando.substring(separator + 1).toInt();
        if (p1 >= 0 && p2 >= 0) {
          digitalWrite(leds[p1], HIGH);
          digitalWrite(leds[p2], HIGH);
        }
        // se p1 ou p2 for -1, mantém tudo apagado
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
      // Não apaga LEDs — o Python já deixou só o LED da premiada ligado
      buzzerOn(1500); // som curto de vitória
    }

    else if (comando == "errou") {
      // Pisca todos os LEDs por 5 vezes
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

