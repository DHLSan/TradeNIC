# TradeNIC (Team number: xohw22-111)

## Trading-Specific SmartNIC Design For Low Latency and High Throughput Algorithmic Trading
University: Eskisehir Technical University  

Supervisor name: Asst. Prof. İsmail SAN (isan@eskisehir.edu.tr)

Participants: İsmail Hakkı İPEK (hakkipek@outlook.com) - Kerem SARI (keremsar30@outlook.com)  

Board used: ZedBoard Zynq-7000 SoC

Software versions: Vivado 2021.1, Vitis HLS 2021.1, Vitis SDK 2021.1, Python 3.8.10

## Project Description

Stock and currency exchange transactions are increasing with the emergence of cryptocurrencies and the high profit rates they provide. The application-specific digital hardware designs increase the advantage with Neural Network and machine learning-based smarter and automatic algorithms instead of conventional trading methods. These algorithms that make pricing forecasting require heavy computation and data transfer should be with minimum delay. In this study, all data transfer is performed on the network base with the SmartNIC (Smart Network Interface Card) approach, and accelerates in hardware the network-based operations on the incoming data on the network adapter. The prediction algorithm is accelerated on the hardware by designing a custom SoC (System on Chip) architecture based on SmarNIC. Data transmission is provided in low latency mode with UDP-based communication with specific ports. Financial data that changes depending on the time axis is predicted in hardware instead of CPU with Recurrent Neural Network (RNN) based Long Short Term Memory (LSTM) algorithm. Intraday historical data is obtained from the online trading platform and the model is trained by applying Batch Normalization. In this way, while the input data is normalized during training, shifting is minimized and the delay between layers is reduced. In this study, which was carried out with the motivation of Hardware / Software co-design, while financial data was processed on the software, hardware acceleration was provided on the SoC. LSTM algorithm was synthesized at RTL level with Vivado/HLS tools and implemented on programmable logic. The time it takes to predict the financial data is considerably reduced compared to the processor and the algorithm is about x5.8 times faster. When the predictions made are analyzed by comparing them with real data, the deviation between true data and predict data is very small and mostly catches the rise-fall points.

# TradeNIC
* Our project has been motivated from [RNN-LSTM-Accelerator](https://github.com/JerrytTom/FPGA-Based-LSTM-Accelerator) 

## Experimental Results
New models were trained by changing the number of neurons in the LSTM layer. It was aimed to examine the number of neurons with the least prediction deviation of the model depending on the number of LSTM cells and the resource usage of the model with this experiment.Thus, the efficiency of TradeNIC is increased by creating a model with the number of neurons from which the best values are obtained. A total of 19 models were trained on Python Keras by increasing the number of neurons in the range of 10-190 by ten. 
<img src="https://user-images.githubusercontent.com/98567140/177320879-9ea784a9-2505-492d-aaca-8ad0af92b5bf.png" width="500"/>

When the number of neurons is small, the deviation rate can be more than 100%. In addition, even though it is not greater than 100%, the deviation rate is very high in large neuron numbers. The deviation rate was lowest when the model was trained with 100 neurons, which is approximately 1.5%. Although the model makes predictions with less than 10% deviation in the use of such 100-130 neuron values, it has created a higher learnable parameter here. As a result, using 100 neurons for maximum throughput makes for TradeNIC to work more optimised and efficiently with higher throughputs.

