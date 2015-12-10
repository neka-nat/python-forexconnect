"""
An example getting user infomation.
"""
import forexconnect
import lib.login_manager as lm

username, password, connection = lm.get_login_params()
client = forexconnect.ForexConnectClient(username,
                                         password,
                                         connection)
print "Balance:", client.get_balance()
print "UsedMargin:", client.get_used_margin()
trades = client.get_trades()
for t in trades:
    print t
client.logout()
