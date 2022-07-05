from turtle import end_fill
import bitfinex
import datetime
import time
import pandas as pd
import socket
# Create api instance of the v2 API
api_v2 = bitfinex.bitfinex_v2.api_v2()
client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
unity_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
adr = ("192.168.1.10", 9999)
adr2 = ("127.0.0.1", 12345)

# Define query parameters
pair = 'BTCUSD' # Currency pair of interest
TIMEFRAME = '1m'#,'4h','1h','15m','1m'

# Define the start date
t_start = datetime.datetime(2022, 6, 14, 0, 0)
t_start = time.mktime(t_start.timetuple()) * 1000

# Define the end date
t_stop = datetime.datetime(2022, 6, 14, 1, 35)
t_stop = time.mktime(t_stop.timetuple()) * 1000
result = api_v2.candles(symbol=pair, interval=TIMEFRAME, limit=50)
# Convert list of data to pandas dataframe
names = ['Date', 'Open', 'Close', 'High', 'Low', 'Volume']
df = pd.DataFrame(result, columns=names)
df['Date'] = pd.to_datetime(df['Date'], unit='ms')+pd.Timedelta('03:00:00')
data  = df
print(data)
for i in range(0,48):
    x_test_string = str(float(data.Close[i]))
    test_val_byte = str.encode(x_test_string)
    client_socket.sendto(test_val_byte, adr)
    unity_socket.sendto(test_val_byte, adr2)
    time.sleep(0.4)
    print(float(data.Close[i]))
    x_test_string = str(float(data.Open[i]))
    test_val_byte = str.encode(x_test_string)
    client_socket.sendto(test_val_byte, adr)
   
   # print(float(data.Open[i]))

#data.to_csv("historical.csv")
while(1):
    time.sleep(60)
    # Download OHCL data from API
    result = api_v2.candles(symbol=pair, interval=TIMEFRAME, limit=1)
    # Convert list of data to pandas dataframe
    names = ['Date', 'Open', 'Close', 'High', 'Low', 'Volume']
    df = pd.DataFrame(result, columns=names)
    df['Date'] = pd.to_datetime(df['Date'], unit='ms')+pd.Timedelta('03:00:00')
    data  = df.iloc[::-1]
    
    x_test_string = str(float(data.Close.iloc[-1]))
    test_val_byte = str.encode(x_test_string)
    client_socket.sendto(test_val_byte, adr)
    client_socket.sendto(test_val_byte, adr2)
    #print(float(data.Close.iloc[-1]) )

    x_test_string = str(float(data.Open.iloc[-1]))
    test_val_byte = str.encode(x_test_string)
    client_socket.sendto(test_val_byte, adr)
    #print(float(data.Open.iloc[-1]))
   
    #print("Open: ",float(data.Open.iloc[-1]))
    #print("close: ",float(data.Close.iloc[-1]))
    print(data)
    #data.to_csv("historical.csv")
 

# we can plot our downloaded data
import matplotlib.pyplot as plt
plt.plot(df['Close'],'-')
plt.show()
