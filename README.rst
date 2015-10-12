pyforexconnect
==============

About
------
This library is a Python binding of Forexconnect API.

Build
-----

    $ cd forexconnect

    $ mkdir build

    $ cd build

    $ cmake ..

    $ make

    $ make install

Usage
------

::

   >> import forexconnect
   >> cl = forexconnect.ForexConnectClient("usrname", "pass", "Real")
   >> cl.open_position("EUR/JPY", "B", 1)
   >> ti = cl.get_trades()
   >> cl.close_position(ti[0].trade_id)
   >> cl.logout()
