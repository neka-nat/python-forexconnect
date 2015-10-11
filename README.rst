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
   >> cl.logout()
