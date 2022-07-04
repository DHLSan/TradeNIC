# Step-by-Step Building 

Creating core IP with HLS:
* Open Vitis HLS 2021.1
* Create New Project
* Downland the [LSTM.cpp](https://github.com/DHLSan/TradeNIC/blob/main/LSTM_HLS_VitisHls2021.1/LSTM.cpp) file and import to sources
* Click C synthesis.
* When synthesis is finished, click Export RTL.
* Do same operations with [dense.cpp](https://github.com/DHLSan/TradeNIC/blob/main/LSTM_HLS_VitisHls2021.1/dense.cpp)

Creating SoC Design with All Components:
* Open Vivado 2021.1.
* Create project.
* Choose Zedboard (Zynq-7000 SoC) as destination board.
* Create block design.
* Add ip --> ZynQ7 processing system.
* Click Window --> Add ip catalog --> click right click on list --> select your the path of the lstm IP you exported --> click OK.
* Click Window --> Add ip catalog --> click right click on list --> select your the path of the dense IP you exported --> click OK.
* Add ip --> lstm.
* Add ip --> Dense.
* Click Adress Editor --> Change lstm_axi_BUS_A master base adress to 0x4010_0000  --> Change range 512 K.
* Click validate design and generate output products.
* Click Generate Bitstream.
* Click File --> Export --> Export Hardware.
* Click include bitstream and Export.

Creating Software application:
* Open Vitis 2021.1, choose workspace for new project
* Create the platform project, select the .xsa file that are created after the export hardware step.
* Create the application project, select created platform project and select a lwIP Echo Server application project to create your project.
* Copy and Replace all of the files [HERE](https://github.com/DHLSan/TradeNIC/tree/main/TradeNIC_HW_Vitis2021.1) to src folder. 
* Ready to run (Dont forget to increase heap and stack size of the project).
* Download [These](https://github.com/DHLSan/TradeNIC/tree/main/LoadingModelUDP) files and send ready model or other models from your host machine to TradeNIC.
* After loading the model, Compile [BITFINEX](https://github.com/DHLSan/TradeNIC/blob/main/Bitfinex_API/bitfinex.py) file and perfom prediction with 1m intervals.



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
