import serial
import time
from gtts import gTTS
import os
import random

def falar(texto):
    tts = gTTS(text=texto, lang='pt-br')
    tts.save("fala.mp3")
    os.system("mpg123 fala.mp3 > /dev/null 2>&1")
    os.remove("fala.mp3")

def acender_led(porta_serial, numero):
    porta_serial.write(numero.encode())
    time.sleep(1)

def piscar_leds(porta_serial):
    porta_serial.write(b'p')  # comando 'p' faz o Arduino piscar todos por 5s
    time.sleep(5.5)

# Início
porta = serial.Serial('/dev/ttyUSB0', 9600)  # ajuste se necessário
time.sleep(2)

nome = input("Qual é o seu nome? ").strip().capitalize()
falar(f"Que comecem os jogos, {nome}!")

portas = ['1', '2', '3']
premio = random.choice(portas)

# Escolha inicial
escolha = ''
while escolha not in portas:
    escolha = input("Escolha uma porta (1, 2 ou 3): ").strip()

acender_led(porta, escolha)
falar(f"Você escolheu a porta {escolha}.")

# Abre uma porta vazia (nem prêmio, nem a escolhida)
portas_vazias = [p for p in portas if p != escolha and p != premio]
porta_aberta = random.choice(portas_vazias)
falar(f"Abrindo uma porta vazia... É a porta {porta_aberta}.")
acender_led(porta, porta_aberta)

# Pergunta se troca ou mantém
falar("Você quer manter sua escolha ou trocar de porta?")
decisao = ''
while decisao not in ['m', 't']:
    decisao = input("Digite 'm' para manter ou 't' para trocar: ").strip().lower()

if decisao == 't':
    escolha_final = [p for p in portas if p != escolha and p != porta_aberta][0]
    falar(f"Você trocou para a porta {escolha_final}.")
else:
    escolha_final = escolha
    falar(f"Você manteve sua escolha na porta {escolha_final}.")

# Suspense: piscar LEDs finais
falar("Suspense... O prêmio está em uma destas portas.")
piscar_leds(porta)

# Revelação final
falar(f"O prêmio está na porta {premio}.")
acender_led(porta, premio)

# Resultado
if escolha_final == premio:
    falar(f"Parabéns, {nome}! Você ganhou!")
else:
    falar(f"Que pena, {nome}. Você perdeu desta vez.")

porta.close()
