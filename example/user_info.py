"""
An example getting user infomation.
"""
import forexconnect
import login_manager

username, password, connection = login_manager.get_login_params()
client = forexconnect.ForexConnectClient(username,
                                         password,
                                         connection)
print "Balance:", client.get_balance()
print "UsedMargin:", client.get_used_margin()
trades = client.get_trades()
for t in trades:
    print t
client.logout()
