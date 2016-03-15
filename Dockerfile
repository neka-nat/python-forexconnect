### misc ###

FROM ubuntu:latest

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update
RUN apt-get install -y wget git
RUN apt-get -y install make cmake build-essential
RUN apt-get -y install libboost-all-dev
RUN apt-get -y install python-dev

### forexconnect ###

RUN mkdir -p /opt/forexconnect

WORKDIR /opt/forexconnect

RUN wget http://fxcodebase.com/bin/forexconnect/1.4.1/ForexConnectAPI-1.4.1-Linux-x86_64.tar.gz
RUN tar -xzf ForexConnectAPI-1.4.1-Linux-x86_64.tar.gz

ENV FOREXCONNECT_ROOT /opt/forexconnect/ForexConnectAPI-1.4.1-Linux-x86_64


### python-forexconnect###

RUN git clone https://github.com/neka-nat/python-forexconnect.git

RUN cd python-forexconnect; mkdir build; cd build; cmake .. -DDEFAULT_FOREX_URL="http://www.fxcorporate.com/Hosts.jsp"; make install

ENV LD_LIBRARY_PATH /opt/forexconnect/python-forexconnect/build/forexconnect/sample_tools/lib/:/opt/forexconnect/ForexConnectAPI-1.4.1-Linux-x86_64/lib
