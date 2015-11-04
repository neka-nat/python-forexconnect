"""
An example getting user infomation.
"""
import forexconnect
import getpass

username = raw_input("username: ")
password = getpass.getpass("password: ")
connection = raw_input("connection: ")
client = forexconnect.ForexConnectClient(username,
                                         password,
                                         connection)
print "Balance:", client.get_balance()
print "UsedMargin:", client.get_used_margin()
trades = client.get_trades()
for t in trades:
    print t
client.logout()
