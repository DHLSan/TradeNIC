# TradeNIC Hardware Implemantation
-echo.c file includes all the system on the project.    

-On UDP port 9995 weights and biases loaded through network and initializes the Lstm IP core.   

-On UDP port 9999 normalized test functions recives through network and makes prediction every 49*2 window size .

-In udp_recvBack() function it sends back predicted value over ethernet to 9998 UDP port.

-In udp_recvBack() function both structure is created for both real time and Normalized data frame with 49 sequence. In this project real-time option is not optimized yet.

-For real time option our custom circular buffer implemented to the project.
