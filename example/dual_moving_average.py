"""
An algorithm trading example using dual moving average.
"""
import sys
import forexconnect
import time
import datetime
import lib.login_manager as lm
import lib.realtime_chart as rc
import collections
import numpy as np
import talib

class Trader:
    _DATA_LEN = 100
    def __init__(self, client, instrument):
        self._client = client
        self._instrument = instrument
        self._data = collections.deque(maxlen = self._DATA_LEN)
        self._held_trade = None
        cur_data = client.get_historical_prices(self._instrument,
                                                datetime.datetime.now() - datetime.timedelta(minutes=self._DATA_LEN + 1),
                                                datetime.datetime.now())
        self._data.extend([c.close for c in cur_data[::-1]])
        short_mavg = talib.SMA(np.array(self._data), timeperiod = 5)
        long_mavg = talib.SMA(np.array(self._data), timeperiod = 25)
        short_mavg[np.isnan(short_mavg)] = short_mavg[np.isfinite(short_mavg)][0]
        long_mavg[np.isnan(long_mavg)] = long_mavg[np.isfinite(long_mavg)][0]
        rc.add_data("close", self._data)
        rc.add_data("short_mavg", short_mavg)
        rc.add_data("long_mavg", long_mavg)
        rc.init()

    def buy(self):
        if (not self._held_trade is None) and self._held_trade.buy_sell == forexconnect.SELL:
            res = self._client.close_position(self._held_trade.trade_id)
            if res:
                self._held_trade = None
        else:
            res = self._client.open_position(self._instrument, forexconnect.BUY, 1)
            if res:
                self._held_trade = self.get_latest_trade()
        return res

    def sell(self):
        if (not self._held_trade is None) and self._held_trade.buy_sell == forexconnect.BUY:
            res = self._client.close_position(self._held_trade.trade_id)
            if res:
                self._held_trade = None
        else:
            res = self._client.open_position(self._instrument, forexconnect.SELL, 1)
            if res:
                self._held_trade = self.get_latest_trade()
        return res

    def get_latest_trade(self):
        trades = self._client.get_trades()
        return max(enumerate(trades), key=lambda x: x[1].open_date)[1]

    def tick(self):
        self._data.append(self._client.get_ask(self._instrument))
        short_mavg = talib.SMA(np.array(self._data), timeperiod = 5)
        long_mavg = talib.SMA(np.array(self._data), timeperiod = 25)

        res = True
        if short_mavg[-2] < long_mavg[-2] and short_mavg[-1] > long_mavg[-1]:
            print datetime.datetime.now(), "Buy signal", self._instrument, self._data[-1]
            res = self.buy()
        elif short_mavg[-2] > long_mavg[-2] and short_mavg[-1] < long_mavg[-1]:
            print datetime.datetime.now(), "Sell signal", self._instrument, self._data[-1]
            res = self.sell()
        if not res:
            print "Fail to operate position."
        print "Balance:", self._client.get_balance()

        short_mavg[np.isnan(short_mavg)] = short_mavg[np.isfinite(short_mavg)][0]
        long_mavg[np.isnan(long_mavg)] = long_mavg[np.isfinite(long_mavg)][0]
        rc.update_data({"close": self._data,
                        "short_mavg": short_mavg,
                        "long_mavg": long_mavg}, True)


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print "Usage: python dual_moving_average.py instrument(etc. 'EUR/USD')"
        sys.exit()

    instrument = sys.argv[1]
    username, password, connection = lm.get_login_params()
    try:
        client = forexconnect.ForexConnectClient(username,
                                                 password,
                                                 connection)
    except:
        lm.clear_cache()
        sys.exit()
    trader = Trader(client, instrument)
    print "Start trading..."

    interval_sec = 60.0
    while True:
        start = time.time()
        print datetime.datetime.now()
        trader.tick()
        time.sleep(interval_sec - (time.time() - start))
