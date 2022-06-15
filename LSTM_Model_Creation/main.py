import os
import json
import time
import math
import pandas as pd
import matplotlib.pyplot as plt
from core.data_processor import DataLoader
from core.model import Model
import numpy as np
from sklearn.preprocessing import MinMaxScaler
from sklearn.preprocessing import StandardScaler
import socket
import websocket, json
import dateutil.parser
from datetime import date, datetime, timedelta

scaler = StandardScaler()
def plot_results(predicted_data, true_data):
    fig = plt.figure(facecolor='white')
    ax = fig.add_subplot(111)
    ax.plot(true_data, label='True Data')
    plt.plot(predicted_data, label='Prediction')
    plt.legend()
    plt.show()


def plot_results_multiple(predicted_data, true_data, prediction_len):
    fig = plt.figure(facecolor='white')
    ax = fig.add_subplot(111)
    ax.plot(true_data, label='True Data')
    # Pad the list of predictions to shift it in the graph to it's correct start
    for i, data in enumerate(predicted_data):
        padding = [None for p in range(i * prediction_len)]
        plt.plot(padding + data, label='Prediction')
        plt.legend()
    plt.show()


def main():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    adr = ("192.168.1.10", 9999)

    configs = json.load(open('config_stock.json', 'r'))
    if not os.path.exists(configs['model']['save_dir']): os.makedirs(configs['model']['save_dir'])
    data = DataLoader(
        os.path.join('data', configs['data']['filename']),
        configs['data']['train_test_split'],
        configs['data']['columns']
    )

    
    # train model
    model = Model()
    model.build_model(configs)
    x, y = data.get_train_data(
    	seq_len = configs['data']['sequence_length'],
    	normalise = configs['data']['normalise']
    )
    model.train(
    	x,
    	y,
    	epochs = configs['training']['epochs'],
    	batch_size = configs['training']['batch_size'],
        save_dir = configs['model']['save_dir']
    )

    

    x_test , y_test = data.get_test_data(
        seq_len=configs['data']['sequence_length'],
        normalise=configs['data']['normalise']
    )

    for i in range(124):
        time.sleep(0.1)
        for j in range(1, 49):
            for k in range(2):
                x_test_string = str(x_test[i][j][k])
                test_val_byte = str.encode(x_test_string)
                client_socket.sendto(test_val_byte, adr)
                print(x_test[i][j][k])

    # save test data
    """
    np.set_printoptions(suppress=True)
    np.set_printoptions(precision=10)
    for i in range(len(x_test)):
        np.savetxt("/home/kerem/FPGA-Based-LSTM-Accelerator/LSTMcell/python/testdata/x_test" + str(i) + ".csv",x_test[i], fmt='%.10f')
    np.savetxt("/home/kerem/FPGA-Based-LSTM-Accelerator/LSTMcell/python/testdata/y_test.csv", y_test, fmt='%.10f')
    """



    """
    for i in range(1,28):
        time.sleep(0.1)
        for j in range(1,2):
            for k in range(2):
                x_test_string = str(x_test[i][j][k])
                test_val_byte = str.encode(x_test_string)
                client_socket.sendto(test_val_byte, adr)
                print(x_test[i][j][k])
    # predict and plot
    """
    """
    model = Model()
    model.load_model("./saved_models/14062022-150953-e2.h5")

    # predictions = model.predict_sequences_multiple(x_test, configs['data']['sequence_length'], configs['data']['sequence_length'])
    # predictions = model.predict_sequence_full(x_test, configs['data']['sequence_length'])

    predictions = model.predict_point_by_point(x_test)
    model.model.summary()
    #plot_results_multiple(predictions, y_test, configs['data']['sequence_length'])
    plot_results(predictions, y_test)
    np.savetxt("./result/predictions.csv", predictions, delimiter=',')
    """

    """
    # plot C prediction
    predictions = np.loadtxt("predictions_c.csv")
    print(predictions)
    plot_results(predictions, y_test)
    """


if __name__ == '__main__':
    main()
