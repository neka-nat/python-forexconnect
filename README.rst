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

First, install the required packages.

    $ sudo apt-get install build-essential cmake libboost-log-dev libboost-date-time-dev libboost-python-dev

And then, download "ForexConnectAPI-1.4.1" from http://www.fxcodebase.com/wiki/index.php/Download
and set environment "FOREXCONNECT_ROOT" which is the path ForexConnectAPI installed.

    $ wget http://fxcodebase.com/bin/forexconnect/1.4.1/ForexConnectAPI-1.4.1-Linux-x86_64.tar.gz

    $ tar xvf ForexConnectAPI-1.4.1-Linux-x86_64.tar.gz

    $ export FOREXCONNECT_ROOT=$(pwd)/ForexConnectAPI-1.4.1-Linux-x86_64

    $ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/ForexConnectAPI-1.4.1-Linux-x86_64/lib

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
* ForexConnectAPI 1.4.1
