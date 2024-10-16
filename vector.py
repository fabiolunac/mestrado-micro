import serial
import numpy as np
import matplotlib.pyplot as plt

porta_serial = serial.Serial('COM5', baudrate= 115200, timeout= 1)

def send_float_vector(serialobj: serial.Serial, vector: np.ndarray) -> None:
    for value in vector:
        #Converte cada float para 4 bytes e envia
        serialobj.write(np.float32(value).tobytes())

def receive_float_vector(serialobj: serial.Serial, num_elements: int) -> np.ndarray:
    received_vector = np.zeros(num_elements, dtype= np.float32)

    for i in range(num_elements):
        # Lê os 4 bytes e converte para float32
        data = serialobj.read(4)
        received_vector[i] = np.frombuffer(data, dtype= np.float32)[0]
    
    return received_vector



############### Gerar o sinal ###############
N = 100
Fs = 200
f0 = 60
t = np.linspace(0, 2*np.pi, N)

x = np.sin(2*np.pi*f0*t/Fs)

# Enviar o sinal para o microcontrolador
send_float_vector(porta_serial, x)

#Receber o sinal do microcontrolador
y = receive_float_vector(porta_serial, len(x))


porta_serial.close()

# ---------------------------------------
# Plot do sinal
# ---------------------------------------

plt.figure()
plt.subplot(211)
plt.plot(t, x)
plt.title('Sinal original (x) - Enviado pro Micro')

plt.subplot(212)
plt.plot(t, y)
plt.title('Sinal processado (y) - Recebido do micro')

plt.tight_layout()
plt.show()

