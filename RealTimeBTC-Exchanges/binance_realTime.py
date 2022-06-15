from sqlalchemy import true
import websocket, json
import pandas as pd 
import dateutil.parser
import datetime
from datetime import date, datetime, timedelta
import numpy as np
from script.data_processor2 import DataLoader
import socket

minutes_processed = {}
minute_candlesticks = []
current_tick = None
previous_tick = None
count = 0

socket2 = 'wss://ws-feed.pro.coinbase.com'
"""
data = DataLoader(
        "bitcoin_data_tut22.csv",
       	0,
        ["close","Volume"]
    )


x_test , y_test = data.get_test_data(
	seq_len=3,
	normalise=true
)
print(x_test[0][1])
for i in range(len(x_test)):
        np.savetxt("C:/Users/kerem/Desktop/LSTM_Stock_market_data/test_data/x_test" + str(i) + ".csv",x_test[i], fmt='%.10f')
"""  


def on_open(ws):
	print("Connection is opened")
	subscribe_msg = {
		"type": "subscribe",
		"channels": [
			{
			"name": "ticker",
			"product_ids": [
				"BTC-USD"
				]
			}

		]
	}

	ws.send(json.dumps(subscribe_msg))

def on_message(ws, message):
	global current_tick, previous_tick

	previous_tick = current_tick
	current_tick = json.loads(message)

	tick_datetime_object = dateutil.parser.parse(current_tick['time'])
	timenow = tick_datetime_object + timedelta(hours=3) # gmt +3
	tick_dt = timenow.strftime("%m/%d/%Y %H:%M")


	if not tick_dt in minutes_processed:
		print("This is a new DATA")
		minutes_processed[tick_dt] = True
		global count
		if len(minute_candlesticks) > 0:
			minute_candlesticks[-1]['close'] = previous_tick['price']

		minute_candlesticks.append({
			'Date': tick_dt,
			'Open': current_tick['price'],
			'High': current_tick['price'],
			'Low': current_tick['price'],
			'Volume': current_tick['volume_24h']
			})
		df = pd.DataFrame(minute_candlesticks[:-1])
		df.to_csv("bitcoin_data_tut22.csv")
		print(df)

		data = DataLoader(
        	"bitcoin_data_tut22.csv",
       		0,
        	["close","Open"]
    	)

		x_test , y_test = data.get_test_data(
			seq_len=3,
			normalise=true
		)
		client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
		adr = ("192.168.1.10", 9999)
		print(count)
		for k in range(2):
			x_test_string = str(x_test[count][1][k])
			test_val_byte = str.encode(x_test_string)
			client_socket.sendto(test_val_byte, adr)
			print(x_test[count][1][k])
		count = count + 1
		#print(minute_candlesticks[:-1])

		if len(minute_candlesticks) > 0:
			current_candlestick = minute_candlesticks[-1]
			if current_tick['price'] > current_candlestick['high']:
				current_candlestick['high'] = current_tick['price']
			if current_tick['price'] < current_candlestick['low']:
				current_candlestick['low'] = current_tick['price']
	


ws = websocket.WebSocketApp(socket2, on_open=on_open, on_message=on_message)
ws.run_forever()


