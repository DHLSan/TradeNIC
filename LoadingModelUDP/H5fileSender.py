from re import L
import numpy as np
import h5py
import socket
import time
from datetime import datetime

client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
adr = ("192.168.1.10", 9995)

hf = h5py.File('14062022-150953-e2.h5', 'r')
print(hf.keys())
#send dense bias from model 
print("#########Dense Bias###############")
bias = hf.get('model_weights/dense/dense/bias:0')
bias = np.array(bias)
Bias_string = str(bias[0])
Bias_byte = str.encode(Bias_string)
client_socket.sendto(Bias_byte, adr)
print(bias[0])
time.sleep(2)

#send lstm bias from model
print("#########LSTM Bias ###############")
LstmBias = hf.get('model_weights/lstm/lstm/lstm_cell/bias:0')
LstmBias = np.array(LstmBias)
for i in range(0,400):
    LstmBias_string = str(LstmBias[i])
    LstmBias_byte = str.encode(LstmBias_string)
    client_socket.sendto(LstmBias_byte, adr)
    print(LstmBias[i])
time.sleep(2)

#send dense kernel from model
print("#########Dense Kernel ###############")
kernel = hf.get('model_weights/dense/dense/kernel:0')
kernel = np.array(kernel)
for i in range(0,100):
    kernel_string = str(kernel[i][0])
    kernel_byte = str.encode(kernel_string)
    client_socket.sendto(kernel_byte, adr)
    print(kernel[i][0])
time.sleep(2)


#send lstm kernel from model
print("#########LSTM Kernel ###############")
LstmKernel = hf.get('model_weights/lstm/lstm/lstm_cell/kernel:0')
LstmKernel = np.array(LstmKernel)
for i in range(0,2):
    print("second lstm kernel itteration")
    for j in range(0,400):
        LstmKernel_string = str(LstmKernel[i][j])
        LstmKernel_byte = str.encode(LstmKernel_string)
        client_socket.sendto(LstmKernel_byte, adr)
        print(LstmKernel[i][j])
time.sleep(2)


#send lstm reccurent_kernel from model
print("#########LSTM Reccurent_Kernel ###############")
LstmRecu = hf.get('model_weights/lstm/lstm/lstm_cell/recurrent_kernel:0')
LstmRecu  = np.array(LstmRecu)
for i in range(0,100):
    print(" recurrent kernel  itteration")
    #time.sleep(0.2)
    for j in range(0,400):
        LstmRecu_string = str(LstmRecu[i][j])
        LstmRecu_byte = str.encode(LstmRecu_string)
        client_socket.sendto(LstmRecu_byte, adr)
        print(LstmRecu[i][j])
      #  time.sleep(0.01)


