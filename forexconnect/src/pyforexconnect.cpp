#include "ForexConnectClient.h"
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
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

BOOST_PYTHON_MODULE(forexconnect)
{
    using namespace boost::python;
    PyDateTime_IMPORT;

    ptime_from_python_datetime();
    to_python_converter<const boost::posix_time::ptime, ptime_to_python_datetime>();

    class_<LoginParams>("LoginParams")
	.def(init<std::string, std::string, std::string>())
	.def_readwrite("login", &LoginParams::mLogin)
	.def_readwrite("password", &LoginParams::mPassword)
	.def_readwrite("connection", &LoginParams::mConnection);

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
	.def(self_ns::str(self));

    class_<std::vector<TradeInfo> >("vector<TradeInfo>")
	.def(vector_indexing_suite<std::vector<TradeInfo> >());

    class_<ForexConnectClient>("ForexConnectClient", init<LoginParams>())
	.def(init<std::string, std::string, std::string>())
	.def("get_trades", &ForexConnectClient::getTrades)
	.def("login", &ForexConnectClient::login)
	.def("logout", &ForexConnectClient::logout)
	.def("print_accounts", &ForexConnectClient::printAccounts);
}
