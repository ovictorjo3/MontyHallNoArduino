import serial
import time
import random
from gtts import gTTS
import os
import tempfile

porta_serial = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)

def falar(texto):
    print(texto)
    with tempfile.NamedTemporaryFile(delete=True, suffix=".mp3") as fp:
        tts = gTTS(text=texto, lang='pt')
        tts.save(fp.name)
        os.system(f"mpg123 -q {fp.name}")

def enviar(comando):
    porta_serial.write((comando + '\n').encode())

def esperar_botao():
    while True:
        if porta_serial.in_waiting:
            dado = porta_serial.readline().decode(errors='ignore').strip()
            if dado.startswith("botao "):
                return int(dado.split(" ")[1])

falar("Olá! Qual o seu nome?")
nome = input("Digite seu nome: ")
falar(f"Bem-vindo, {nome}! Vamos jogar Monty Hall.")

premiada = random.randint(1, 3)
print(premiada)

# 1. Escolha inicial
falar("Escolha uma porta. Pressione o botão 1, 2 ou 3.")
escolha = esperar_botao()
falar(f"Você escolheu a porta {escolha}.")
enviar(f"escolha {escolha}")

# Buzzer curto e agradável (1.5s)
enviar("buzzer_on 1500")
time.sleep(0.5)

# 2. Abrir porta vazia (não acende LED dela)
portas = [1, 2, 3]
restantes = [p for p in portas if p != escolha and p != premiada]
porta_aberta = random.choice(restantes)

# Acende as duas portas restantes (não a aberta)
outras_duas = [p for p in portas if p != porta_aberta]
enviar(f"duas_ligadas {outras_duas[0]-1},{outras_duas[1]-1}")

falar(f"Abrindo a porta {porta_aberta}. Ela está vazia.")

# 3. Pergunta se quer trocar
falar(f"Para manter sua escolha, aperte o botão da porta {escolha}.")
troca_possivel = [p for p in portas if p != escolha and p != porta_aberta]
falar(f"Para trocar, aperte o botão da porta {troca_possivel[0]}.")

while True:
    decisao = esperar_botao()
    if decisao == escolha:
        escolha_final = escolha
        falar(f"Você manteve a porta {escolha}.")
        break
    elif decisao == troca_possivel[0]:
        escolha_final = troca_possivel[0]
        falar(f"Você trocou para a porta {escolha_final}.")
        break

# 4. Piscar LEDs restantes (excluindo a porta aberta)
piscar_leds = [p-1 for p in portas if p != porta_aberta]
enviar(f"piscar {piscar_leds[0]},{piscar_leds[1]} 500 10")
time.sleep(1)

# 5. Revelar porta premiada
enviar(f"escolha {premiada}")
falar("O prêmio está na porta...")
time.sleep(2)

# 6. Resultado
if escolha_final == premiada:
    falar(f"Parabéns, {nome}! Você ganhou!")
    falar(f"O prêmio estava na porta {premiada}.")
    enviar(f"escolha {premiada}")
    enviar("buzzer_on 1500")
else:
    erradas = [p for p in portas if p != premiada and p != porta_aberta]
    if len(erradas) == 2:
        enviar(f"duas_ligadas {erradas[0]-1},{erradas[1]-1}")
    elif len(erradas) == 1:
        enviar(f"escolha {erradas[0]-1}")
    # se erradas estiver vazia, não acende nada

    falar(f"Que pena, {nome}. O prêmio não estava na porta {escolha_final}.")
    time.sleep(1)
    enviar(f"escolha {premiada}")
    falar(f"O prêmio estava na porta {premiada}.")
    enviar("errou")

# 7. Final: apagar todos os LEDs após 2 segundos
time.sleep(2)
enviar("duas_ligadas -1,-1")

porta_serial.close()

