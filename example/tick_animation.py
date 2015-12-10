"""
An example plotting time series currency.
"""
import sys
import time
import forexconnect
import lib.login_manager as lm
import lib.realtime_chart as rc

if len(sys.argv) < 2:
    print "Usage: python tick_animation.py instrument(etc. 'EUR/USD')"
    sys.exit()

instrument = sys.argv[1]
username, password, connection = lm.get_login_params()
client = forexconnect.ForexConnectClient(username,
                                         password,
                                         connection)
rc.add_data(instrument, [client.get_ask(instrument)] * 100)
rc.init("Real time chart")
while True:
    rc.update_data(instrument, client.get_ask(instrument))
    time.sleep(1)
