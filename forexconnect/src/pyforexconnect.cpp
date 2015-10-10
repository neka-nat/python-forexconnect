#include "ForexConnectClient.h"
#include <boost/python.hpp>
using namespace pyforexconnect;

BOOST_PYTHON_MODULE(forexconnect)
{
    using namespace boost::python;
    class_<LoginParams>("LoginParams")
	.def(init<std::string, std::string, std::string>())
	.def_readwrite("login", &LoginParams::mLogin)
	.def_readwrite("password", &LoginParams::mPassword)
	.def_readwrite("connection", &LoginParams::mConnection);

    class_<ForexConnectClient>("ForexConnectClient", init<LoginParams>())
	.def(init<std::string, std::string, std::string>())
	.def("login", &ForexConnectClient::login)
	.def("logout", &ForexConnectClient::logout)
	.def("print_accounts", &ForexConnectClient::printAccounts);
}
