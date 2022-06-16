# TradeNIC (Team number: xohw22-111)

## Trading-Specific SmartNIC Design For Low Latency and High Throughput Algorithmic Trading
University: Eskisehir Technical University  

Supervisor name: Asst. Prof. İsmail SAN (isan@eskisehir.edu.tr)

Participants: İsmail Hakkı İPEK (hakkipek@outlook.com) - Kerem SARI (keremsar30@outlook.com)  

Board used: ZedBoard Zynq-7000 SoC

Software versions: Vivado 2021.1, Vitis HLS 2021.1, Vitis SDK 2021.1

## Project Description

Stock and currency exchange transactions are increasing with the emergence of cryptocurrencies and the high profit rates they provide. Instead of conventional trading methods, application-specific digital hardware designs increase the advantage with Neural Network and machine learning-based smarter and automatic algorithms. These algorithms that make pricing estimation require heavy computation and data transfer should be with minimum delay. In this study, all data transfer is performed on the network base with the SmartNIC (Smart Network Interface Card) approach, and accelerates in hardware the network-based operations on the incoming data on the network adapter. By designing a custom SoC (System on Chip) architecture based on SmarNIC, the prediction algorithm is accelerated on the hardware. Data transmission is provided in low latency mode with UDP-based communication with specific ports. Financial data that changes depending on the time axis is predicted in hardware instead of CPU with Recurrent Neural Network (RNN) based Long Short Term Memory (LSTM) algorithm. Intraday historical data is obtained from the online trading platform and the model is trained by applying Batch Normalization. In this way, while the input data is normalized during training, shifting is minimized and the delay between layers is reduced. In this study, which was carried out with the motivation of Hardware / Software co-design, while financial data was processed on the software, hardware acceleration was provided on the SoC. LSTM algorithm was synthesized at RTL level with Vivado/HLS tools and implemented on programmable logic. The time to predict financial data is considerably reduced compared to the processor. When the predictions made are compared with the real data, the accuracy value is quite high.

# TradeNIC
 <!--  * Our main Goal in this study is to design an low latency system for trade exchanges. One of the powerful method RNN-LSTM model is used for time-series stock predictions  -->
