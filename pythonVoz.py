import serial
import time
from gtts import gTTS
import os

# Função para falar usando gTTS + mpg123
def falar(texto):
    tts = gTTS(text=texto, lang='pt-br')
    tts.save("fala.mp3")
    os.system("mpg123 fala.mp3 > /dev/null 2>&1")
    os.remove("fala.mp3")

# Porta serial (ajuste conforme necessário)
porta = serial.Serial('/dev/ttyUSB0', 9600)
time.sleep(2)

nome = input("Qual é o seu nome? ").strip().capitalize()
falar(f"Que comecem os jogos, {nome}!")

print("\nBem-vindo(a),{}! \nPressione 1, 2 ou 3 para acender um LED.".format(nome))
print("Aperte 'q' para sair.\n")

while True:
    tecla = input("Escolha (1/2/3): ").strip()

    if tecla in ['1', '2', '3']:
        porta.write(tecla.encode())
        falar(f"Você escolheu a porta {tecla}, {nome}.")
    elif tecla.lower() == 'q':
        falar(f"Encerrando o jogo. Até mais, {nome}.")
        print("Saindo...")
        break
    else:
        print("Entrada inválida. Digite 1, 2, 3 ou 'q' para sair.")

porta.close()

