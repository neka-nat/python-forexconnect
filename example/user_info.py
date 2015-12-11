"""
An example getting user infomation.
"""
import sys
import forexconnect
import lib.login_manager as lm

username, password, connection = lm.get_login_params()
try:
    client = forexconnect.ForexConnectClient(username,
                                            password,
                                            connection)
except:
    lm.clear_cache()
    sys.exit()
print "Balance:", client.get_balance()
print "UsedMargin:", client.get_used_margin()
trades = client.get_trades()
for t in trades:
    print t
client.logout()
