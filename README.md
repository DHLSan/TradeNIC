# TradeNIC (Team number: xohw22-111)

## Trading-Specific SmartNIC Design For Low Latency and High Throughput Algorithmic Trading
University: Eskisehir Technical University  

Supervisor name: Asst. Prof. İsmail SAN (isan@eskisehir.edu.tr)

Participants: İsmail Hakkı İPEK (hakkipek@outlook.com) - Kerem SARI (keremsar30@outlook.com)  

Board used: ZedBoard Zynq-7000 SoC

Software versions: Vivado 2021.1, Vitis HLS 2021.1, Vitis SDK 2021.1, Python 3.8.10

Video Link : https://www.youtube.com/watch?v=zLlkz4FTuO0

Project Report : [xohw22_111_TradeNIC_Report.pdf](https://github.com/DHLSan/TradeNIC/blob/main/Project_Report/xohw22_111_TradeNIC_Report.pdf)

Last version of the Project [TradeNIC_HW_Vitis2021.1](https://github.com/DHLSan/TradeNIC/tree/main/TradeNIC_HW_Vitis2021.1)

## Project Description

Stock and currency exchange transactions are increasing with the emergence of cryptocurrencies and the high profit rates they provide. The application-specific digital hardware designs increase the advantage with Neural Network and machine learning-based smarter and automatic algorithms instead of conventional trading methods. These algorithms that make pricing forecasting require heavy computation and data transfer should be with minimum delay. In this study, all data transfer is performed on the network base with the SmartNIC (Smart Network Interface Card) approach, and accelerates in hardware the network-based operations on the incoming data on the network adapter. The prediction algorithm is accelerated on the hardware by designing a custom SoC (System on Chip) architecture based on SmarNIC. Data transmission is provided in low latency mode with UDP-based communication with specific ports. Financial data that changes depending on the time axis is predicted in hardware instead of CPU with Recurrent Neural Network (RNN) based Long Short Term Memory (LSTM) algorithm. Intraday historical data is obtained from the online trading platform and the model is trained by applying Batch Normalization. In this way, while the input data is normalized during training, shifting is minimized and the delay between layers is reduced. In this study, which was carried out with the motivation of Hardware / Software co-design, while financial data was processed on the software, hardware acceleration was provided on the SoC. LSTM algorithm was synthesized at RTL level with Vivado/HLS tools and implemented on programmable logic. The time it takes to predict the financial data is considerably reduced compared to the processor and the algorithm is about x5.8 times faster. When the predictions made are analyzed by comparing them with real data, the deviation between true data and predict data is very small and mostly catches the rise-fall points.



# TradeNIC
![alt-text](https://github.com/DHLSan/TradeNIC/blob/main/TradeNIC_UI/TradeNIC.gif)
* Our project has been motivated from [RNN-LSTM-Accelerator](https://github.com/JerrytTom/FPGA-Based-LSTM-Accelerator) 

## Experimental Results
### System Optimization Experiments and Results
New models were trained by changing the number of neurons in the LSTM layer. It was aimed to examine the number of neurons with the least prediction deviation of the model depending on the number of LSTM cells and the resource usage of the model with this experiment.Thus, the efficiency of TradeNIC is increased by creating a model with the number of neurons from which the best values are obtained. A total of 19 models were trained on Python Keras by increasing the number of neurons in the range of 10-190 by ten. 
<img src="https://user-images.githubusercontent.com/98567140/177320879-9ea784a9-2505-492d-aaca-8ad0af92b5bf.png" width="500"/>

* When the number of neurons is small, the deviation rate can be more than 100%. In addition, even though it is not greater than 100%, the deviation rate is very high in large neuron numbers. The deviation rate was lowest when the model was trained with 100 neurons, which is approximately 1.5%. Although the model makes predictions with less than 10% deviation in the use of such 100-130 neuron values, it has created a higher learnable parameter here. As a result, using 100 neurons for maximum throughput makes for TradeNIC to work more optimised and efficiently with higher throughputs.

<img src="https://user-images.githubusercontent.com/98567140/177329848-747e5902-dcb6-441c-8cd7-cb33be44f9a5.png" width="500"/>

* When the experimental results are examined on the graph in Figure above,prediction deviation was at least half the number of neurons when window-size was used, and there was a deviation of approximately 1.5% in this value. Usually, a deviation below 50% is obtained, while the window-size, when the number of neurons rises to 90% or more, the prediction deviation is above this value. As a result of this experiment, it was proved that the optimum value of window-size is half the number of neurons to keep the prediction throughput at maximum.

### Prediction Throughput Test Perform on TradeNIC

<img src="https://user-images.githubusercontent.com/92585550/177376049-3ce6d645-0505-44d1-b248-a1adb4f1cfe9.jpg" width="700"/>


* Result of the experiment, the predicted and real data of the cryptocurrency BTC-USD are given in figure above as normalised. Shown in the orange line are the predictions made with the LSTM hardware accelerator, and in the blue line are the correct historical data. A total of 124 pricing forecasts made at 1-minute intervals are compared on the line graph. As can be seen, the prediction values ​​mostly follow the true data closely. Also, it caught the rising and falling trends of the BTC-USD values. The values ​​are more stable and closer to the real values ​​because of the throughput optimizations which have been designed.

<img src="https://user-images.githubusercontent.com/98567140/177331783-1b59e7ed-87e5-43cd-b906-00227e56c422.png" width="700"/>

* Result of the experiment is shown in the table above. The real and predicted close parameters of BTC-USD are compared in the table. Predicted close values show the data which is prediction of the actual value after 1 minute, and this is also indicated by blue arrows. The deviation rate values are shown as permillage and are often less than 1‰. When the rise and fall intervals of the real values are examined in the table; The green upward arrows which are next to predicted values indicate that BTC-USD will rise after 1 minute, and red downward arrows indicate that it will fall after 1 minute.

### Prediction Latency Optimization Experiments Results on TradeNIC

The entire algorithm spent about 123.3 milliseconds on the ZynQ7 only processor of the SoC to make a prediction by using 50 window-size. After the hardware acceleration was applied, the prediction time accelerated by x5.81 times and decreased to 21.22 milliseconds. The execution time of the dense layer decreased from about 9.5 us to 6.5 us.

<img src="https://user-images.githubusercontent.com/98567140/177332827-ae959235-9184-4624-a01e-7035ca3b07e5.png" width="700"/>





