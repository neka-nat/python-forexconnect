python-forexconnect
===================

.. image:: https://travis-ci.org/neka-nat/python-forexconnect.svg?branch=master
    :target: https://travis-ci.org/neka-nat/python-forexconnect

About
------
This library is a Python binding of Forexconnect API
using boost.python.

Build
-----

First, download "ForexConnectAPI-1.3.2" from http://www.fxcodebase.com/wiki/index.php/Download
and set environment "FOREXCONNECT_ROOT" which is the path ForexConnectAPI installed.

    $ export FOREXCONNECT_ROOT=/.../ForexConnectAPI

Next, clone this repository.

    $ git clone https://github.com/neka-nat/python-forexconnect.git

Build and install.

    $ cd python-forexconnect

    $ mkdir build

    $ cd build

    $ cmake .. -DDEFAULT_FOREX_URL="http://<Your forexconnect URL>"

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
   >> cl.open_position("EUR/JPY", forexconnect.BUY, 1)
   >> ti = cl.get_trades()
   >> cl.close_position(ti[0].trade_id)
   >> cl.logout()

Requirements
-------------

* boost 1.54
* cython 0.20
* ForexConnectAPI 1.3.2
