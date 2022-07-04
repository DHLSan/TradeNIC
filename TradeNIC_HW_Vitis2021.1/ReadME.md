# Step-by-step Building 

Creating core IP with HLS:
* Open Vitis HLS 2021.1
* Create New Project
* Downland the [LSTM.cpp](https://github.com/DHLSan/TradeNIC/blob/main/LSTM_HLS_VitisHls2021.1/LSTM.cpp) file and import to sources
* Click C synthesis.
* When synthesis is finished, click Export RTL.
* Do same operations with [dense.cpp](https://github.com/DHLSan/TradeNIC/blob/main/LSTM_HLS_VitisHls2021.1/dense.cpp)

Creating SoC Design with All Components:
* Open Vivado 2021.1
* Create project
* Choose Zedboard (Zynq-7000 SoC) as destination board
* Create block design
* Add ip --> ZynQ7 processing system
* Click Window --> Add ip catalog --> click right click on list --> select your the path of the lstm IP you exported --> click OK
* Click Window --> Add ip catalog --> click right click on list --> select your the path of the dense IP you exported --> click OK
* Add ip --> lstm
* Add ip --> Dense
* Click validate design and generate output products
* Click Generate Bitstream
* Click File --> Export --> Export Hardware
* Click include bitstream and Export.

# TradeNIC System Overall
- echo.c file includes all the system on the project.    

- On UDP port 9995 weights and biases loaded through network and initializes the Lstm IP core this way, it can be implemented to other exchange models easily.   

- On UDP port 9999 normalized test functions recives through network and makes prediction every 49*2 window size .

- In udp_recvBack() function it sends back predicted value over ethernet to desired host on 9998 UDP port.

- In udp_recvBack() function both structure is created for both real time and Normalized data frame with 49 sequence. In this project real-time option is not optimized yet.

- For real time option our custom circular buffer structure implemented to the project.

## TradeNIC Hardware Implemantation

![image](https://user-images.githubusercontent.com/98567140/173881009-ea405000-f291-4ab6-b58a-12e1d92fb73d.png)

## Prediction Results
![image](https://user-images.githubusercontent.com/98567140/173881218-afa94804-0893-4b31-bffa-bc309a371040.png)
