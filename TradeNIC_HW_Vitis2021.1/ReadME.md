# TradeNIC System Overall
-echo.c file includes all the system on the project.    

-On UDP port 9995 weights and biases loaded through network and initializes the Lstm IP core.   

-On UDP port 9999 normalized test functions recives through network and makes prediction every 49*2 window size .

-In udp_recvBack() function it sends back predicted value over ethernet to 9998 UDP port.

-In udp_recvBack() function both structure is created for both real time and Normalized data frame with 49 sequence. In this project real-time option is not optimized yet.

-For real time option our custom circular buffer implemented to the project.

## TradeNIC Hardware Implemantation

![image](https://user-images.githubusercontent.com/98567140/173881009-ea405000-f291-4ab6-b58a-12e1d92fb73d.png)

## Prediction Results
![image](https://user-images.githubusercontent.com/98567140/173881218-afa94804-0893-4b31-bffa-bc309a371040.png)
