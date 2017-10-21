#include "ForexConnectClient.h"
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/log/trivial.hpp>
#include <datetime.h>
using namespace pyforexconnect;

static long get_usecs(boost::posix_time::time_duration const& d)
{
    static long resolution
	= boost::posix_time::time_duration::ticks_per_second();
    long fracsecs = d.fractional_seconds();
    if (resolution > 1000000)
	return fracsecs / (resolution / 1000000);
    else
	return fracsecs * (1000000 / resolution);
}


/* Convert ptime to/from python */
struct ptime_to_python_datetime
{
    static PyObject* convert(boost::posix_time::ptime const& pt)
    {
        boost::gregorian::date date = pt.date();
        boost::posix_time::time_duration td = pt.time_of_day();
        return PyDateTime_FromDateAndTime((int)date.year(),
					  (int)date.month(),
					  (int)date.day(),
					  td.hours(),
					  td.minutes(),
					  td.seconds(),
					  get_usecs(td));
    }
};


struct ptime_from_python_datetime
{
     ptime_from_python_datetime()
     {
         boost::python::converter::registry::push_back(&convertible,
						       &construct,
						       boost::python::type_id<boost::posix_time::ptime>());
     }

     static void* convertible(PyObject * obj_ptr)
     {
	 if (!PyDateTime_Check(obj_ptr))
	     return 0;
	 return obj_ptr;
     }

     static void construct(PyObject* obj_ptr,
			   boost::python::converter::rvalue_from_python_stage1_data * data)
     {
	 PyDateTime_DateTime const* pydate
	     = reinterpret_cast<PyDateTime_DateTime*>(obj_ptr);

	 // Create date object
	 boost::gregorian::date _date(PyDateTime_GET_YEAR(pydate),
				      PyDateTime_GET_MONTH(pydate),
				      PyDateTime_GET_DAY(pydate));

	 // Create time duration object
	 boost::posix_time::time_duration
	     _duration(PyDateTime_DATE_GET_HOUR(pydate),
		       PyDateTime_DATE_GET_MINUTE(pydate),
		       PyDateTime_DATE_GET_SECOND(pydate),
		       0);
	 // Set the usecs value
	 _duration += boost::posix_time::microseconds(PyDateTime_DATE_GET_MICROSECOND(pydate));

	 // Create posix time object
	 void* storage = ((boost::python::converter::rvalue_from_python_storage<boost::posix_time::ptime>*)
			  data)->storage.bytes;
	 new(storage) boost::posix_time::ptime(_date, _duration);
	 data->convertible = storage;
     }
};

struct prices_pickle_suite : boost::python::pickle_suite
{
    static boost::python::tuple getinitargs(const Prices& p)
    {
        return boost::python::make_tuple(p.mDate, p.mAskOpen, p.mAskHigh, p.mAskLow, p.mAskClose,
                                         p.mBidOpen, p.mBidHigh, p.mBidLow, p.mBidClose, p.mVolume);
    }

    static boost::python::tuple getstate(boost::python::object obj)
    {
        Prices const& p = boost::python::extract<Prices const&>(obj)();
	boost::python::dict d = boost::python::extract<boost::python::dict>(obj.attr("__dict__"));
	d["date"] = p.mDate;
	d["askopen"] = p.mAskOpen;
	d["askhigh"] = p.mAskHigh;
	d["asklow"] = p.mAskLow;
	d["askclose"] = p.mAskClose;
	d["bidopen"] = p.mBidOpen;
	d["bidhigh"] = p.mBidHigh;
	d["bidlow"] = p.mBidLow;
	d["bidclose"] = p.mBidClose;
	d["volume"] = p.mVolume;
        return boost::python::make_tuple(d);
    }

    static void setstate(boost::python::object obj, boost::python::tuple state)
    {
        using namespace boost::python;
	Prices& p = extract<Prices&>(obj)();

        if (len(state) != 1)
        {
	    PyErr_SetObject(PyExc_ValueError,
			    ("expected 1-item tuple in call to __setstate__; got %s"
			     % state).ptr()
		);
	    throw_error_already_set();
        }
	dict d = extract<dict>(state[0]);
	p.mDate = extract<boost::posix_time::ptime>(d["date"]);
	p.mAskOpen = extract<double>(d["askopen"]);
	p.mAskHigh = extract<double>(d["askhigh"]);
	p.mAskLow = extract<double>(d["asklow"]);
	p.mAskClose = extract<double>(d["askclose"]);
	p.mBidOpen = extract<double>(d["bidopen"]);
	p.mBidHigh = extract<double>(d["bidhigh"]);
	p.mBidLow = extract<double>(d["bidlow"]);
	p.mBidClose = extract<double>(d["bidclose"]);
	p.mVolume = extract<int>(d["volume"]);
    }

    static bool getstate_manages_dict() {return true;}
};

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(getHistoricalPricesForPythonOverloads,
				       ForexConnectClient::getHistoricalPricesForPython, 3, 4);

BOOST_PYTHON_MODULE(forexconnect)
{
    setLogLevel(boost::log::trivial::info);
    using namespace boost::python;
    PyDateTime_IMPORT;

    ptime_from_python_datetime();
    to_python_converter<const boost::posix_time::ptime, ptime_to_python_datetime>();

    scope().attr("TF_m1") = std::string("m1");
    scope().attr("TF_m5") = std::string("m5");
    scope().attr("TF_H1") = std::string("H1");
    scope().attr("TF_D1") = std::string("D1");
    scope().attr("TF_W1") = std::string("W1");
    scope().attr("BUY") = std::string(O2G2::Buy);
    scope().attr("SELL") = std::string(O2G2::Sell);

    scope().attr("LOG_TRACE") = static_cast<int>(boost::log::trivial::trace);
    scope().attr("LOG_DEBUG") = static_cast<int>(boost::log::trivial::debug);
    scope().attr("LOG_INFO") = static_cast<int>(boost::log::trivial::info);
    scope().attr("LOG_WARNING") = static_cast<int>(boost::log::trivial::warning);
    scope().attr("LOG_ERROR") = static_cast<int>(boost::log::trivial::error);
    scope().attr("LOG_FATAL") = static_cast<int>(boost::log::trivial::fatal);
    def("set_log_level", setLogLevel);

    class_<LoginParams>("LoginParams")
	.def(init<std::string, std::string, std::string>())
	.def(init<std::string, std::string, std::string, std::string>())
	.def_readwrite("login", &LoginParams::mLogin)
	.def_readwrite("password", &LoginParams::mPassword)
	.def_readwrite("connection", &LoginParams::mConnection)
	.def_readwrite("url", &LoginParams::mUrl)
	.def(self_ns::str(self))
	.def(self_ns::repr(self));

    class_<AccountInfo>("AccountInfo")
	.def_readwrite("balance", &AccountInfo::mBalance)
	.def_readwrite("used_margin", &AccountInfo::mUsedMargin)
	.def_readwrite("usable_margin", &AccountInfo::mUsableMargin)
	.def_readwrite("base_unit_size", &AccountInfo::mBaseUnitSize)
	.def_readwrite("equity", &AccountInfo::mEquity)
	.def_readwrite("gross_pl", &AccountInfo::mGrossPL)
	.def(self_ns::str(self))
	.def(self_ns::repr(self));

    class_<TradeInfo>("TradeInfo")
	.def_readwrite("instrument", &TradeInfo::mInstrument)
	.def_readwrite("trade_id", &TradeInfo::mTradeID)
	.def_readwrite("buy_sell", &TradeInfo::mBuySell)
	.def_readwrite("open_rate", &TradeInfo::mOpenRate)
	.def_readwrite("amount", &TradeInfo::mAmount)
	.add_property("open_date",
		      make_getter(&TradeInfo::mOpenDate, return_value_policy<return_by_value>()),
		      make_setter(&TradeInfo::mOpenDate, return_value_policy<copy_non_const_reference>()))
	.def_readwrite("gross_pl", &TradeInfo::mGrossPL)
	.def(self_ns::str(self))
	.def(self_ns::repr(self));

    class_<Prices>("Prices")
	.def(init<boost::posix_time::ptime, double, double, double, double, double, double, double, double, int>())
	.add_property("date",
		      make_getter(&Prices::mDate, return_value_policy<return_by_value>()),
		      make_setter(&Prices::mDate, return_value_policy<copy_non_const_reference>()))
	.def_readwrite("askopen", &Prices::mAskOpen)
	.def_readwrite("askhigh", &Prices::mAskHigh)
	.def_readwrite("asklow", &Prices::mAskLow)
	.def_readwrite("askclose", &Prices::mAskClose)
	.def_readwrite("bidopen", &Prices::mBidOpen)
	.def_readwrite("bidhigh", &Prices::mBidHigh)
	.def_readwrite("bidlow", &Prices::mBidLow)
	.def_readwrite("bidclose", &Prices::mBidClose)
	.def_readwrite("volume", &Prices::mVolume)
	.def(self_ns::str(self))
	.def(self_ns::repr(self))
	.def_pickle(prices_pickle_suite());

    class_<ForexConnectClient>("ForexConnectClient", init<LoginParams>())
	.def(init<std::string, std::string, std::string>())
	.def(init<std::string, std::string, std::string, std::string>())
	.def("get_trades", &ForexConnectClient::getTradesForPython)
	.def("get_offers", &ForexConnectClient::getOffersForPython)
	.def("is_connected", &ForexConnectClient::isConnected)
	.def("open_position", &ForexConnectClient::openPosition,
	     ":param str instrument: type of quotes(ex. 'EUR/USD')\n:param str buysell: buy or sell\n:param int amount: amount of position")
	.def("close_position", &ForexConnectClient::closePosition,
	    ":param str tradeID: trade id which can get from 'get_trades'")
	.def("get_bid", &ForexConnectClient::getBid)
	.def("get_ask", &ForexConnectClient::getAsk)
	.def("get_historical_prices",
	     &ForexConnectClient::getHistoricalPricesForPython,
	     getHistoricalPricesForPythonOverloads())
	.def("login", &ForexConnectClient::login)
	.def("logout", &ForexConnectClient::logout)
	.def("get_account_id", &ForexConnectClient::getAccountID)
	.def("get_account_info", &ForexConnectClient::getAccountInfo);
}
