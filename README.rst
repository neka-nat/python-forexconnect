python-forexconnect
===================

About
------
This library is a Python binding of Forexconnect API.

Build
-----

First, set environment "FOREXCONNECT_ROOT" which is the path ForexConnectAPI installed.

    $ export FOREXCONNECT_ROOT=/.../ForexConnectAPI

Next, clone this repository.

    $ git clone https://github.com/neka-nat/python-forexconnect.git

Build and install.

    $ cd python-forexconnect

    $ mkdir build

    $ cd build

    $ cmake ..

    $ make

    $ make install

Usage
------

This tutorial is simple trading using python-forexconnect.
First, give FXCM account username, password and type ("Real" or "Demo") to login.
Next, send query to open position and get the position list which you have.
Finally, close the opened position and logout.

::

   >> import forexconnect
   >> cl = forexconnect.ForexConnectClient("usrname", "pass", "Real")
   >> cl.open_position("EUR/JPY", "B", 1)
   >> ti = cl.get_trades()
   >> cl.close_position(ti[0].trade_id)
   >> cl.logout()
