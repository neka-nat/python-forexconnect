python-forexconnect
===================

.. image:: https://travis-ci.org/neka-nat/python-forexconnect.svg?branch=master
    :target: https://travis-ci.org/neka-nat/python-forexconnect

About
------
This library is a Python binding of Forexconnect API using boost.python.

Requirements
-------------

* Python 3.x
* boost 1.65
* ForexConnectAPI 1.4.1

Build
-----

Download "ForexConnectAPI-1.4.1" from http://www.fxcodebase.com/wiki/index.php/Download
and set environment "FOREXCONNECT_ROOT" which is the path ForexConnectAPI installed.

    $ wget http://fxcodebase.com/bin/forexconnect/1.4.1/ForexConnectAPI-1.4.1-Linux-x86_64.tar.gz

    $ tar xvf ForexConnectAPI-1.4.1-Linux-x86_64.tar.gz
    
    $ mv ForexConnectAPI-1.4.1-Linux-x86_64 ForexConnectAPI

    $ export FOREXCONNECT_ROOT=$(pwd)/ForexConnectAPI

    $ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/ForexConnectAPI/lib

Next, clone this repository.

    $ git clone https://github.com/neka-nat/python-forexconnect.git

Build and install.

    $ cd python-forexconnect

    $ mkdir build

    $ cd build

    $ cmake .. -DDEFAULT_FOREX_URL="http://<Your forexconnect URL>"
    
    example url: "http://www.fxcorporate.com/Hosts.jsp"

    $ sudo make install


Usage
------

This tutorial is simple trading using python-forexconnect.
First, give FXCM account username, password and type ("Real" or "Demo") to login.
Next, send query to open position and get the position list which you have.
Finally, close the opened position and logout.

::

   >> import forexconnect as fx
   >> client = fx.ForexConnectClient("usrname", "pass", "Real")
   >> client.get_offers()
   >> cl.logout()
