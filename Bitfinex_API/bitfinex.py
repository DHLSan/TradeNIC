from turtle import end_fill
import bitfinex
import datetime
import time
import pandas as pd

# Create api instance of the v2 API
api_v2 = bitfinex.bitfinex_v2.api_v2()

# Define query parameters
pair = 'BTCUSD' # Currency pair of interest
TIMEFRAME = '1m'#,'4h','1h','15m','1m'

# Define the start date
t_start = datetime.datetime(2022, 6, 14, 0, 0)
t_start = time.mktime(t_start.timetuple()) * 1000

# Define the end date
t_stop = datetime.datetime(2022, 6, 14, 1, 35)
t_stop = time.mktime(t_stop.timetuple()) * 1000

# Download OHCL data from API
result = api_v2.candles(symbol=pair, interval=TIMEFRAME, limit=4000, end=t_stop)

# Convert list of data to pandas dataframe
names = ['Date', 'Open', 'Close', 'High', 'Low', 'Volume']
df = pd.DataFrame(result, columns=names)
df['Date'] = pd.to_datetime(df['Date'], unit='ms')
data  = df.iloc[::-1]
print(data)
data.to_csv("historical.csv")

# we can plot our downloaded data
import matplotlib.pyplot as plt
plt.plot(df['Open'],'-')
plt.show()