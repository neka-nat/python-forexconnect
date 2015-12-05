"""
An example saving historical data to 'test.csv'

The data has 7-days data, and the sampling is 1 minute.
The following data is a content of 'test.csv'.

date,close,high,low,open
2015-11-04 22:26:00,121.272,121.295,121.267,121.291
2015-11-04 22:25:00,121.291,121.298,121.274,121.279
2015-11-04 22:24:00,121.279,121.281,121.27,121.272
2015-11-04 22:23:00,121.272,121.284,121.257,121.28
2015-11-04 22:22:00,121.28,121.28,121.258,121.265
...

"""
import sys
import forexconnect
import login_manager
import datetime
import pandas

if len(sys.argv) < 3:
    print "Usage: python save_historical_data.py instrument(etc. 'EUR/USD') filename"
    sys.exit()

instrument = sys.argv[1]
out_file = sys.argv[2]
username, password, connection = login_manager.get_login_params()
client = forexconnect.ForexConnectClient(username,
                                         password,
                                         connection)
data = client.get_historical_prices(instrument,
                                    datetime.datetime.now() - datetime.timedelta(days = 7),
                                    datetime.datetime.now())
data = [d.__getstate__()[0] for d in data]
df = pandas.DataFrame.from_records(data, index = "date")
df.to_csv(out_file)
