# RNN-LSTM based Model

* This file contains RNN-LSTM python code written using keras. The original code is taken from [FPGA-Based-LSTM-Accelerator](https://github.com/JerrytTom/FPGA-Based-LSTM-Accelerator). In order to run the code, you should upload your stock exchange dataset and you can create your own model

* After generating the model you can parse it to see Bias, Kernel (Weights), Recurrent Kernel (Hidden Matrix) using our python code [HERE](https://github.com/DHLSan/TradeNIC/blob/main/LoadingModelUDP/H5fileSender.py) or you can download [HDF5](https://www.hdfgroup.org/solutions/hdf5/)

# Step-by-Step Building 
* Download [these](https://github.com/DHLSan/TradeNIC/tree/main/LSTM_Model_Creation) files to your host machine
* Install the specified below Python3 libraries into your machine
* Specify the [config_stock.json](https://github.com/DHLSan/TradeNIC/blob/main/LSTM_Model_Creation/config_stock.json) for the model you want to generate (TradeNIC performs with these configurations)
* You can change the historical data type and configure for your financial data 

## Library Versions
* Keras 2.8.0
* Tesnsorflow 2.8.0
* Numpy 1.22.3
* Pandas 1.4.2
* Matplotlib 3.5.2
