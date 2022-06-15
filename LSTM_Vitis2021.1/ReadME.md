## LSTM-Zynq-Only
This file contains the Dense and LSTM layer of implementation in C programming language. Under the src folder you can find all the kernel and bias values of sp500.csv file and compile it with using lstm.h script.
Lstm.h header file includes all the LSTM gate calculations under the lstm function. You can implement to any C compiler and try.
The output values you expect to see are normalized values since latency is a huge problem we used batch normalization technique while training with 32 mini-batch size.
