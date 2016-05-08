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
print client.get_account_info()
trades = client.get_trades()
for t in trades:
    print t
client.logout()
