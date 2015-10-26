import sys
import forexconnect
import getpass
import datetime
import pandas

if len(sys.argv) < 3:
    print "Usage: python save_historical_data.py instrument(etc. 'EUR/USD') filename"
    sys.exit()

instrument = sys.argv[1]
out_file = sys.argv[2]
username = raw_input("username: ")
password = getpass.getpass("password: ")
connection = raw_input("connection: ")
client = forexconnect.ForexConnectClient(username,
                                         password,
                                         connection)
data = client.get_historical_prices(instrument,
                                    datetime.datetime.now() - datetime.timedelta(days = 7),
                                    datetime.datetime.now())
data = [d.__getstate__()[0] for d in data]
df = pandas.DataFrame.from_records(data, index = "date")
df.to_csv(out_file)
